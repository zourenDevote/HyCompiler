/**
 * Author:  Devotes
 * Date:    2022/5/28
 *
 * FixLog:
 *
 */

#ifndef COMPILER_AST_VISITOR_H
#define COMPILER_AST_VISITOR_H

#include "ast/ast.h"
#include <memory>

class AstVisitor {
public:
    virtual void visit(const std::shared_ptr<CompUnitNode>&         node){}
    virtual void visit(const std::shared_ptr<VarDef>&               node){}
    virtual void visit(const std::shared_ptr<ArrayVarDef>&          node){}
    virtual void visit(const std::shared_ptr<InitializedNode>&      node){}
    virtual void visit(const std::shared_ptr<FuncDef>&              node){}
    virtual void visit(const std::shared_ptr<CommonFormalParaNode>& node){}
    virtual void visit(const std::shared_ptr<ArrayFormalParaNode>&  node){}
    virtual void visit(const std::shared_ptr<CodeBlockNode>&        node){}
    virtual void visit(const std::shared_ptr<IfStmtNode>&           node){}
    virtual void visit(const std::shared_ptr<WhileStmtNode>&        node){}
    virtual void visit(const std::shared_ptr<BreakStmtNode>&        node){}
    virtual void visit(const std::shared_ptr<ContinueStmtNode>&     node){}
    virtual void visit(const std::shared_ptr<ReturnStmtNode>&       node){}
    virtual void visit(const std::shared_ptr<ExpressionStmtNode>&   node){}
    virtual void visit(const std::shared_ptr<AssignStmtNode>&       node){}
    virtual void visit(const std::shared_ptr<ExpressionNode>&       node){}
    virtual void visit(const std::shared_ptr<CommonLValueNode>&     node){}
    virtual void visit(const std::shared_ptr<ArrayRefLValueNode>&   node){}
    virtual void visit(const std::shared_ptr<IntValueNode>&         node){}
    virtual void visit(const std::shared_ptr<FloatValueNode>&       node){}
    virtual void visit(const std::shared_ptr<FuncCallNode>&         node){}
    virtual void visit(const std::shared_ptr<UnaryExpNode>&         node){}
    virtual void visit(const std::shared_ptr<BinaryOpExprNode>&     node){}

private:
};


#endif //COMPILER_AST_VISITOR_H
