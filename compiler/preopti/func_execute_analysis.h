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
    /* 处理语句 */
    void anaStmt(const SmartNode& node);
    /* 进入作用域 */
    void enterScope();
    /* 离开作用域 */
    void leaveScope();
    /* 分析CallChain */
    void analysisCallChain();
    void analysisCallChain(const std::shared_ptr<FuncDef>& funcDef, std::set<std::string> callSet);

    std::shared_ptr<FuncDef> currentFuncPtr;                 /* 指向当前函数的指针 */
    std::shared_ptr<FuncDef> mainFuncPtr;                    /* 指向main函数的指针 */
    std::shared_ptr<SymScope> currentScopePtr{nullptr};      /* 指向当前作用域的指针 */
    std::map<std::string, std::shared_ptr<FuncDef>> funcMap; /* 函数AST散列表 */
    int deep{0};                                             /* 深度 */

};



#endif //COMPILER_FUNC_EXECUTE_ANALYSIS_H
