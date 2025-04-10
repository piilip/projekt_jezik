#include "Codegen.h"
#include "Exp.h"
#include "FuncDef.h"
#include "Program.h"
#include "Stmt.h"
#include "Visitor.h"

#include <llvm/IR/Argument.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/raw_ostream.h>
#include <map>

using namespace llvm;

// local variables are mapped to the stack allocated pointers, while parameters are mapped to the llvm equivalent values
using SymbolTable = std::map<const VarDecl *, Value *>;

// Maps function definitions to the llvm equivalent
using FunctionTable = std::map<const FuncDef *, Function *>;

namespace {

// Class that holds llvm objects, and also helper functions that will help us in the Codegen phase
    class CodegenBase {
    public:

        CodegenBase(LLVMContext *context, Module *module, IRBuilder<> *builder)
                : m_context(context), m_module(module), m_builder(builder), m_boolType(IntegerType::get(*m_context, 1)),
                  m_intType(IntegerType::get(*m_context, 32)), m_floatType(llvm::Type::getFloatTy(*m_context)) {
            llvm::FunctionType *printfType = llvm::FunctionType::get(llvm::IntegerType::getInt32Ty(*m_context),
                                                                     {llvm::PointerType::get(
                                                                             llvm::Type::getInt8Ty(*m_context), 0)},
                                                                     true);
            m_module->getOrInsertFunction("printf", printfType);

            llvm::FunctionType *exitType = llvm::FunctionType::get(llvm::Type::getVoidTy(*m_context),
                                                                   {llvm::IntegerType::getInt32Ty(*m_context)}, false);
            m_module->getOrInsertFunction("exit", exitType);
        }

        LLVMContext *getContext() { return m_context; }

        Module *getModule() { return m_module; }

        IRBuilder<> *getBuilder() { return m_builder; }

        // Convert a type to its LLVM equivalent.
        llvm::Type *ConvertType(::Type type) {
            switch (type) {
                case kTypeBool:
                    return m_boolType;
                case kTypeInt:
                    return m_intType;
                case kTypeFloat:
                    return m_floatType;
                case kTypeUnknown:
                    assert(false && "Invalid type");
                    return nullptr;
            }
            return nullptr;
        }

        std::pair<Value*, Value*> EnsureSameType(llvm::Value *lhs, llvm::Value *rhs) {
            llvm::Type *lhsType = lhs->getType();
            llvm::Type *rhsType = rhs->getType();
            llvm::Value* zero = GetFloat(0);
            if(lhsType->getTypeID()==rhsType->getTypeID()){
                return std::pair(lhs, rhs);
            }

            if (lhsType != rhsType) {
                // INT -> FLOAT
                if (lhsType->isIntegerTy(32)&& rhsType->isFloatTy()) {
                    lhs = getBuilder()->CreateSIToFP(lhs, rhsType, "inttofloat");
                } else if (lhsType->isFloatTy() && rhsType->isIntegerTy(32)) {
                    rhs = getBuilder()->CreateSIToFP(rhs, lhsType, "inttofloat");
                }
                    // INT -> BOOL
                else if (lhsType->isIntegerTy(32)&& rhsType->isIntegerTy(1)) {
                    lhs = convertToTargetType(lhs, rhsType);
                } else if (lhsType->isIntegerTy(1) && rhsType->isIntegerTy(32)) {
                    rhs = convertToTargetType(rhs, lhsType);
                }
                    // FLOAT -> BOOL
                else if (lhsType->isIntegerTy(1)&& rhsType->isFloatTy()) {
                    rhs = getBuilder()->CreateFCmpONE(lhs, zero, "floattobool");
                } else if (lhsType->isFloatTy() && rhsType->isIntegerTy(1)) {
                    lhs = getBuilder()->CreateFCmpONE(rhs, zero, "floattobool");
                }
            }
            return std::pair(lhs, rhs);
        }

