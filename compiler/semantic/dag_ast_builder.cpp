//
// Created by 20580 on 2022/8/3.
//

#include "dag_ast_builder.h"

std::string toOper(Operator op){

    switch (op) {
        case ADD_OP:
            return "+";
        case SUB_OP:
            return "-";
        case MUL_OP:
            return "*";
        case DIV_OP:
            return "/";
        case LE:
            return ">=";
        case LT:
            return ">";
        case GE:
            return "<=";
        case GT:
            return "<";
        case EQUAL_OP:
            return "==";
        case NEQUAL_OP:
            return "!=";
        case AND:
            return "&&";
        case OR:
            return "||";
        case NOT:
            return "!";
        case MOD_OP:
            return "%";
    }
    return "";

}

std::string Scope::getSymbolName(const std::string &id) {
    Scope* tmp = this;
    while(tmp != nullptr){
        if(tmp->symbolTable.find(id) != tmp->symbolTable.end()){
            return id + "$" + std::to_string(tmp->scopeNumber);
        }
        tmp = tmp->parent;
    }
    return "";
}

void Scope::addSymbol(const std::string &id) {
    symbolTable.insert(id);
}

void DagAstBuilder::enterScope() {
    auto scope = new Scope(++numberRecord);
    scope->parent = currentScope;
    currentScope = scope;
}

void DagAstBuilder::leaveScope() {
    Scope* scope = currentScope;
    currentScope = currentScope->parent;
    delete scope;
}

std::string DagAstBuilder::calcuExpr(SmartExpr expr) {

    switch (expr->type()) {
        case NodeType::INT_ITER_NODE:
            return std::to_string(std::dynamic_pointer_cast<IntValueNode>(expr)->v);
        case NodeType::FLOAT_ITER_NODE:
            return std::to_string(std::dynamic_pointer_cast<FloatValueNode>(expr)->v);
        case NodeType::FUNC_CALL_NODE:{
            return "";
        }
        case NodeType::BINARY_OP_NODE:{
            auto binary = std::dynamic_pointer_cast<BinaryOpExprNode>(expr);
            std::string lexpr = calcuExpr(binary->lhs);
            std::string rexpr = calcuExpr(binary->rhs);
            if(lexpr == "" || rexpr == "")
                return "";
            return lexpr + toOper(binary->oper) + rexpr;
        }
        case NodeType::COMMON_LVALUE:{
            auto commonValue = std::dynamic_pointer_cast<CommonLValueNode>(expr);
            return currentScope->getSymbolName(commonValue->id);
        }
        case NodeType::ARRAY_REF_LVALUE:{
            auto arrRef = std::dynamic_pointer_cast<ArrayRefLValueNode>(expr);
            std::string arrayValue = currentScope->getSymbolName(arrRef->id);
            for(const auto& exp : arrRef->arrayRef){
                auto res = calcuExpr(exp);
                if(res == "")
                    return "";
                arrayValue.append("[");
                arrayValue.append(res);
                arrayValue.append("]");
            }
            return arrayValue;
        }
        case NodeType::UNART_NODE:{
            auto unary = std::dynamic_pointer_cast<UnaryExpNode>(expr);
            auto res = calcuExpr(unary->node);
            if(res == "")
                return "";
            return toOper(unary->unary_op) + res;
        }
        default:
            break;
    }
    return "";
}

void DagAstBuilder::visit(const std::shared_ptr<ArrayRefLValueNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<InitializedNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<CommonLValueNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<CompUnitNode>& node) {

    enterScope();
    for(const auto& elem : node->elems){
        if(elem->type() == NodeType::FUNC_DEF){
            visit(std::dynamic_pointer_cast<FuncDef>(elem));
        }else if(elem->type() == NodeType::VAR_DEF){
            visit(std::dynamic_pointer_cast<VarDef>(elem));
        }else if(elem->type() == NodeType::ARRAY_VAR_DEF){
            visit(std::dynamic_pointer_cast<ArrayVarDef>(elem));
        }
    }
    leaveScope();

}

void DagAstBuilder::visit(const std::shared_ptr<FuncDef>& node) {
    enterScope();
    visit(std::dynamic_pointer_cast<CodeBlockNode>(node->block));
    leaveScope();
}

void DagAstBuilder::visit(const std::shared_ptr<VarDef>& node) {
    currentScope->addSymbol(node->id);
    if(node->initial)
        visit(node->initial);
}

void DagAstBuilder::visit(const std::shared_ptr<ArrayVarDef>& node) {
    currentScope->addSymbol(node->id);
    if(node->initial)
        visit(node->initial);
}

void DagAstBuilder::visit(const std::shared_ptr<Node>& stmt) {
    switch (stmt->type()) {
        case NodeType::VAR_DEF:
            visit(std::dynamic_pointer_cast<VarDef>(stmt));
            break;
        case NodeType::ARRAY_VAR_DEF:
            visit(std::dynamic_pointer_cast<ArrayVarDef>(stmt));
            break;
        case NodeType::ASSIGN_STATEMENT:
            visit(std::dynamic_pointer_cast<AssignStmtNode>(stmt));
            break;
        case NodeType::IF_STATEMENT:
            visit(std::dynamic_pointer_cast<IfStmtNode>(stmt));
            break;
        case NodeType::WHILE_STATEMENT:
            visit(std::dynamic_pointer_cast<WhileStmtNode>(stmt));
            break;
        case NodeType::EXPRESSION_STATEMENT:
            visit(std::dynamic_pointer_cast<ExpressionStmtNode>(stmt));
            break;
        case NodeType::RETURN_STATEMENT:
            visit(std::dynamic_pointer_cast<ReturnStmtNode>(stmt));
            break;
        case NodeType::BREAK_STATEMENT:
            visit(std::dynamic_pointer_cast<BreakStmtNode>(stmt));
            break;
        case NodeType::CONTINUE_STATEMENT:
            visit(std::dynamic_pointer_cast<ContinueStmtNode>(stmt));
            break;
        case NodeType::CODE_BLOCK:{
            enterScope();
            visit(std::dynamic_pointer_cast<CodeBlockNode>(stmt));
            leaveScope();
        }
        default:
            break;
    }
}

