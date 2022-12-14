//
// Created by 20580 on 2022/8/19.
//

#ifndef COMPILER_FUNC_EXECUTE_ANALYSIS_H
#define COMPILER_FUNC_EXECUTE_ANALYSIS_H

#include "../ast_visitor.h"
#include "symbol_table.h"

class FuncExecuteAnalysis : public AstVisitor{
public:
    void visit(const std::shared_ptr<CompUnitNode>          &node) override;
    void visit(const std::shared_ptr<FuncDef>               &node) override;
    void visit(const std::shared_ptr<CommonFormalParaNode>  &node) override{}
    void visit(const std::shared_ptr<ArrayFormalParaNode>   &node) override{}
    void visit(const std::shared_ptr<VarDef>                &node) override;
    void visit(const std::shared_ptr<ArrayVarDef>           &node) override;
    void visit(const std::shared_ptr<InitializedNode>       &node) override;
    void visit(const std::shared_ptr<CodeBlockNode>         &node) override;
    void visit(const std::shared_ptr<AssignStmtNode>        &node) override;
    void visit(const std::shared_ptr<ExpressionStmtNode>    &node) override;
    void visit(const std::shared_ptr<IfStmtNode>            &node) override;
    void visit(const std::shared_ptr<WhileStmtNode>         &node) override;
    void visit(const std::shared_ptr<ReturnStmtNode>        &node) override;
    void visit(const std::shared_ptr<ContinueStmtNode>      &node) override{}
    void visit(const std::shared_ptr<BreakStmtNode>         &node) override{}
    void visit(const std::shared_ptr<ExpressionNode>        &node) override;
    void visit(const std::shared_ptr<CommonLValueNode>      &node) override;
    void visit(const std::shared_ptr<ArrayRefLValueNode>    &node) override;
    void visit(const std::shared_ptr<IntValueNode>          &node) override{}
    void visit(const std::shared_ptr<FuncCallNode>          &node) override;
    void visit(const std::shared_ptr<FloatValueNode>        &node) override{}
    void visit(const std::shared_ptr<UnaryExpNode>          &node) override;
    void visit(const std::shared_ptr<BinaryOpExprNode>      &node) override;
private:
    /* ???????????? */
    void anaStmt(const SmartNode& node);
    /* ??????????????? */
    void enterScope();
    /* ??????????????? */
    void leaveScope();
    /* ??????CallChain */
    void analysisCallChain();
    void analysisCallChain(const std::shared_ptr<FuncDef>& funcDef, std::set<std::string> callSet);

    std::shared_ptr<FuncDef> currentFuncPtr;                 /* ??????????????????????????? */
    std::shared_ptr<FuncDef> mainFuncPtr;                    /* ??????main??????????????? */
    std::shared_ptr<SymScope> currentScopePtr{nullptr};      /* ?????????????????????????????? */
    std::map<std::string, std::shared_ptr<FuncDef>> funcMap; /* ??????AST????????? */
    int deep{0};                                             /* ?????? */

};



#endif //COMPILER_FUNC_EXECUTE_ANALYSIS_H
