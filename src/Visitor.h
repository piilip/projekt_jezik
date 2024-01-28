#pragma once

#include "Nodes.h"


class ExpVisitor
{
  public:
    virtual void* Visit( BoolExp& exp ) = 0;
    virtual void* Visit( IntExp& exp )  = 0;
    virtual void* Visit( VarExp& exp )  = 0;
    virtual void* Visit( CallExp& exp ) = 0;
    virtual void* Visit( FloatExp& exp ) = 0;
    virtual void* Visit( ArrayAccessExp& exp ) = 0;
};


/// Statement visitor base class.
class StmtVisitor
{
  public:
    virtual void Visit( CallStmt& exp )   = 0;
    virtual void Visit( AssignStmt& exp ) = 0;
    virtual void Visit( DeclStmt& exp )   = 0;
    virtual void Visit( ReturnStmt& exp ) = 0;
    virtual void Visit( SeqStmt& exp )    = 0;
    virtual void Visit( IfStmt& exp )     = 0;
    virtual void Visit( WhileStmt& exp )  = 0;
    virtual void Visit( ArrayAssignStmt& exp )  = 0;
    virtual void Visit( ForStmt& exp )  = 0;


};


