//
// Created by 20580 on 2022/8/12.
//

#include "func_inline.h"
#include <cassert>
#include <algorithm>

const std::set<std::string> FuncInline::stdFuncSet = {
      "getint", "getfloat", "getch", "getarray", "getfarray",
      "putint", "putch", "putfloat", "putarray", "putfarray",
      "putf", "starttime", "stoptime"
};

static std::map<std::string, Type> stdFuncRetTyMap{
        {"getint", INT},
        {"getfloat", FLOAT},
        {"getch", INT},
        {"getarray", INT},
        {"getfarray", INT},
        {"putint", VOID},
        {"putch", VOID},
        {"putfloat", VOID},
        {"putarray", VOID},
        {"putfarray", VOID},
        {"putf", VOID},
        {"starttime", VOID},
        {"stoptime", VOID}
};

std::shared_ptr<Symbol> makeSymbol(const std::string& id, bool isConst, bool isFloat, bool isArray){
    std::shared_ptr<Symbol> symbol = std::make_shared<Symbol>();
    symbol->id = id;
    symbol->isConst = isConst;
    symbol->isAref = 0;
    symbol->isArray = isArray;
    symbol->type = isFloat;
    return symbol;
}

Type FuncInline::getType(const SmartExpr &expr) {

    switch (expr->type()) {
        case BINARY_OP_NODE:{
            auto binary = std::dynamic_pointer_cast<BinaryOpExprNode>(expr);
            Type ty1 = getType(binary->lhs);
            Type ty2 = getType(binary->rhs);
            if(ty1 == FLOAT || ty2 == FLOAT) return FLOAT;
            return INT;
        }
        case UNART_NODE:{
            auto unary = std::dynamic_pointer_cast<UnaryExpNode>(expr);
            return getType(unary->node);
        }
        case COMMON_LVALUE:{
            auto comLValue = std::dynamic_pointer_cast<CommonLValueNode>(expr);
            if(curScopePtr->getSymbol(comLValue->id)->type == 1)
                return FLOAT;
            return INT;
        }
        case ARRAY_REF_LVALUE:{
            auto arrRef = std::dynamic_pointer_cast<ArrayRefLValueNode>(expr);
            if(curScopePtr->getSymbol(arrRef->id)->type == 1)
                return FLOAT;
            return INT;
        }
        case FUNC_CALL_NODE:{
            auto funcCall = std::dynamic_pointer_cast<FuncCallNode>(expr);
            if(stdFuncRetTyMap.find(funcCall->func_name) != stdFuncRetTyMap.end()){
                return stdFuncRetTyMap[funcCall->func_name];
            }else{
                return funcDefMap[funcCall->func_name]->ret_type;
            }
        }
        case INT_ITER_NODE: return INT;
        case FLOAT_ITER_NODE: return FLOAT;
        default:
            assert(false && "error expr type!");
    }
    
}

void FuncInline::enterScope() {
    auto scope = curScopePtr;
    curScopePtr = std::make_shared<SymScope>();
    curScopePtr->setScopeDeep(deep++);
    curScopePtr->parent = scope;
}

void FuncInline::leaveScope() {
    curScopePtr = curScopePtr->parent;
    deep--;
}

void FuncInline::visit(const std::shared_ptr<CompUnitNode> &node) {

    /* 分析一遍 */
    enterScope();
    for(const auto& elem : node->elems){
        switch (elem->type()) {
            case VAR_DEF:{
                visit(std::dynamic_pointer_cast<VarDef>(elem));
                break;
            }
            case ARRAY_VAR_DEF:{
                visit(std::dynamic_pointer_cast<ArrayVarDef>(elem));
                break;
            }
            case FUNC_DEF:{
                visit(std::dynamic_pointer_cast<FuncDef>(elem));
                break;
            }
            default:
                break;
        }
    }
    /* 判断函数的语句量级 */
    leaveScope();

    funcInline();

    for(const auto& funcDef : funcDefMap){
        funcDef.second->callChains.clear();
    }
    funcDefMap.clear();

    /* 删除已经内联了的函数 */
    std::vector<SmartNode> newElems;
    for(const auto& funcDef : node->elems){
        if(funcCanDelete.find(funcDef) == funcCanDelete.end())
            newElems.push_back(funcDef);
    }
    node->elems = newElems;
    funcCanDelete.clear();


}