void DagAstBuilder::visit(const std::shared_ptr<CodeBlockNode>& node) {
    for(const auto& stmt : node->stmts){
        visit(stmt);
    }
}

void DagAstBuilder::visit(const std::shared_ptr<WhileStmtNode>& node) {
    visit(node->l_or_exp);

    auto res = calcuExpr(node->l_or_exp);
    if(res != ""){
        if(valueMap.find(res) != valueMap.end()){
            node->l_or_exp = valueMap[res];
        }else
            valueMap.insert({res, node->l_or_exp});
    }

    visit(node->stmt);
}

void DagAstBuilder::visit(const std::shared_ptr<IfStmtNode>& node) {
    visit(node->l_or_expr);
    auto res = calcuExpr(node->l_or_expr);
    if(res != ""){
        if(valueMap.find(res) != valueMap.end()){
            node->l_or_expr = valueMap[res];
        }else
            valueMap.insert({res, node->l_or_expr});
    }
    visit(node->correct_stmt);
    if(node->else_stmt)
        visit(node->else_stmt);
}

void DagAstBuilder::visit(const std::shared_ptr<BreakStmtNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<AssignStmtNode>& node) {

    visit(node->lhs);
    visit(node->rhs);
    auto res1 = calcuExpr(node->lhs);
    if(res1 != ""){
        if(valueMap.find(res1) != valueMap.end()){
            node->lhs = valueMap[res1];
        }
        else
            valueMap.insert({res1, node->lhs});
    }

    auto res2 = calcuExpr(node->rhs);
    if(res2 != ""){
        if(valueMap.find(res2) != valueMap.end()){
            node->rhs = valueMap[res2];
        }
        else
            valueMap.insert({res2, node->rhs});
    }
}

void DagAstBuilder::visit(const std::shared_ptr<ReturnStmtNode>& node) {
    if(node->expr){
        visit(node->expr);
        auto res = calcuExpr(node->expr);
        if(res != ""){
            if(valueMap.find(res) != valueMap.end()){
                node->expr = valueMap[res];
            }else{
                valueMap.insert({res, node->expr});
            }
        }
    }
}

void DagAstBuilder::visit(const std::shared_ptr<FuncCallNode>& node) {
    for(auto& value : node->pparams){
        visit(value);
        auto res = calcuExpr(value);
        if(res != ""){
            if(valueMap.find(res) != valueMap.end()){
                value = valueMap[res];
            }else{
                valueMap.insert({res, value});
            }
        }
    }
}

void DagAstBuilder::visit(const std::shared_ptr<IntValueNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<FloatValueNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<UnaryExpNode>& node) {

    visit(node->node);
    auto res = calcuExpr(node->node);
    if(res != ""){
        if(valueMap.find(res) != valueMap.end()){
            node->node = valueMap[res];
        }else{
            valueMap.insert({res, node->node});
        }
    }
}

void DagAstBuilder::visit(const std::shared_ptr<ExpressionStmtNode>& node) {

    if(node->expr != nullptr){
        visit(node->expr);
        auto res = calcuExpr(node->expr);
        if(res != ""){
            if(valueMap.find(res) != valueMap.end()){
                node->expr = valueMap[res];
            }
            else
                valueMap.insert({res, node->expr});
        }
    }

}

void DagAstBuilder::visit(const std::shared_ptr<ContinueStmtNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<BinaryOpExprNode>& node) {

    visit(node->lhs);
    visit(node->rhs);

    auto res1 = calcuExpr(node->lhs);
    if(res1 != ""){
        if(valueMap.find(res1) != valueMap.end()){
            node->lhs = valueMap[res1];
        }else
            valueMap.insert({res1, node->lhs});
    }

    auto res2 = calcuExpr(node->rhs);
    if(res2 != ""){
        if(valueMap.find(res2) != valueMap.end()){
            node->rhs = valueMap[res2];
        }else
            valueMap.insert({res2, node->rhs});
    }

}

void DagAstBuilder::visit(const std::shared_ptr<ExpressionNode>& node) {

    switch(node->type()){
        case NodeType::INT_ITER_NODE:{
            visit(std::dynamic_pointer_cast<IntValueNode>(node));
            break;
        }
        case NodeType::FLOAT_ITER_NODE:{
            visit(std::dynamic_pointer_cast<FloatValueNode>(node));
            break;
        }
        case NodeType::FUNC_CALL_NODE:{
            visit(std::dynamic_pointer_cast<FuncCallNode>(node));
            break;
        }
        case NodeType::UNART_NODE:{
            visit(std::dynamic_pointer_cast<UnaryExpNode>(node));
            break;
        }
        case NodeType::ARRAY_REF_LVALUE:{
            visit(std::dynamic_pointer_cast<ArrayRefLValueNode>(node));
            break;
        }
        case NodeType::COMMON_LVALUE:{
            visit(std::dynamic_pointer_cast<CommonLValueNode>(node));
            break;
        }
        case NodeType::BINARY_OP_NODE:{
            visit(std::dynamic_pointer_cast<BinaryOpExprNode>(node));
            break;
        }
        default:
            break;
    }
}

void DagAstBuilder::visit(const std::shared_ptr<CommonFormalParaNode>& node) {}

void DagAstBuilder::visit(const std::shared_ptr<ArrayFormalParaNode>& node) {}