        llvm::Value* convertToTargetType(llvm::Value* value, llvm::Type* target) {
            llvm::Type* sourceType = value->getType();
            if(sourceType == target){
                return value;
            }

            // int -> float
            if (sourceType->isIntegerTy() && target->isFloatTy()) {
                return m_builder->CreateSIToFP(value, target, "intToFloat");
            }
                // float -> int
            else if (sourceType->isFloatTy() && target->isIntegerTy()) {
                return m_builder->CreateFPToSI(value, target, "floatToInt");
            }
                // int -> bool (bool is i1)
            else if (sourceType->isIntegerTy() && target->isIntegerTy(1)) {
                llvm::Value* zero = llvm::ConstantInt::get(sourceType, 0);
                return m_builder->CreateICmpNE(value, zero, "intToBool");
            }
                // bool -> int
            else if (sourceType->isIntegerTy(1) && target->isIntegerTy()) {
                return m_builder->CreateZExt(value, target, "boolToInt");
            }
                // float -> bool
            else if (sourceType->isFloatTy() && target->isIntegerTy(1)) {
                llvm::Value* zero = llvm::ConstantFP::get(sourceType, 0.0);
                return m_builder->CreateFCmpONE(value, zero, "floatToBool");
            }
                // bool -> float
            else if (sourceType->isIntegerTy(1) && target->isFloatTy()) {
                return  m_builder->CreateUIToFP(value, target, "boolToFloat");
            }
                // float -> double Because of printf
            else if (sourceType->isFloatTy() && target->isDoubleTy()) {
                return  m_builder->CreateFPExt(value, target, "floatToDouble");
            }


            return value;
        }


        llvm::Type *GetBoolType() const { return m_boolType; }

        llvm::Type *GetIntType() const { return m_intType; }

        llvm::Type *GetFloatType() const { return m_floatType; }


        Constant *GetBool(bool b) const { return ConstantInt::get(GetBoolType(), int(b), false); }

        Constant *GetFloat(float f) const { return ConstantFP::get(GetFloatType(), f); }

        Constant *GetInt(int i) const { return ConstantInt::get(GetIntType(), i, true); }

    protected:
        LLVMContext *m_context;
        Module *m_module;
        IRBuilder<> *m_builder;
        llvm::Type *m_boolType;
        llvm::Type *m_intType;
        llvm::Type *m_floatType;
    };


    class CodegenExp : public ExpVisitor, CodegenBase {
    public:
        CodegenExp(LLVMContext *context, Module *module, IRBuilder<> *builder,
                   SymbolTable *symbols, FunctionTable *functions)
                : CodegenBase(context, module, builder), m_symbols(symbols), m_functions(functions) {
        }


        Value *Codegen(const Exp &exp) { return reinterpret_cast<Value *>( const_cast<Exp &>( exp ).Dispatch(*this)); }

        void *Visit(BoolExp &exp) override { return GetBool(exp.getValue()); }

        void *Visit(IntExp &exp) override { return GetInt(exp.getValue()); }

        void *Visit(FloatExp &exp) override { return GetFloat(exp.getValue()); }

        void *Visit(ArrayAccessExp &exp) override {

            const VarDecl *varDecl = exp.getVarDecl();
            auto it = m_symbols->find(varDecl);
            assert(it != m_symbols->end());
            Value *arrayPtr = it->second;

            llvm::AllocaInst *allocInst = llvm::dyn_cast<llvm::AllocaInst>(arrayPtr);
            auto var1 = allocInst->getAllocatedType();
            //auto var2 = var1->getArrayElementType();
            //auto var3 = allocInst->getArraySize();

            Value *index = Codegen(*exp.getIndexExp());
            if(allocInst->getAllocatedType()->isArrayTy()) {
                std::vector<Value *> indices;
                Value *zero = GetInt(0);
                indices.push_back(zero); // First index: 0 for the array pointer
                indices.push_back(index); // Second index: your desired index
                Value *elementPtr = getBuilder()->CreateInBoundsGEP(allocInst->getAllocatedType(), allocInst, indices);


                return getBuilder()->CreateLoad(allocInst->getAllocatedType()->getArrayElementType(), elementPtr,
                                                "load");

            }else{
                Value *elementPtr = getBuilder()->CreateInBoundsGEP(allocInst->getAllocatedType(), allocInst, index, "molim");
                return getBuilder()->CreateLoad(allocInst->getAllocatedType(), elementPtr, "load");
            }

        }