void FuncInline::visit(const std::shared_ptr<FuncDef> &node) {

    /* 设置当前的函数结点指针 */
    currentFunc = node;
    if(node->func_name == "main") mainFunc = node;

    /* 检查函数是否有返回值 */
    if(node->ret_type == VOID)
        node->hasRet = false;

    /* 保存函数结点的记录 */
    funcDefMap.insert({node->func_name, node});
    enterScope();
    /* 将函数参数添加到符号表 */
    for(const auto& para : node->parameters){
        if(para->type() == COMMON_FORMAL_PARA){
            auto comPara = std::dynamic_pointer_cast<CommonFormalParaNode>(para);
            curScopePtr->addSymbol(makeSymbol(comPara->name, false, comPara->ty==FLOAT, false));
        }else if(para->type() == ARRAY_FORMAL_PARA){
            auto comPara = std::dynamic_pointer_cast<ArrayFormalParaNode>(para);
            node->hasArrayPara = true;
            curScopePtr->addSymbol(makeSymbol(comPara->name, false, comPara->ty==FLOAT, false));
        }
    }
    /* 判断函数的语句量级 */
    auto block = std::dynamic_pointer_cast<CodeBlockNode>(node->block);
    if(block->stmts.size() == 1){
        NodeType type = block->stmts[0]->type();
        switch (type) {
            case CODE_BLOCK:
            case IF_STATEMENT:
            case WHILE_STATEMENT:
                break;
            default:
                node->onlyHasSimpleStmt = true;
                break;
        }
    }

    /* 分析函数下面的语句 */
    visit(block);
    leaveScope();
}

void FuncInline::visit(const std::shared_ptr<FuncCallNode> &node) {

    /* 构建函数的调用链 */
    if(stdFuncSet.find(node->func_name) == stdFuncSet.end()){
        assert(funcDefMap.find(node->func_name) != funcDefMap.end());
        for(const auto& para : node->pparams){
            node->realTypes.push_back(getType(para));
        }
        currentFunc->callChains.insert(currentFunc->callChains.begin(), {funcDefMap[node->func_name], node});
    }else{
        currentFunc->hasStdFuncCallIn = true;
    }
    for(const auto& para : node->pparams)
        visit(para);
}

void FuncInline::visit(const std::shared_ptr<VarDef> &node) {
    curScopePtr->addSymbol(makeSymbol(node->id, node->is_const, node->var_type == FLOAT, false));
    if(node->initial)
        visit(node->initial);
}

void FuncInline::visit(const std::shared_ptr<ArrayVarDef> &node) {
    curScopePtr->addSymbol(makeSymbol(node->id, node->is_const, node->var_type == FLOAT,true));
    if(node->initial)
        visit(node->initial);
}

void FuncInline::visit(const std::shared_ptr<CodeBlockNode> &node) {
    for(const auto& stmt : node->stmts)
        gotoVisitStmt(stmt);
}

void FuncInline::visit(const std::shared_ptr<ReturnStmtNode> &node) {
    if(node->expr)
        visit(node->expr);
}

void FuncInline::visit(const std::shared_ptr<ArrayRefLValueNode> &node) {
    for(const auto& ref : node->arrayRef)
        visit(ref);
}

void FuncInline::visit(const std::shared_ptr<CommonLValueNode> &node) {}

void FuncInline::visit(const std::shared_ptr<AssignStmtNode> &node) {
    
    std::shared_ptr<Symbol> symbol;
    if(node->lhs->type() == COMMON_LVALUE){
        auto cmlvalue = std::dynamic_pointer_cast<CommonLValueNode>(node->lhs);
        symbol = curScopePtr->getSymbol(cmlvalue->id);
    }else{
        auto arrRef = std::dynamic_pointer_cast<ArrayRefLValueNode>(node->lhs);
        symbol = curScopePtr->getSymbol(arrRef->id);
    }
    assert(symbol != nullptr && "symbol not found");
    if(symbol->scodep == 1)
        currentFunc->hasParaAssign = true;
    
    visit(node->rhs);
    visit(node->lhs);
}

void FuncInline::visit(const std::shared_ptr<IfStmtNode> &node) {
    visit(node->l_or_expr);
    gotoVisitStmt(node->correct_stmt);
    if(node->else_stmt)
        gotoVisitStmt(node->else_stmt);
}

void FuncInline::visit(const std::shared_ptr<WhileStmtNode> &node) {
    visit(node->l_or_exp);
    gotoVisitStmt(node->stmt);
}

void FuncInline::visit(const std::shared_ptr<ExpressionStmtNode> &node) {
    if(node->expr)
        visit(node->expr);
}

void FuncInline::visit(const std::shared_ptr<UnaryExpNode> &node) {
    visit(node->node);
}

