//
// Created by 20580 on 2022/8/19.
//

#include "func_execute_analysis.h"
#include <set>

using namespace std;

/* 构建符号 */
inline std::shared_ptr<Symbol> makeSymbol(const std::string& id, bool isConst, bool isFloat, bool isArray){
    std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>();
    symbol->id = id;
    symbol->isConst = isConst;
    symbol->isAref = 0;
    symbol->isArray = isArray;
    symbol->type = isFloat;
    return symbol;
}

/* 影响函数调用结果的标准库方法 */
static set<string> stdFuncSet = {
        "getint", "getfloat", "getch", "getarray", "getfarray",
        "putint", "putch", "putfloat", "putarray", "putfarray",
        "putf", "starttime", "stoptime"
};

static set<string> stdImpactFuncSet = {
        "getint", "getfloat", "getch", "getarray", "getfarray",
        "starttime", "stoptime"
};

/* 进入作用域 */
void FuncExecuteAnalysis::enterScope() {
    auto oldScopePtr = currentScopePtr;
    currentScopePtr = make_shared<SymScope>();
    currentScopePtr->setScopeDeep(deep);
    deep++;
    currentScopePtr->parent = oldScopePtr;
}

/* 离开作用域 */
void FuncExecuteAnalysis::leaveScope() {
    currentScopePtr = currentScopePtr->parent;
    deep--;
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<CompUnitNode> &node) {
    enterScope();
    for(const auto& elem : node->elems){
        switch (elem->type()) {
            case FUNC_DEF:{
                visit(dynamic_pointer_cast<FuncDef>(elem));
                break;
            }
            case VAR_DEF:{
                visit(dynamic_pointer_cast<VarDef>(elem));
                break;
            }
            case ARRAY_VAR_DEF:{
                visit(dynamic_pointer_cast<ArrayVarDef>(elem));
                break;
            }
            default:
                break;
        }
    }
    leaveScope();

    /* 执行影响分析 */
    analysisCallChain();

    /* 清理掉这些信息，节省空间 */
    for(const auto& funcDef : funcMap){
        funcDef.second->funcChains.clear();
    }
    funcMap.clear();
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<FuncDef> &node) {

    currentFuncPtr = node;
    if(node->func_name == "main"){
        mainFuncPtr = node;
    }
    funcMap.insert({node->func_name, node});

    /* 扫描参数 */
    enterScope();
    for(const auto& para : node->parameters){
        if(para->type() == COMMON_LVALUE){
            currentScopePtr->addSymbol(makeSymbol(para->name, false, para->ty == FLOAT, false));
        }else{
            currentScopePtr->addSymbol(makeSymbol(para->name, false, para->ty == FLOAT, true));
        }
    }

    visit(dynamic_pointer_cast<CodeBlockNode>(node->block));
    leaveScope();
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<InitializedNode> &node) {
    if(node->expr)
        visit(node->expr);
    for(const auto& initial : node->inits)
        visit(initial);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<VarDef> &node) {
    currentScopePtr->addSymbol(makeSymbol(node->id, node->is_const, node->var_type == FLOAT, false));
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<ArrayVarDef> &node) {
    currentScopePtr->addSymbol(makeSymbol(node->id, node->is_const, node->var_type == FLOAT, true));
    for(const auto& index : node->arrayDimVec){
        visit(index);
    }
    if(node->initial)
        visit(node->initial);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<AssignStmtNode> &node) {
    visit(node->lhs);
    visit(node->rhs);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<ExpressionStmtNode> &node) {
    if(node->expr)
        visit(node->expr);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<IfStmtNode> &node) {
    visit(node->l_or_expr);
    anaStmt(node->correct_stmt);
    if(node->else_stmt)
        anaStmt(node->else_stmt);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<WhileStmtNode> &node) {
    visit(node->l_or_exp);
    anaStmt(node->stmt);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<ReturnStmtNode> &node) {
    if(node->expr)
        visit(node->expr);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<ExpressionNode> &node) {
    switch (node->type()) {
        case COMMON_LVALUE:{
            visit(dynamic_pointer_cast<CommonLValueNode>(node));
            break;
        }
        case ARRAY_REF_LVALUE:{
            visit(dynamic_pointer_cast<ArrayRefLValueNode>(node));
            break;
        }
        case FUNC_CALL_NODE:{
            visit(dynamic_pointer_cast<FuncCallNode>(node));
            break;
        }
        case BINARY_OP_NODE:{
            visit(dynamic_pointer_cast<BinaryOpExprNode>(node));
            break;
        }
        case UNART_NODE:{
            visit(dynamic_pointer_cast<UnaryExpNode>(node));
            break;
        }
        default:
            break;
    }
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<CodeBlockNode> &node) {
    for(const auto& stmt : node->stmts){
        anaStmt(stmt);
    }
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<BinaryOpExprNode> &node) {
    visit(node->lhs);
    visit(node->rhs);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<FuncCallNode> &node) {
    if(stdFuncSet.find(node->func_name) == stdFuncSet.end()){
        /* 添加到调用链 */
        currentFuncPtr->funcChains.insert(funcMap[node->func_name]);
    }else if(stdImpactFuncSet.find(node->func_name) != stdImpactFuncSet.end()){
        currentFuncPtr->hasCallImpactStdFunc = true;
    }
    for(const auto& para : node->pparams)
        visit(para);
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<ArrayRefLValueNode> &node) {
    auto symbol = currentScopePtr->getSymbol(node->id);
    if(symbol->scodep == 0 && currentFuncPtr){
        if(!symbol->isConst){
            currentFuncPtr->hasUseGlobalVar = true;
        }
    }
    for(const auto& index : node->arrayRef){
        visit(index);
    }
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<CommonLValueNode> &node) {
    auto symbol = currentScopePtr->getSymbol(node->id);
    if(symbol->scodep == 0 && currentFuncPtr){
        if(!symbol->isConst){
            currentFuncPtr->hasUseGlobalVar = true;
        }
    }
}