        void *Visit(VarExp &exp) override {

            const VarDecl *varDecl = exp.getVarDecl();
            assert(varDecl);


            auto it = m_symbols->find(varDecl);
            assert(it != m_symbols->end() && "We couldn't find the variable: Calling from Codegen VarExp");
            Value *value = it->second;

            // The value is either a function parameter or a pointer to storage for a local variable.
            auto kindOf = varDecl->GetKind();
            switch (kindOf) {
                case VarDecl::kParam:
                    return value;
                case VarDecl::kLocal:
                    return getBuilder()->CreateLoad(this->ConvertType(varDecl->GetType()), value, varDecl->GetName());
            }
            return nullptr;
        }

        // Generate code for a function call.
        void *Visit(CallExp &exp) override {

            const std::string &funcName = exp.getFuncName();
            if (funcName == "&&") {

                llvm::Value* lhs = Codegen(*exp.getArgs().at(0));
                llvm::Function *func = getBuilder()->GetInsertBlock()->getParent();
                llvm::BasicBlock *rhsBlock = llvm::BasicBlock::Create(getBuilder()->getContext(), "and.rhs", func);
                llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(getBuilder()->getContext(), "and.end");

                getBuilder()->CreateCondBr(lhs, rhsBlock, endBlock);

                getBuilder()->SetInsertPoint(rhsBlock);
                llvm::Value* rhs = Codegen(*exp.getArgs().at(1));
                getBuilder()->CreateBr(endBlock);

                rhsBlock = getBuilder()->GetInsertBlock();
                func->insert(func->end(),endBlock);
                getBuilder()->SetInsertPoint(endBlock);
                llvm::PHINode *phi = getBuilder()->CreatePHI(getBuilder()->getInt1Ty(), 2);
                phi->addIncoming(getBuilder()->getFalse(), getBuilder()->GetInsertBlock()->getPrevNode()->getPrevNode());
                phi->addIncoming(rhs, rhsBlock);

                return phi;
            } else if (funcName == "||") {

                llvm::Value* lhs = Codegen(*exp.getArgs().at(0));
                llvm::Function *func = getBuilder()->GetInsertBlock()->getParent();
                llvm::BasicBlock *rhsBlock = llvm::BasicBlock::Create(getBuilder()->getContext(), "or.rhs", func);
                llvm::BasicBlock *endBlock = llvm::BasicBlock::Create(getBuilder()->getContext(), "or.end");

                getBuilder()->CreateCondBr(lhs, endBlock, rhsBlock);

                getBuilder()->SetInsertPoint(rhsBlock);
                llvm::Value* rhs = Codegen(*exp.getArgs().at(1));; // Evaluate RHS here
                getBuilder()->CreateBr(endBlock);

                rhsBlock = getBuilder()->GetInsertBlock();
                func->insert(func->end(),endBlock);
                getBuilder()->SetInsertPoint(endBlock);
                llvm::PHINode *phi = getBuilder()->CreatePHI(getBuilder()->getInt1Ty(), 2);
                phi->addIncoming(getBuilder()->getTrue(), getBuilder()->GetInsertBlock()->getPrevNode()->getPrevNode());
                phi->addIncoming(rhs, rhsBlock);

                return phi;
            }

            std::vector<Value *> args;
            args.reserve(exp.getArgs().size());


            for (const ExpPtr &arg: exp.getArgs()) {
                args.push_back(Codegen(*arg));
            }
            if (args.size() == 2) {
                auto arguments= EnsureSameType(args[0], args[1]);
                args[0]=arguments.first;
                args[1]=arguments.second;
            }

            auto returnType = ConvertType(exp.getType());

            if (funcName == "+") {
                return returnType->isFloatTy() ?
                       getBuilder()->CreateFAdd(args[0], args[1]) :
                       getBuilder()->CreateAdd(args[0], args[1]);
            } else if (funcName == "-") {
                if( args.size() == 1 ) {
                    return returnType->isFloatTy() ?
                           getBuilder()->CreateFNeg(args[0]) :
                           getBuilder()->CreateNeg(args[0]);
                }else {
                    return returnType->isFloatTy() ?
                           getBuilder()->CreateFSub(args[0], args[1]) :
                           getBuilder()->CreateSub(args[0], args[1]);
                }
            } else if (funcName == "*") {
                return returnType->isFloatTy() ?
                       getBuilder()->CreateFMul(args[0], args[1]) :
                       getBuilder()->CreateMul(args[0], args[1]);
            } else if (funcName == "/") {
                if (returnType->isFloatTy()) {

                    return getBuilder()->CreateFDiv(args[0], args[1]);
                } else {
                    return getBuilder()->CreateSDiv(args[0], args[1]);
                }
            } else if (funcName == "%") {
                if (returnType->isFloatTy()) {
                    return getBuilder()->CreateFRem(args[0], args[1]);
                } else {
                    return getBuilder()->CreateSRem(args[0], args[1]);
                }
            }
            else if (funcName == "!=") {
                return args[0]->getType()->isFloatTy() ?
                       getBuilder()->CreateFCmpUNE(args[0], args[1]) :
                       getBuilder()->CreateICmpNE(args[0], args[1]);
            } else if (funcName == "<") {
                return args[0]->getType()->isFloatTy() ?
                       getBuilder()->CreateFCmpOLT(args[0], args[1], "floatLT") :
                       getBuilder()->CreateICmpSLT(args[0], args[1]);
            } else if (funcName == "<=") {
                return args[0]->getType()->isFloatTy() ?
                       getBuilder()->CreateFCmpULE(args[0], args[1]) :
                       getBuilder()->CreateICmpSLE(args[0], args[1]);
            } else if (funcName == ">") {
                return args[0]->getType()->isFloatTy() ?
                       getBuilder()->CreateFCmpUGT(args[0], args[1]) :
                       getBuilder()->CreateICmpSGT(args[0], args[1]);
            } else if (funcName == ">=") {
                return args[0]->getType()->isFloatTy() ?
                       getBuilder()->CreateFCmpUGE(args[0], args[1]) :
                       getBuilder()->CreateICmpSGE(args[0], args[1]);
            } else if (funcName == "==") {
                return args[0]->getType()->isFloatTy() ?
                       getBuilder()->CreateFCmpUEQ(args[0], args[1]) :
                       getBuilder()->CreateICmpEQ(args[0], args[1]);
            } else if (funcName == "!") {
                return getBuilder()->CreateICmpEQ(args[0], GetBool(false));
            }  else if (funcName == "float") {
                if (args[0]->getType()->isIntegerTy()) {
                    return getBuilder()->CreateSIToFP(args[0], GetFloatType(), "intToFloat");
                } else if (args[0]->getType()->isIntegerTy(1)) {
                    return getBuilder()->CreateUIToFP(args[0], GetFloatType(), "boolToFloat");
                }
            } else if (funcName == "bool") {
                if (args[0]->getType()->isIntegerTy()) {
                    return getBuilder()->CreateICmpNE(args[0], GetInt(0), "intToBool");
                } else if (args[0]->getType()->isFloatTy()) {
                    return getBuilder()->CreateFCmpUNE(args[0], llvm::ConstantFP::get(GetFloatType(), 0.0), "floatToBool");
                }
            } else if (funcName == "int") {
                if (args[0]->getType()->isFloatTy()) {
                    return getBuilder()->CreateFPToSI(args[0], GetIntType(), "floatToint");
                } else {
                    return getBuilder()->CreateZExt(args.at(0), GetIntType());
                }
            } else if (funcName == "print") {

                llvm::Function *printFunc = m_module->getFunction("printf");
                if (!printFunc) {
                    llvm::FunctionType *printFuncType = llvm::FunctionType::get(
                            llvm::IntegerType::getInt32Ty(*m_context),
                            llvm::PointerType::get(
                                    llvm::IntegerType::getInt8Ty(*m_context), 0), true);
                    printFunc = llvm::Function::Create(printFuncType, llvm::Function::ExternalLinkage, "printf",
                                                       m_module);
                }
                if (!printFunc) {
                    throw std::runtime_error("Printf function not found");
                }
                std::vector<llvm::Value *> printfArgs;
                auto argType = args.at(0)->getType();
                if(argType->isFloatTy()){

                    printfArgs.push_back(m_builder->CreateGlobalStringPtr("%f\n"));
                    args[0] = convertToTargetType(args[0], getBuilder()->getDoubleTy());
                    printfArgs.push_back(args[0]);
                } else if (argType->isIntegerTy(32)){
                    printfArgs.push_back(m_builder->CreateGlobalStringPtr("%d\n"));
                    printfArgs.push_back(args[0]);
                } else if (argType->isIntegerTy(1)){
                    llvm::Value* trueStr = getBuilder()->CreateGlobalStringPtr("true");
                    llvm::Value* falseStr = getBuilder()->CreateGlobalStringPtr("false");

                    llvm::Value* isTrue = getBuilder()->CreateICmpNE(args[0], llvm::ConstantInt::get(GetBoolType(), 0), "isTrue");

                    auto trueOrFalse = getBuilder()->CreateSelect(isTrue, trueStr, falseStr, "boolToStr");
                    printfArgs.push_back(m_builder->CreateGlobalStringPtr("%s\n"));
                    printfArgs.push_back(trueOrFalse);
                }

                return m_builder->CreateCall(printFunc, llvm::ArrayRef<llvm::Value *>(printfArgs));
            }


            // We can get the function definition from CallExp
            const FuncDef *funcDef = exp.getFuncDef();
            assert(funcDef);

            // From the function definition we can find the llvm equivalent in a map
            auto it = m_functions->find(funcDef);
            assert(it != m_functions->end());
            Function *function = it->second;

            // Generate LLVM function call.
            return getBuilder()->CreateCall(function, args, funcDef->getName());
        }