void FuncInline::visit(const std::shared_ptr<InitializedNode> &node) {
    if(node->expr)
        visit(node->expr);
    for(const auto& init : node->inits)
        visit(init);
}

void FuncInline::visit(const std::shared_ptr<BinaryOpExprNode> &node) {
    visit(node->lhs);
    visit(node->rhs);
}

void FuncInline::visit(const std::shared_ptr<ExpressionNode> &node) {
    switch (node->type()) {
        case FUNC_CALL_NODE:{
            visit(std::dynamic_pointer_cast<FuncCallNode>(node));
            break;
        }
        case BINARY_OP_NODE:{
            visit(std::dynamic_pointer_cast<BinaryOpExprNode>(node));
            break;
        }
        case UNART_NODE:{
            visit(std::dynamic_pointer_cast<UnaryExpNode>(node));
            break;
        }
        case COMMON_LVALUE:{
            visit(std::dynamic_pointer_cast<CommonLValueNode>(node));
            break;
        }
        case ARRAY_REF_LVALUE:{
            visit(std::dynamic_pointer_cast<ArrayRefLValueNode>(node));
            break;
        }
        default:
            break;
    }
}

void FuncInline::gotoVisitStmt(const SmartNode &node) {
    switch (node->type()) {
        case VAR_DEF:{
            visit(std::dynamic_pointer_cast<VarDef>(node));
            break;
        }
        case ARRAY_VAR_DEF:{
            visit(std::dynamic_pointer_cast<ArrayVarDef>(node));
            break;
        }
        case IF_STATEMENT:{
            visit(std::dynamic_pointer_cast<IfStmtNode>(node));
            break;
        }
        case WHILE_STATEMENT:{
            visit(std::dynamic_pointer_cast<WhileStmtNode>(node));
            break;
        }
        case CODE_BLOCK:{
            enterScope();
            visit(std::dynamic_pointer_cast<CodeBlockNode>(node));
            leaveScope();
            break;
        }
        case RETURN_STATEMENT:{
            visit(std::dynamic_pointer_cast<ReturnStmtNode>(node));
            break;
        }
        case EXPRESSION_STATEMENT:{
            visit(std::dynamic_pointer_cast<ExpressionStmtNode>(node));
            break;
        }
        case ASSIGN_STATEMENT:{
            visit(std::dynamic_pointer_cast<AssignStmtNode>(node));
            break;
        }
        default:
            break;
    }
}

void FuncInline::funcInline() {
    for(const auto& callChain : mainFunc->callChains){
        funcInline(callChain.funcDef, callChain.funcCall, {});
    }
}

bool FuncInline::funcInline(FuncInline::FuncDefPtr defLoc, FuncInline::FuncCallPtr callLoc, FuncInline::CallSet cset) {
    bool canInline = true;
    bool res;

    if(cset.find(defLoc->func_name) != cset.end()){
        canInline = false;
    }else{
        cset.insert(defLoc->func_name);
    }

    /* 根据前面的分析结果，判断函数是否可以内联 */
    if(defLoc->hasParaAssign)
        canInline = false;
    if(defLoc->hasArrayPara)
        canInline = false;
    if(!defLoc->onlyHasSimpleStmt)
        canInline = false;
    if(!defLoc->hasRet)
        canInline = false;
    if(defLoc->hasStdFuncCallIn)
        canInline = false;

    /* 深度优先遍历调用链，执行内联操作 */
    for(const auto& callChain : defLoc->callChains){
        if(cset.find(callChain.funcDef->func_name) != cset.end()){
            canInline = false;
        }else{
            res = funcInline(callChain.funcDef, callChain.funcCall, cset);
            if(canInline)
                canInline = res;
        }
    }

   if(canInline){
        funcCanDelete.insert(defLoc);
        executeInline(defLoc, callLoc);
    }

    return canInline;
}

void FuncInline::executeInline(FuncInline::FuncDefPtr defLoc, FuncInline::FuncCallPtr callLoc) {

    // 构建内联映射
    std::map<std::string, SmartExpr> p2pMap;
    int i = 0;
    std::string paraName;
    Type ty;
    for(const auto& para : callLoc->pparams){
        paraName = std::dynamic_pointer_cast<CommonFormalParaNode>(defLoc->parameters[i])->name;
        ty = std::dynamic_pointer_cast<CommonFormalParaNode>(defLoc->parameters[i])->ty;
        if(ty == INT && callLoc->realTypes[i] == FLOAT){
            /* set must convert */
            setMustConvertToExpr(para);
        }
        p2pMap.insert({paraName, para});
        i++;
    }

    // 构建内联语句
    auto Expr = std::dynamic_pointer_cast<ReturnStmtNode>(std::dynamic_pointer_cast<CodeBlockNode>(defLoc->block)->stmts[0])->expr->deepCopy();

    // 扫描expr, 替换其中的Value
    scanExprToReplacePara(std::dynamic_pointer_cast<ExpressionNode>(Expr), p2pMap);

    // 使用expr, 替换当前的调用位置
    exprReplaceFuncCall(std::dynamic_pointer_cast<ExpressionNode>(Expr), callLoc);
}