void FuncExecuteAnalysis::visit(const std::shared_ptr<UnaryExpNode> &node) {
    visit(node->node);
}

void FuncExecuteAnalysis::anaStmt(const SmartNode &node) {

    switch (node->type()) {
        case VAR_DEF:{
            visit(dynamic_pointer_cast<VarDef>(node));
            break;
        }
        case ARRAY_VAR_DEF:{
            visit(dynamic_pointer_cast<ArrayVarDef>(node));
            break;
        }
        case ASSIGN_STATEMENT:{
            visit(dynamic_pointer_cast<AssignStmtNode>(node));
            break;
        }
        case EXPRESSION_STATEMENT:{
            visit(dynamic_pointer_cast<ExpressionStmtNode>(node));
            break;
        }
        case IF_STATEMENT:{
            visit(dynamic_pointer_cast<IfStmtNode>(node));
            break;
        }
        case WHILE_STATEMENT:{
            visit(dynamic_pointer_cast<WhileStmtNode>(node));
            break;
        }
        case RETURN_STATEMENT:{
            visit(dynamic_pointer_cast<ReturnStmtNode>(node));
            break;
        }
        case CODE_BLOCK:{
            enterScope();
            visit(dynamic_pointer_cast<CodeBlockNode>(node));
            leaveScope();
            break;
        }
        default:
            break;
    }
}

void FuncExecuteAnalysis::analysisCallChain() {
    for(const auto& funcDef : mainFuncPtr->funcChains){
        analysisCallChain(funcDef, {});
    }
}

/**
 * Overview: 分析这个函数是否只受传入参数影响
 *      1. 函数内部调用了全局变量，表明函数的执行结果受外部影响
 *          有两种情况，1. 通过传参的形式传入了全局数组变量
 *                    2. 直接在函数内部使用了全局变量
 *      2. 函数调用了一些交互的标准库函数，比如getint这些
 *      3. 函数中调用的子函数受外部影响，从而导致当前函数受外部影响
 */
void FuncExecuteAnalysis::analysisCallChain(const std::shared_ptr<FuncDef> &funcDef, std::set<std::string> callSet) {
    bool onlyImpactWithPara = true;

    callSet.insert(funcDef->func_name);

    /* 是否有数组参数传递进来 */
    if(funcDef->hasArrayPara)
        onlyImpactWithPara = false;
    if(funcDef->hasCallImpactStdFunc)
        onlyImpactWithPara = false;
    if(funcDef->hasUseGlobalVar)
        onlyImpactWithPara = false;

    for(const auto& childFunc : funcDef->funcChains){
        if(callSet.find(childFunc->func_name) == callSet.end()){
            analysisCallChain(childFunc, callSet);
            if(!childFunc->resultOnlyImpactByPara){
                onlyImpactWithPara = false;
            }
        }
    }

    funcDef->resultOnlyImpactByPara = onlyImpactWithPara;
}