    private:
        SymbolTable *m_symbols;
        FunctionTable *m_functions;
    };


// Statement code generator.
    class CodegenStmt : public StmtVisitor, CodegenBase {
    public:
        CodegenStmt(LLVMContext *context, Module *module, IRBuilder<> *builder,
                    SymbolTable *symbols, FunctionTable *functions, Function *currentFunction)
                : CodegenBase(context, module, builder), m_symbols(symbols), m_functions(functions),
                  m_currentFunction(currentFunction), m_codegenExp(context, module, builder, symbols, functions) {
        }

        void Codegen(const Stmt &stmt) {
            const_cast<Stmt &>( stmt ).Dispatch(*this);
        }

        void Visit(ArrayAssignStmt &stmt) override {

            const VarDecl *varDecl = stmt.GetVarDecl();
            llvm::Type *elementType = ConvertType(varDecl->GetType());

            // Get the location (pointer) of the array from the symbol table
            auto it = m_symbols->find(varDecl);
            assert(it != m_symbols->end() && "Array wasn't mapped to a pointer");

            Value *arrayLocation = it->second;

            llvm::AllocaInst *allocInst = llvm::dyn_cast<llvm::AllocaInst>(arrayLocation);
            //auto var1 = allocInst->getAllocatedType();
            //auto var2 = var1->getArrayElementType();
            //auto var3 = allocInst->getArraySize();

            // Create an index value for the element you want to assign
            Value *rvalue = m_codegenExp.Codegen(stmt.GetRvalue());
            Value *index = m_codegenExp.Codegen(*stmt.getIndexExp());


            rvalue = convertToTargetType(rvalue, allocInst->getAllocatedType());
            if(allocInst->getAllocatedType()->isArrayTy()){
                std::vector<Value *> indices;
                Value *zero = GetInt(0);
                indices.push_back(zero); // First index: 0 for the array pointer (this is useful when dealing with multiple dimension arrays)
                indices.push_back(index); // Second index: desired index
                Value *elementPtr = getBuilder()->CreateInBoundsGEP(allocInst->getAllocatedType(), allocInst, indices, "idemo1");

                // Store the new value into the array element
                getBuilder()->CreateStore(rvalue, elementPtr);
            }else{
                Value *elementPtr = getBuilder()->CreateInBoundsGEP(allocInst->getAllocatedType(), allocInst, index, "halooo");
                getBuilder()->CreateStore(rvalue, elementPtr);
            }

        }