void FuncInline::scanExprToReplacePara(const SmartExpr &node, std::map<std::string, SmartExpr> &p2pMap) {

    switch (node->type()) {
        case BINARY_OP_NODE:{
            auto binary = std::dynamic_pointer_cast<BinaryOpExprNode>(node);
            if(binary->lhs->type() == COMMON_LVALUE){
                std::string id = std::dynamic_pointer_cast<CommonLValueNode>(binary->lhs)->id;
                if(p2pMap.find(id) != p2pMap.end()){
                    binary->setLhs(std::dynamic_pointer_cast<ExpressionNode>(p2pMap[id]->deepCopy()));
                }
            }else{
                scanExprToReplacePara(binary->lhs, p2pMap);
            }
            if(binary->rhs->type() == COMMON_LVALUE){
                std::string id = std::dynamic_pointer_cast<CommonLValueNode>(binary->rhs)->id;
                if(p2pMap.find(id) != p2pMap.end()){
                    binary->setRhs(p2pMap[id]);
                }
            }else{
                scanExprToReplacePara(binary->rhs, p2pMap);
            }
            break;
        }
        case ARRAY_REF_LVALUE:{
            auto arrayRef = std::dynamic_pointer_cast<ArrayRefLValueNode>(node);
            for(auto& index : arrayRef->arrayRef){
                if(index->type() == COMMON_LVALUE){
                    std::string id = std::dynamic_pointer_cast<CommonLValueNode>(index)->id;
                    if(p2pMap.find(id) != p2pMap.end()){
                        index = std::dynamic_pointer_cast<ExpressionNode>(p2pMap[id]->deepCopy());
                        index->parent = arrayRef;
                    }
                }else{
                    scanExprToReplacePara(index, p2pMap);
                }
            }
            break;
        }
        case UNART_NODE:{
            auto unary = std::dynamic_pointer_cast<UnaryExpNode>(node);
            if(unary->node->type() == COMMON_LVALUE){
                std::string id = std::dynamic_pointer_cast<CommonLValueNode>(unary->node)->id;
                if(p2pMap.find(id) != p2pMap.end()){
                    unary->setExprNode(std::dynamic_pointer_cast<ExpressionNode>(p2pMap[id]->deepCopy()));
                }
            }else{
                scanExprToReplacePara(unary->node, p2pMap);
            }
            break;
        }
        case FUNC_CALL_NODE:{
            auto func = std::dynamic_pointer_cast<FuncCallNode>(node);
            int i = 0;
            for(; i < func->pparams.size() ; i++){
                if(func->pparams[i]->type() == COMMON_LVALUE){
                    std::string id = std::dynamic_pointer_cast<CommonLValueNode>(func->pparams[i])->id;
                    if(p2pMap.find(id) != p2pMap.end()){
                        func->pparams[i] = std::dynamic_pointer_cast<ExpressionNode>(p2pMap[id]->deepCopy());
                        func->pparams[i]->parent = func;
                    }
                }else{
                    scanExprToReplacePara(func->pparams[i], p2pMap);
                }
            }
        }
        default:
            break;
    }


}

