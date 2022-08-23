//
// Created by devotes on 22-8-7.
//

#ifndef COMPILER_COMPILER_SEMANTIC_PRE_IR_ANALYSIS_H_
#define COMPILER_COMPILER_SEMANTIC_PRE_IR_ANALYSIS_H_

#include "../ast_visitor.h"
#include <map>
#include <string>

 struct AnalysisScope : public std::enable_shared_from_this<AnalysisScope>{
  AnalysisScope() = default;

  SmartNode getSymbolValue(const std::string& id);
  void addSymbolValue(const std::string& id, const SmartNode& node);

  std::map<std::string, SmartNode> constNodeMap;
  std::shared_ptr<AnalysisScope> parentScope{nullptr};
};

class PreIrGenAnalysis : public AstVisitor{
 public:
    PreIrGenAnalysis() = default;

    void visit(const std::shared_ptr<CompUnitNode> &node) override;
    void visit(const std::shared_ptr<ArrayFormalParaNode> &node) override;
    void visit(const std::shared_ptr<ArrayRefLValueNode> &node) override;
    void visit(const std::shared_ptr<BinaryOpExprNode> &node) override;
    void visit(const std::shared_ptr<AssignStmtNode> &node) override;
    void visit(const std::shared_ptr<VarDef> &node) override;
    void visit(const std::shared_ptr<ArrayVarDef> &node) override;
    void visit(const std::shared_ptr<BreakStmtNode> &node) override;
    void visit(const std::shared_ptr<CodeBlockNode> &node) override;
    void visit(const std::shared_ptr<CommonFormalParaNode> &node) override;
    void visit(const std::shared_ptr<CommonLValueNode> &node) override;
    void visit(const std::shared_ptr<ContinueStmtNode> &node) override;
    void visit(const std::shared_ptr<ExpressionNode> &node) override;
    void visit(const std::shared_ptr<ExpressionStmtNode> &node) override;
    void visit(const std::shared_ptr<FloatValueNode> &node) override;
    void visit(const std::shared_ptr<FuncCallNode> &node) override;
    void visit(const std::shared_ptr<FuncDef> &node) override;
    void visit(const std::shared_ptr<IfStmtNode> &node) override;
    void visit(const std::shared_ptr<InitializedNode> &node) override;
    void visit(const std::shared_ptr<IntValueNode> &node) override;
    void visit(const std::shared_ptr<ReturnStmtNode> &node) override;
    void visit(const std::shared_ptr<UnaryExpNode> &node) override;
    void visit(const std::shared_ptr<WhileStmtNode> &node) override;
    void visitStmt(const SmartNode& node);

 private:
    // 进入分析作用域
    void enterAnalysisScope();
    // 离开分析作用域
    void leaveAnalysisScope();
    // 移项分析
    void transposeAnalysis(const std::string& id, const std::shared_ptr<BinaryOpExprNode>& binary);

    std::shared_ptr<CommonLValueNode> hasAssignValue(const std::string& id, const SmartExpr& node);

    // 作用域指针
    std::shared_ptr<AnalysisScope> scopePtr{nullptr};

    // Opertype
    Operator op;
};

#endif //COMPILER_COMPILER_SEMANTIC_PRE_IR_ANALYSIS_H_

