#include "src//Builtins.h"
#include "src/Codegen.h"
#include "src/FuncDef.h"
#include "src/Parser.h"
#include "src/Printer.h"
#include "src/Program.h"
#include "src/TokenStream.h"
#include "src/Typechecker.h"


#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_os_ostream.h>
#include <llvm/Support/raw_ostream.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/TargetSelect.h>

#include <llvm/ExecutionEngine/GenericValue.h>

#include <llvm/MC/TargetRegistry.h>

#include <llvm/LinkAllPasses.h>
#include <llvm/TargetParser/Host.h>

#include <fstream>
#include <iostream>
#include <llvm/Passes/PassBuilder.h>

#ifndef OPT_LEVEL
/// Optimization level, which defaults to -O2.
#define OPT_LEVEL 2
#endif

constexpr int dumpIt = 1;

namespace {
    using namespace llvm;

    void optimize( llvm::Module* module, int optLevel );
    int  readFile( const char* filename, std::vector<char>* buffer );
    void dumpSyntax( const Program& program, const char* srcFilename );
    void dumpIR( llvm::Module& module, const char* srcFilename, const char* what );

    // Parse and typecheck the given source code, adding definitions to the given Program.
    // First for builtins then for user code.
    int parseAndTypecheck( const char* source, Program* program )
    {
        // Construct token stream, which encapsulates the lexer.  \see TokenStream.
        TokenStream tokens( source );

        // Parse the token stream into a program.
        int status = ParseProgram( tokens, program );

        // If the parser succeeded, typecheck the program.
        if( status == 0 )
            status = Typecheck( *program );
        return status;
    }

}


int main( int argc, const char* const* argv ) {
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    InitializeNativeTargetAsmParser();

    // Get command-line arguments.
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return -1;
    }
    const char *filename = argv[1];

    std::vector<char> source;
    int status = readFile(argv[1], &source);
    if (status != 0) {
        std::cerr << "Unable to open input file: " << argv[1] << std::endl;
        return status;
    }
    const char* envVarValue = std::getenv("DUMP");
    if (envVarValue != nullptr) {
        dumpIt = envVarValue;
}

    // Parse and typecheck builtin functions.
    ProgramPtr program(new Program);
    status = parseAndTypecheck(GetBuiltins(), program.get());
    assert(status == 0);

    // Parse and typecheck user source code.
    status = parseAndTypecheck(source.data(), program.get());
    if (status)
        return status;
    dumpSyntax(*program, filename);

    // Generate LLVM IR.
    llvm::LLVMContext context;
    std::unique_ptr<llvm::Module> module(Codegen(&context, *program));
    dumpIR(*module, filename, "initial");



    // Verify the module, which catches malformed instructions and type errors.
    assert(!verifyModule(*module, &llvm::errs()));

    if(module.get() == nullptr){
        std::cout << "module je null exit";
        exit(0);
    }
    optimize(module.get(), OPT_LEVEL);
    dumpIR(*module, filename, "optimized");
    // Create the JIT
    std::string errStr;
    ExecutionEngine *engine = EngineBuilder(std::move(module))
            .setErrorStr(&errStr)
            .setEngineKind(EngineKind::JIT)
            .create();

    if (!engine) {
        errs() << "Failed to construct ExecutionEngine: " << errStr << "\n";
        return 1;
    }
    auto mainFunction = engine->FindFunctionNamed("main");
    if (!mainFunction) {
        errs() << "Function 'main' not found\n";
        return 1;
    }

    // Execute the function
    // If we want to pass the arguments from the command line to the function we can do it here
    //std::vector<GenericValue> args(1);
    //args[0].IntVal = APInt(32, 5);
    engine->getTargetMachine()->setOptLevel(static_cast<CodeGenOpt::Level>(2));
    std::vector<llvm::GenericValue> noargs;
    auto result = engine->runFunction(mainFunction, noargs);
    return 0;
}

namespace {

// Optimize the module using the given optimization level (0 -  3).
    void optimize( Module* module, int optLevel )
    {
        llvm::PassBuilder passBuilder;

        llvm::LoopAnalysisManager loopAnalysisManager;
        llvm::FunctionAnalysisManager functionAnalysisManager;
        llvm::CGSCCAnalysisManager cGSCCAnalysisManager;
        llvm::ModuleAnalysisManager moduleAnalysisManager;

        passBuilder.registerModuleAnalyses(moduleAnalysisManager);
        passBuilder.registerCGSCCAnalyses(cGSCCAnalysisManager);
        passBuilder.registerFunctionAnalyses(functionAnalysisManager);
        passBuilder.registerLoopAnalyses(loopAnalysisManager);

        passBuilder.crossRegisterProxies(loopAnalysisManager, functionAnalysisManager, cGSCCAnalysisManager, moduleAnalysisManager);

        llvm::ModulePassManager modulePassManager;


        llvm::OptimizationLevel level;
        switch (optLevel) {
            case 0: level = llvm::OptimizationLevel::O0; break;
            case 1: level = llvm::OptimizationLevel::O1; break;
            case 2: level = llvm::OptimizationLevel::O2; break;
            case 3: level = llvm::OptimizationLevel::O3; break;
            default: level = llvm::OptimizationLevel::O2; // Default to O2 if an invalid level is provided
        }

        modulePassManager = passBuilder.buildPerModuleDefaultPipeline(level);

        // Run the optimizations on the module.
        modulePassManager.run(*module, moduleAnalysisManager);
    }

// Read file into the given buffer.  Returns zero for success.
    int readFile( const char* filename, std::vector<char>* buffer )
    {
        // Open the stream at the end, get file size, and allocate data.
        std::ifstream in( filename, std::ifstream::ate | std::ifstream::binary );
        if( in.fail() )
            return -1;
        size_t length = static_cast<size_t>( in.tellg() );
        buffer->resize( length + 1 );

        // Rewind and read entire file
        in.clear();  // clear EOF
        in.seekg( 0, std::ios::beg );
        in.read( buffer->data(), length );

        (*buffer)[length] = '\0';
        return 0;
    }

    void dumpSyntax( const Program& program, const char* srcFilename )
    {
        if ( dumpIt == 0)
            return;
        std::string   filename( std::string( srcFilename ) + ".syn" );
        std::ofstream out( filename );
        out << program << std::endl;
    }
    void dumpIR( llvm::Module& module, const char* srcFilename, const char* what )
    {
        if ( dumpIt == 0)
            return;
        std::string   filename( std::string( srcFilename ) + "." + what + ".ll" );
        std::ofstream stream( filename );
        llvm::raw_os_ostream out( stream );
        out << module;
    }
}