void FuncInline::exprReplaceFuncCall(const SmartExpr &node, const SmartExpr &funcCall) {

    switch (funcCall->parent->type()) {
        case BINARY_OP_NODE:{
            auto binary = std::dynamic_pointer_cast<BinaryOpExprNode>(funcCall->parent);
            if(binary->lhs == funcCall)
                binary->setLhs(node);
            else
                binary->setRhs(node);
            break;
        }
        case UNART_NODE:{
            auto unary = std::dynamic_pointer_cast<UnaryExpNode>(funcCall->parent);
            unary->setExprNode(node);
            break;
        }
        case FUNC_CALL_NODE:{
            auto func = std::dynamic_pointer_cast<FuncCallNode>(funcCall->parent);
            node->parent = func;
            int i = 0;
            for(; i < func->pparams.size() ; i++){
                if(func->pparams[i] == funcCall){
                   func->pparams[i] = node;
                }
            }
            break;
        }
        case ARRAY_REF_LVALUE:{
            auto aref = std::dynamic_pointer_cast<ArrayRefLValueNode>(funcCall->parent);
            node->parent = aref;
            for(auto& index : aref->arrayRef){
                if(index == funcCall){
                    index = node;
                }
            }
            break;
        }
        case EXPRESSION_STATEMENT:{
            auto exprStmt = std::dynamic_pointer_cast<ExpressionStmtNode>(funcCall->parent);
            exprStmt->setExpr(node);
            break;
        }
        case ASSIGN_STATEMENT:{
            auto assign = std::dynamic_pointer_cast<AssignStmtNode>(funcCall->parent);
            assign->setRhs(node);
            break;
        }
        case IF_STATEMENT:{
            auto ifStmt = std::dynamic_pointer_cast<IfStmtNode>(funcCall->parent);
            ifStmt->l_or_expr = node;
            ifStmt->setLocParent();
            break;
        }
        case WHILE_STATEMENT:{
            auto whileStmt = std::dynamic_pointer_cast<WhileStmtNode>(funcCall->parent);
            whileStmt->l_or_exp = node;
            whileStmt->setLocParent();
            break;
        }
        case RETURN_STATEMENT:{
            auto returnStmt = std::dynamic_pointer_cast<ReturnStmtNode>(funcCall->parent);
            returnStmt->setExp(node);
            break;
        }
        case VAR_DEF:{
            auto varDef = std::dynamic_pointer_cast<VarDef>(funcCall->parent);
            varDef->setInitialNode(node);
            break;
        }
        case INITIALIZED_NODE:{
            auto initilizeNode = std::dynamic_pointer_cast<InitializedNode>(funcCall->parent);
            initilizeNode->setExpr(node);
            break;
        }
        default:
            printf("NodeType: %d\n", funcCall->parent->type());
            assert(false && "not think father node type");
    }


}

void FuncInline::setMustConvertToExpr(const SmartExpr &expr) {

    switch (expr->type()) {
        case BINARY_OP_NODE:{
            auto binary = std::dynamic_pointer_cast<BinaryOpExprNode>(expr);
            if(binary->lhs->type() == FLOAT_ITER_NODE){
                int v = (int) std::dynamic_pointer_cast<FloatValueNode>(binary->lhs)->v;
                binary->setLhs(std::make_shared<IntValueNode>(binary->lhs->pos, v));
            }else{
                setMustConvertToExpr(binary->lhs);
            }
            if(binary->rhs->type() == FLOAT_ITER_NODE){
                int v = (int) std::dynamic_pointer_cast<FloatValueNode>(binary->rhs)->v;
                binary->setRhs(std::make_shared<IntValueNode>(binary->rhs->pos, v));
            }else{
                setMustConvertToExpr(binary->rhs);
            }
            break;
        }
        case COMMON_LVALUE:{
            auto clvalue = std::dynamic_pointer_cast<CommonLValueNode>(expr);
            clvalue->mustConvert = true;
            break;
        }
        case ARRAY_REF_LVALUE:{
            auto aref = std::dynamic_pointer_cast<ArrayRefLValueNode>(expr);
            aref->mustConvert = true;
            break;
        }
        case FUNC_CALL_NODE:{
            auto funCall = std::dynamic_pointer_cast<FuncCallNode>(expr);
            funCall->mustConvert = true;
            break;
        }
        case UNART_NODE:{
            auto unary = std::dynamic_pointer_cast<UnaryExpNode>(expr);
            if(unary->node->type() == FLOAT_ITER_NODE){
                int v = (int)std::dynamic_pointer_cast<FloatValueNode>(unary)->v;
                unary->setExprNode(std::make_shared<IntValueNode>(unary->node->pos, v));
            }else{
                setMustConvertToExpr(unary->node);
            }
            break;
        }
        case FLOAT_ITER_NODE:{
            auto parent = std::dynamic_pointer_cast<FuncCallNode>(expr->parent);
            int v = (int) std::dynamic_pointer_cast<FloatValueNode>(expr)->v;
            auto ivalue = std::make_shared<IntValueNode>(expr->pos, v);
            int i;
            ivalue->parent = parent;
            for(i = 0 ; i < parent->pparams.size() ; ++i){
                if(parent->pparams[i] == expr){
                    parent->pparams[i] = ivalue;
                }
            }
            break;
        }
        default:
            break;
    }
}