        void Visit(CallStmt &stmt) override {
            m_codegenExp.Codegen(stmt.GetCallExp());
        }


        // Generate code for an assignment statement.
        void Visit(AssignStmt &stmt) override {


            const VarDecl *varDecl = stmt.GetVarDecl();
            assert(varDecl && varDecl->GetKind() == VarDecl::kLocal);

            // We search for the location of our stack allocated llvm::value in the table
            auto it = m_symbols->find(varDecl);
            assert(it != m_symbols->end());
            Value *location = it->second;

            // Generate code for the rvalue and store it.
            Value *rvalue = m_codegenExp.Codegen(stmt.GetRvalue());
            rvalue = convertToTargetType(rvalue, ConvertType(varDecl->GetType()));

            getBuilder()->CreateStore(rvalue, location);
        }


        // Generate code for a local variable declaration.
        void Visit(DeclStmt &stmt) override {

            // There's no initializer for arrays, for now at least
            const VarDecl *varDecl = stmt.GetVarDecl();
            llvm::Type *type = ConvertType(varDecl->GetType());
            if (varDecl->GetIsArray()) {

                auto arraySizeValue = m_codegenExp.Codegen(varDecl->getVariable().getArraySizeExp());
                auto *constArraySize = llvm::dyn_cast<llvm::ConstantInt>(arraySizeValue);
                int64_t arraySize = 0;
                // If the array size is constant like int a[2+3] or a[6]
                if (constArraySize) {
                    arraySize = constArraySize->getSExtValue();
                    if (arraySize <= 0) {
                        throw std::runtime_error("Array size must be > 0");
                    }
                    // Create an array type
                    llvm::ArrayType *arrayType = llvm::ArrayType::get(type, arraySize);


                    AllocaInst *arrayAlloc = getBuilder()->CreateAlloca(arrayType, nullptr, varDecl->GetName());

                    // Store the array location in the symbol table.
                    m_symbols->insert(SymbolTable::value_type(varDecl, arrayAlloc));
                }
                else{

                    auto zero = llvm::ConstantInt::get(arraySizeValue->getType(), 0);
                    llvm::Value *isGreaterThanZero = getBuilder()->CreateICmpSGT(arraySizeValue, zero);

                    llvm::Function *func = getBuilder()->GetInsertBlock()->getParent();

                    llvm::BasicBlock *errorBlock = llvm::BasicBlock::Create(*getContext(), "error", func);
                    llvm::BasicBlock *continueBlock = llvm::BasicBlock::Create(*getContext(), "continue", func);


                    getBuilder()->CreateCondBr(isGreaterThanZero, continueBlock, errorBlock);


                    getBuilder()->SetInsertPoint(errorBlock);
                    auto errorMessage = getBuilder()->CreateGlobalStringPtr(
                            "Error: Array size must be greater than 0\n");
                    getBuilder()->CreateCall(m_module->getFunction("printf"),
                                             {getBuilder()->CreateGlobalStringPtr("%s"), errorMessage});
                    getBuilder()->CreateCall(m_module->getFunction("exit"),
                                             llvm::ConstantInt::get(llvm::Type::getInt32Ty(*getContext()), -1));
                    getBuilder()->CreateUnreachable();
                    // No need to add more instructions here, as exit terminates the program

                    // Normal execution resumes in the continueBlock
                    getBuilder()->SetInsertPoint(continueBlock);
                    // Allocation happens only if the condition is true, so it's safe here
                    AllocaInst *arrayAlloc = getBuilder()->CreateAlloca(type, arraySizeValue, varDecl->GetName());
                    // Store the array location in the symbol table
                    m_symbols->insert(SymbolTable::value_type(varDecl, arrayAlloc));
                }
            } else {


                // Generate an "alloca" instruction, which goes in entry block of the current function.
                IRBuilder<> allocaBuilder(&m_currentFunction->getEntryBlock(),
                                          m_currentFunction->getEntryBlock().getFirstInsertionPt());
                Value *location = allocaBuilder.CreateAlloca(type, nullptr, varDecl->GetName());

                // Store the variable location in the symbol table.
                m_symbols->insert(SymbolTable::value_type(varDecl, location));

                // optional codegen for initializer
                if (stmt.HasInitExp()) {
                    Value *rvalue = m_codegenExp.Codegen(stmt.GetInitExp());
                    getBuilder()->CreateStore(rvalue, location);
                }
            }
        }

