//
// Created by devotes on 22-8-7.
//

#include "pre_ir_analysis.h"

using namespace std;

SmartNode AnalysisScope::getSymbolValue(const std::string &id) {
  auto scope = shared_from_this();
  while(scope != nullptr){
    if(scope->constNodeMap.find(id) != scope->constNodeMap.end())
      return scope->constNodeMap[id];
    scope = scope->parentScope;
  }
  return nullptr;
}

void AnalysisScope::addSymbolValue(const std::string &id, const SmartNode &node) {
  constNodeMap.insert({id, node});
}

// 进入分析作用域
void PreIrGenAnalysis::enterAnalysisScope() {
    auto newScope = make_shared<AnalysisScope>();
    newScope->parentScope = scopePtr;
    scopePtr = newScope;
}

// 离开分析作用域
void PreIrGenAnalysis::leaveAnalysisScope() {
    scopePtr = scopePtr->parentScope;
}

// 顶层进入入口
void PreIrGenAnalysis::visit(const std::shared_ptr<CompUnitNode> &node) {

  for(const auto& elem : node->elems){
    switch (elem->type()) {
      case NodeType::VAR_DEF: visit(dynamic_pointer_cast<VarDef>(elem));
        break;
      case NodeType::ARRAY_VAR_DEF: visit(dynamic_pointer_cast<ArrayVarDef>(elem));
        break;
      case NodeType::FUNC_DEF: visit(dynamic_pointer_cast<FuncDef>(elem));
        break;
      default: break;
    }
  }
}

void PreIrGenAnalysis::visit(const std::shared_ptr<VarDef> &node) {
    if(node->initial != nullptr)
        visit(node->initial);
}


void PreIrGenAnalysis::visit(const std::shared_ptr<ArrayVarDef> &node) {
    if(node->initial != nullptr)
        visit(node->initial);
}

void PreIrGenAnalysis::visit(const std::shared_ptr<FuncDef> &node) {
    visitStmt(node->block);
}

void PreIrGenAnalysis::visit(const std::shared_ptr<ArrayFormalParaNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<CommonFormalParaNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<BreakStmtNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<ReturnStmtNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<ContinueStmtNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<ExpressionStmtNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<IfStmtNode> &node) {
  visitStmt(node->correct_stmt);
  if(node->else_stmt)
    visitStmt(node->else_stmt);
}

void PreIrGenAnalysis::visit(const std::shared_ptr<WhileStmtNode> &node) {
  visitStmt(node->stmt);
}

void PreIrGenAnalysis::visit(const std::shared_ptr<AssignStmtNode> &node) {
  if(node->lhs->type() == COMMON_LVALUE){
    if(node->rhs->type() == BINARY_OP_NODE){
      auto id = dynamic_pointer_cast<CommonLValueNode>(node->lhs)->id;
      transposeAnalysis(id, dynamic_pointer_cast<BinaryOpExprNode>(node->rhs));
    }
  }
}

void PreIrGenAnalysis::visit(const std::shared_ptr<FuncCallNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<BinaryOpExprNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<CommonLValueNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<FloatValueNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<IntValueNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<InitializedNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<UnaryExpNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<ExpressionNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<ArrayRefLValueNode> &node) {}

void PreIrGenAnalysis::visit(const std::shared_ptr<CodeBlockNode> &node) {
  for(const auto& stmt : node->stmts)
    visitStmt(stmt);
}

void PreIrGenAnalysis::visitStmt(const SmartNode &node) {

  switch (node->type()) {
    case NodeType::VAR_DEF:visit(dynamic_pointer_cast<VarDef>(node));
      break;
    case NodeType::ARRAY_VAR_DEF:visit(dynamic_pointer_cast<ArrayVarDef>(node));
      break;
    case NodeType::EXPRESSION_STATEMENT:visit(dynamic_pointer_cast<ExpressionStmtNode>(node));
      break;
    case NodeType::CONTINUE_STATEMENT:visit(dynamic_pointer_cast<ContinueStmtNode>(node));
      break;
    case NodeType::BREAK_STATEMENT:visit(dynamic_pointer_cast<BreakStmtNode>(node));
      break;
    case NodeType::RETURN_STATEMENT:visit(dynamic_pointer_cast<ReturnStmtNode>(node));
      break;
    case NodeType::WHILE_STATEMENT: visit(dynamic_pointer_cast<WhileStmtNode>(node));
      break;
    case NodeType::IF_STATEMENT:visit(dynamic_pointer_cast<IfStmtNode>(node));
      break;
    case NodeType::ASSIGN_STATEMENT:visit(dynamic_pointer_cast<AssignStmtNode>(node));
      break;
    case NodeType::CODE_BLOCK:{
      enterAnalysisScope();
      visit(dynamic_pointer_cast<CodeBlockNode>(node));
      leaveAnalysisScope();
    }
    default: break;
  }

}

void PreIrGenAnalysis::transposeAnalysis(const std::string &id, const std::shared_ptr<BinaryOpExprNode> &binary) {
  auto res = hasAssignValue(id, binary);

  if(res == nullptr)
    return;
  auto parent = res->parent;
  shared_ptr<BinaryOpExprNode> bin = dynamic_pointer_cast<BinaryOpExprNode>(parent);
  if(bin == nullptr)
    return;
  if(bin->oper == op){
    if(bin->lhs == res){
      bin->lhs = bin->rhs;
      bin->rhs = res;
    }
  }
  parent = parent->parent;
  shared_ptr<BinaryOpExprNode> bin1 = dynamic_pointer_cast<BinaryOpExprNode>(parent);
  while(bin1 != nullptr){
      if(bin1->oper == op){
        res->parent = bin1;
        bin1->rhs->parent = bin;
        bin->rhs = bin1->rhs;
        bin1->rhs = res;
        bin = bin1;
      }
      parent = parent->parent;
      bin1 = dynamic_pointer_cast<BinaryOpExprNode>(parent);
  }

}

std::shared_ptr<CommonLValueNode> PreIrGenAnalysis::hasAssignValue(const std::string &id, const SmartExpr& node) {

  switch (node->type()) {
    case NodeType::COMMON_LVALUE:{
      auto lvalue = dynamic_pointer_cast<CommonLValueNode>(node);

      if(id == lvalue->id){
        auto parent = dynamic_pointer_cast<BinaryOpExprNode>(lvalue->parent);
        if(parent != nullptr){
          if(parent->oper == ADD_OP || parent->oper == MUL_OP){
              op = parent->oper;
              return lvalue;
          }
        }
      }
      break;
    }
    case NodeType::BINARY_OP_NODE:{
      auto binary = dynamic_pointer_cast<BinaryOpExprNode>(node);
      auto res1 = hasAssignValue(id, binary->lhs);
      auto res2 = hasAssignValue(id, binary->rhs);
      if(res1 != nullptr)
        return res1;
      if(res2 != nullptr)
        return res2;
      break;
    }
    default:break;
  }
  return nullptr;
}





