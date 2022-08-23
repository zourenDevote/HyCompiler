//
// Created by 20580 on 2022/8/3.
//

#ifndef COMPILER_DAG_AST_BUILDER_H
#define COMPILER_DAG_AST_BUILDER_H

#include "../ast_visitor.h"
#include "../ast/ast.h"
#include <map>
#include <string>
#include <set>

std::string toOper(Operator op);

struct Scope{
    Scope(int number) : scopeNumber(number), parent(nullptr){}

    // 获取Symbol的特定名字
    std::string getSymbolName(const std::string& id);
    // 获取Symbol的类型
    void addSymbol(const std::string& id);

    std::set<std::string> symbolTable;
    Scope* parent;
    int scopeNumber;
};

class DagAstBuilder : public AstVisitor{
public:
    DagAstBuilder() = default;

    void enterScope();
    void leaveScope();

    void visit(const std::shared_ptr<CommonFormalParaNode>& node)  override;
    void visit(const std::shared_ptr<ArrayFormalParaNode>&  node)  override;
    void visit(const std::shared_ptr<ArrayRefLValueNode>&   node)  override;
    void visit(const std::shared_ptr<ExpressionStmtNode>&   node)  override;
    void visit(const std::shared_ptr<CommonLValueNode>&     node)  override;
    void visit(const std::shared_ptr<BinaryOpExprNode>&     node)  override;
    void visit(const std::shared_ptr<ContinueStmtNode>&     node)  override;
    void visit(const std::shared_ptr<InitializedNode>&      node)  override;
    void visit(const std::shared_ptr<ReturnStmtNode>&       node)  override;
    void visit(const std::shared_ptr<AssignStmtNode>&       node)  override;
    void visit(const std::shared_ptr<FloatValueNode>&       node)  override;
    void visit(const std::shared_ptr<ExpressionNode>&       node)  override;
    void visit(const std::shared_ptr<BreakStmtNode>&        node)  override;
    void visit(const std::shared_ptr<CodeBlockNode>&        node)  override;
    void visit(const std::shared_ptr<WhileStmtNode>&        node)  override;
    void visit(const std::shared_ptr<UnaryExpNode>&         node)  override;
    void visit(const std::shared_ptr<IntValueNode>&         node)  override;
    void visit(const std::shared_ptr<CompUnitNode>&         node)  override;
    void visit(const std::shared_ptr<FuncCallNode>&         node)  override;
    void visit(const std::shared_ptr<ArrayVarDef>&          node)  override;
    void visit(const std::shared_ptr<IfStmtNode>&           node)  override;
    void visit(const std::shared_ptr<FuncDef>&              node)  override;
    void visit(const std::shared_ptr<VarDef>&               node)  override;
    void visit(const std::shared_ptr<Node>&                 stmt);

    std::string calcuExpr(SmartExpr expr);

private:
    std::map<std::string, SmartExpr> valueMap;
    Scope* currentScope{nullptr};
    int numberRecord{0};
};



#endif //COMPILER_DAG_AST_BUILDER_H