        // Generate code for a return statement.
        void Visit(ReturnStmt &stmt) override {
            Value *result = m_codegenExp.Codegen(stmt.GetExp());
            getBuilder()->CreateRet(result);
        }

        // Generate code for a sequence of statements.
        void Visit(SeqStmt &seq) override {
            for (const StmtPtr &stmt: seq.Get()) {
                Codegen(*stmt);
            }
        }

        // Generate code for an "if" statement.
        void Visit(IfStmt &stmt) override {
            // Generate code for the conditional expression.
            Value *condition = codegenCondExp(stmt.getCondExp());

            // then block
            // optional else block
            // join/merge block
            BasicBlock *thenBlock = BasicBlock::Create(*getContext(), "then", m_currentFunction);
            BasicBlock *elseBlock = stmt.hasElseStmt() ? BasicBlock::Create(*getContext(), "else", m_currentFunction)
                                                       : nullptr;
            BasicBlock *joinBlock = BasicBlock::Create(*getContext(), "join", m_currentFunction);


            getBuilder()->CreateCondBr(condition, thenBlock, elseBlock ? elseBlock : joinBlock);

            // Codegen for then branch
            getBuilder()->SetInsertPoint(thenBlock);
            Codegen(stmt.getThenStmt());


            // create a branch to the join block unless it ends with return
            if (!getBuilder()->GetInsertBlock()->getTerminator())
                getBuilder()->CreateBr(joinBlock);

            // If present, generate code for "else" branch.
            if (stmt.hasElseStmt()) {
                getBuilder()->SetInsertPoint(elseBlock);
                Codegen(stmt.getElseStmt());

                // create a branch to the join block unless it ends with return
                if (!getBuilder()->GetInsertBlock()->getTerminator())
                    getBuilder()->CreateBr(joinBlock);
            }


            getBuilder()->SetInsertPoint(joinBlock);
        }

        // Generate code for a while loop.
        void Visit(WhileStmt &stmt) override {
            // Create a basic block for the start of the loop.
            BasicBlock *loopBlock = BasicBlock::Create(*getContext(), "loop", m_currentFunction);
            getBuilder()->CreateBr(loopBlock);
            getBuilder()->SetInsertPoint(loopBlock);

            // Generate code for the loop condition.
            Value *condition = codegenCondExp(stmt.GetCondExp());

            // Create basic blocks for the loop body and the join point.
            BasicBlock *bodyBlock = BasicBlock::Create(*getContext(), "body", m_currentFunction);
            BasicBlock *joinBlock = BasicBlock::Create(*getContext(), "join", m_currentFunction);

            // Create a conditional branch.
            getBuilder()->CreateCondBr(condition, bodyBlock, joinBlock);

            // Generate code for the loop body, followed by an unconditional branch to the loop head.
            getBuilder()->SetInsertPoint(bodyBlock);
            Codegen(stmt.GetBodyStmt());
            getBuilder()->CreateBr(loopBlock);

            // Set the builder insertion point in the join block.
            getBuilder()->SetInsertPoint(joinBlock);
        }
        void Visit(ForStmt &stmt) override {
            // 1. Codegen for initialization (if any).
            if (stmt.HasInitStmt()) {
                Codegen(stmt.GetInitStmt());
            }

            // 2. Create basic blocks for the loop header, body, update, and exit.
            BasicBlock *headerBlock = BasicBlock::Create(*getContext(), "for.header", m_currentFunction);
            BasicBlock *bodyBlock = BasicBlock::Create(*getContext(), "for.body", m_currentFunction);
            BasicBlock *updateBlock = stmt.HasUpdateStmt() ? BasicBlock::Create(*getContext(), "for.update", m_currentFunction) : nullptr;
            BasicBlock *exitBlock = BasicBlock::Create(*getContext(), "for.exit", m_currentFunction);

            // Jump to the header block.
            getBuilder()->CreateBr(headerBlock);
            getBuilder()->SetInsertPoint(headerBlock);

            // 3. Codegen for the condition (if any).
            Value *condition = stmt.HasCondExp() ? m_codegenExp.Codegen(stmt.GetCondExp()) : GetBool(true);

            // Create conditional branch based on the loop condition.
            getBuilder()->CreateCondBr(condition, bodyBlock, exitBlock);

            // 4. Codegen for the loop body.
            getBuilder()->SetInsertPoint(bodyBlock);
            Codegen(stmt.GetBodyStmt());
            getBuilder()->CreateBr(updateBlock ? updateBlock : headerBlock);

            // 5. Codegen for the update expression (if any).
            if (updateBlock) {
                getBuilder()->SetInsertPoint(updateBlock);
                Codegen(stmt.GetUpdateStmt());
                getBuilder()->CreateBr(headerBlock);
            }

            // Set the builder insertion point to the exit block.
            getBuilder()->SetInsertPoint(exitBlock);
        }



    private:
        SymbolTable *m_symbols;
        FunctionTable *m_functions;
        Function *m_currentFunction;
        CodegenExp m_codegenExp;

        // Generate code for the condition expression in an "if" statement or a while loop.
        Value *codegenCondExp(const Exp &exp) {
            Value *condition = m_codegenExp.Codegen(exp);
            if (exp.getType() == kTypeBool) {
                return condition;

                // Convert the integer conditional expresison to a boolean (i1) using a comparison.
            }else if (exp.getType() == kTypeInt) {
                return getBuilder()->CreateICmpNE(condition, GetInt(0));
            }else
                return getBuilder()->CreateFCmpUNE(condition, llvm::ConstantFP::get(GetFloatType(), 0.0), "tobool");

        }
    };


// Function definition code generator.
    class CodegenFunc : public CodegenBase {
    public:
        CodegenFunc(LLVMContext *context, Module *module, FunctionTable *functions)
                : CodegenBase(context, module, &m_builder), m_builder(*context), m_functions(functions) {
        }

        // Generate code for a function definition.
        void Codegen(const FuncDef *funcDef) {
            // Don't generate code for builtin function declarations.
            if (!funcDef->hasBody())
                return;

            // Convert parameter types to LLVM types.
            const std::vector<VarDeclPtr> &params = funcDef->getParams();
            std::vector<llvm::Type *> paramTypes;
            paramTypes.reserve(params.size());
            for (const VarDeclPtr &param: params) {
                paramTypes.push_back(ConvertType(param->GetType()));
            }

            // Construct LLVM function type and function definition.
            llvm::Type *returnType = ConvertType(funcDef->getReturnType());
            FunctionType *funcType = FunctionType::get(returnType, paramTypes, false /*isVarArg*/ );
            Function *function = Function::Create(funcType, Function::ExternalLinkage, funcDef->getName(), getModule());

            // The main function has external linkage.  Other functions are
            // "internal", which encourages inlining.
            function->setLinkage(funcDef->getName() == "main" ? Function::ExternalLinkage : Function::InternalLinkage);

            // Update the function table.
            m_functions->insert(FunctionTable::value_type(funcDef, function));

            // Construct a symbol table that maps the parameter declarations to the LLVM function parameters.
            SymbolTable symbols;
            size_t i = 0;
            for (Argument &arg: function->args()) {
                symbols.insert(SymbolTable::value_type(params[i].get(), &arg));
                ++i;
            }

            // Create entry block and use it as the builder's insertion point.
            BasicBlock *block = BasicBlock::Create(*getContext(), "entry", function);
            getBuilder()->SetInsertPoint(block);

            // Generate code for the body of the function.
            CodegenStmt codegen(getContext(), getModule(), getBuilder(), &symbols, m_functions, function);
            codegen.Codegen(funcDef->GetBody());

            // Add a return instruction if the user neglected to do so.
            if (!getBuilder()->GetInsertBlock()->getTerminator())
                getBuilder()->CreateRet(GetInt(0));
        }

    private:
        IRBuilder<> m_builder;
        FunctionTable *m_functions;
    };

}

std::unique_ptr<Module> Codegen(LLVMContext *context, const Program &program) {
    // Construct LLVM module.
    std::unique_ptr<Module> module(new Module("module", *context));

    // Table that has function definitions and their llvm equivalents
    FunctionTable functions;

    // Generate code for each function, adding LLVM functions to the module.
    for (const FuncDefPtr &funcDef: program.GetFunctions()) {
        CodegenFunc(context, module.get(), &functions).Codegen(funcDef.get());
    }
    return std::move(module);
}