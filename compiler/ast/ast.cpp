/**
 * Author:  Devotes
 * Date:    2022/5/29
 *
 * FixLog:
 *
 */


#include "ast.h"
#include <sstream>
#include <algorithm>
#include <cassert>

using namespace std;


#define POSITION(pos) (pos.file_name + "[" + std::to_string(pos.start_line) + ":" + std::to_string(pos.start_column) + "]")

void inline dealRetract(stringstream& ss, int tab){
    if(tab > 0){
        for(int i = 0; i < tab; i++)
            ss<<"  ";
        ss<<"|----";
    }
}

void inline dealNodeOutput(stringstream& ss, const string& node_name, void* pointer, const position& pos, bool end = true){

    ss<<node_name<<" @"<<hex<<pointer;
    ss<<" "<<POSITION(pos);
    if(end)
        ss<<'\n';
}

void inline dealAttr(stringstream &ss, const string& attr_name, const string& attr_value){
    ss<<" <"<<attr_name<<" = "<<attr_value<<"> ";
}

#define STRN(a) #a

string inline toOperator(Operator op){

    switch (op) {
        case ADD_OP:        return      STRN(ADD_OP);
        case SUB_OP:        return      STRN(SUB_OP);
        case MUL_OP:        return      STRN(MUL_OP);
        case DIV_OP:        return      STRN(DIV_OP);
        case MOD_OP:        return      STRN(MOD_OP);
        case GT:            return      STRN(GT);
        case LT:            return      STRN(LT);
        case LE:            return      STRN(GE);
        case GE:            return      STRN(LE);
        case NOT:           return      STRN(NOT);
        case AND:           return      STRN(AND);
        case OR:            return      STRN(OR);
        case EQUAL_OP:      return      STRN(EQUAL_OP);
        case NEQUAL_OP:     return      STRN(NEQUAL_OP);
    }
    return "";
}

string inline toType(Type tp){

    switch (tp) {
        case INT:           return      "int";
        case VOID:          return      "void";
        case FLOAT:         return      "float";
        default:
            break;
    }
    return "";
}

string inline toUnaryOp(unsigned int op){
    switch (op) {
        case 0: return "+";
        case 1: return "-";
        case 2: return "!";
        default:
            break;
    }
    return "";
}

/*********************
 *Define Of CompUnitNode
 *********************/
void CompUnitNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "CompUnitNode", this, pos);
    tab++;
    for(const auto& elem: elems)
        elem->toAstInfoString(ss, tab);

}

SmartNode CompUnitNode::deepCopy() {
    std::shared_ptr<CompUnitNode> unit = make_shared<CompUnitNode>(pos);
    for(const auto& elem : elems)
        unit->addElem(elem->deepCopy());
    return unit;
}
/*********************
 *Define Of VarDef
 *********************/
SmartNode VarDef::deepCopy() {
    std::shared_ptr<VarDef> vardef = make_shared<VarDef>(pos, id);
    vardef->is_const = is_const;
    if(initial)
        vardef->setInitialNode(dynamic_pointer_cast<ExpressionNode>(initial->deepCopy()));
    return vardef;
}

void VarDef::setInitialNode(const SmartExpr& init) {
    init->parent = shared_from_this();
    initial = init;
}

void VarDef::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "CommonVarDefNode", this, pos, false);
    dealAttr(ss, "Type", toType(var_type));
    dealAttr(ss, "Id", id);
    dealAttr(ss, "IsConst", (is_const?"true":"false"));
    ss<<'\n';

    tab++;
    if(initial) initial->toAstInfoString(ss, tab);
}

/*********************
 *Define Of ArrayVarDef
 *********************/
SmartNode ArrayVarDef::deepCopy() {
    shared_ptr<ArrayVarDef> arrRef = make_shared<ArrayVarDef>(pos, id);
    arrRef->is_const = is_const;
    for(const auto& dim : arrayDimVec)
        arrRef->addArrayDimExpr(dynamic_pointer_cast<ExpressionNode>(dim->deepCopy()));
    if(initial){
        arrRef->initial = dynamic_pointer_cast<InitializedNode>(initial->deepCopy());
    }
    return arrRef;
}

void ArrayVarDef::addArrayDimExpr(const SmartExpr& node) {
    node->parent = shared_from_this();
    arrayDimVec.push_back(node);
}

void ArrayVarDef::setInitialNode(const InitialNode& node) {
    node->parent = shared_from_this();
    initial = node;
}

void ArrayVarDef::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "ArrayVarDefNode", this, pos, false);
    dealAttr(ss, "Type", toType(var_type));
    dealAttr(ss, "Id", id);
    dealAttr(ss, "ArrayDim", to_string(arrayDimVec.size()));
    dealAttr(ss, "IsConst", (is_const?"true":"false"));
    ss<<'\n';

    tab++;
    for(const auto& dim: arrayDimVec)
        dim->toAstInfoString(ss, tab);

    if(initial) initial->toAstInfoString(ss, tab);
}

/*********************
 *Define Of InitializedNode
 *********************/
SmartNode InitializedNode::deepCopy() {
    shared_ptr<InitializedNode> newInit = make_shared<InitializedNode>(pos);
    if(expr){
        newInit->setExpr(dynamic_pointer_cast<ExpressionNode>(expr->deepCopy()));
    }
    for(const auto& init : inits){
        newInit->addInitialExp(dynamic_pointer_cast<InitializedNode>(init->deepCopy()));
    }
    return newInit;
}

void InitializedNode::setExpr(const SmartExpr& exp) {
    exp->parent = shared_from_this();
    expr = exp;
}

void InitializedNode::addInitialExp(const InitialNode& initexp) {
    initexp->parent = shared_from_this();
    inits.push_back(initexp);
}

void InitializedNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "InitializedNode", this, pos);

    tab++;
    if(expr) expr->toAstInfoString(ss, tab);

    for(const auto& init: inits)
        init->toAstInfoString(ss, tab);
}

/*********************
 *Define Of FuncDef
 *********************/
SmartNode FuncDef::deepCopy() {
    shared_ptr<FuncDef> funDef = make_shared<FuncDef>(pos, func_name, ret_type);
    for(const auto& para : funDef->parameters)
        funDef->addParameter(dynamic_pointer_cast<FormalParameter>(para->deepCopy()));
    funDef->setCodeBlock(block->deepCopy());
    return funDef;
}

void FuncDef::addParameter(const FormalPara& para){
    para->parent = shared_from_this();
    parameters.push_back(para);
}

void FuncDef::setCodeBlock(const SmartNode& code_block) {
    code_block->parent = shared_from_this();
    block = code_block;
}

void FuncDef::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "FuncDefNode", this, pos, false);
    dealAttr(ss, "RetType", toType(ret_type));
    dealAttr(ss, "FuncName", func_name);
    ss<<'\n';

    tab++;
    for(const auto& parameter: parameters)
        parameter->toAstInfoString(ss, tab);

    block->toAstInfoString(ss, tab);
}

/*********************
 *Define Of CommonFormalParaNode
 *********************/
void CommonFormalParaNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "CommonFormalParamNode", this, pos, false);
    dealAttr(ss, "Type", toType(ty));
    dealAttr(ss, "FormalParaName", name);
    ss<<'\n';
}

/*********************
 *Define Of ArrayFormalParaNode
 *********************/
void ArrayFormalParaNode::addArrayDimExpr(const SmartExpr& node) {
    node->parent = shared_from_this();
    arrayDims.push_back(node);
}

void ArrayFormalParaNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "ArrayFormalParamNode", this, pos, false);
    dealAttr(ss, "Type", toType(ty));
    dealAttr(ss, "FormalParaName", name);
    dealAttr(ss, "ArrayDim", to_string(arrayDims.size()+1));
    ss<<'\n';

    tab++;
    for(const auto& dim: arrayDims)
        dim->toAstInfoString(ss, tab);
}

/*********************
 *Define Of CodeBlockNode
 *********************/
SmartNode CodeBlockNode::deepCopy() {
    shared_ptr<CodeBlockNode> codeBlk = make_shared<CodeBlockNode>(pos);
    for(const auto& stmt : stmts){
        codeBlk->addStmt(stmt->deepCopy());
    }
    return codeBlk;
}

void CodeBlockNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "CodeBlockStmtNode", this, pos);

    tab++;
    for(const auto& stmt: stmts)
        stmt->toAstInfoString(ss, tab);
}

void CodeBlockNode::replaceStmt(const SmartNode& src, const SmartNode& now) {
    std::replace(stmts.begin(), stmts.end(),src, now);
}

void CodeBlockNode::insertToStmt(const SmartNode &loc, const SmartNode &stmt) {
    auto it = std::find(stmts.begin(), stmts.end(), loc);
    stmts.insert(it, stmt);
}

void CodeBlockNode::copyStmts(const std::vector<SmartNode> &cstmts) {
    for(const auto& stmt : cstmts) {
        addStmt(stmt->deepCopy());
    }
}

void CodeBlockNode::removeStmt(const SmartNode& stmt) {
    auto it = find(stmts.begin(), stmts.end(), stmt);
    stmts.erase(it);
}

/*********************
 *Define Of IfStmtNode
 *********************/
SmartNode IfStmtNode::deepCopy() {
    shared_ptr<IfStmtNode> ifStmt = make_shared<IfStmtNode>(pos, dynamic_pointer_cast<ExpressionNode>(l_or_expr->deepCopy()));
    ifStmt->setLocParent();
    ifStmt->setCorrectStmt(correct_stmt->deepCopy());
    if(else_stmt)
        ifStmt->setElseStmt(else_stmt->deepCopy());
    return ifStmt;
}

void IfStmtNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "IfStmtNode", this, pos);

    tab++;
    l_or_expr->toAstInfoString(ss, tab);
    correct_stmt->toAstInfoString(ss, tab);
    if(else_stmt) else_stmt->toAstInfoString(ss, tab);
}

void IfStmtNode::setLocParent(){
    l_or_expr->parent = shared_from_this();
}

/*********************
 *Define Of WhileStmtNode
 *********************/
SmartNode WhileStmtNode::deepCopy() {
    shared_ptr<WhileStmtNode> whileStmt = make_shared<WhileStmtNode>(pos, dynamic_pointer_cast<ExpressionNode>(l_or_exp->deepCopy()));
    whileStmt->setLocParent();
    whileStmt->setStmt(stmt->deepCopy());
    return whileStmt;
}

void WhileStmtNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "WhileStmtNode", this, pos);

    tab++;
    l_or_exp->toAstInfoString(ss, tab);
    stmt->toAstInfoString(ss, tab);
}

void WhileStmtNode::setLocParent() {
    l_or_exp->parent = shared_from_this();
}

/*********************
 *Define Of BreakStmtNode
 *********************/
SmartNode BreakStmtNode::deepCopy() {
    return make_shared<BreakStmtNode>(pos);
}

void BreakStmtNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "BreakStmtNode", this, pos);
}

/*********************
 *Define Of ContinueStmtNode
 *********************/
SmartNode ContinueStmtNode::deepCopy() {
    return make_shared<ContinueStmtNode>(pos);
}

void ContinueStmtNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "ContinueStmtNode", this, pos);
}

/*********************
 *Define Of ReturnStmtNode
 *********************/
SmartNode ReturnStmtNode::deepCopy() {
    shared_ptr<ReturnStmtNode> retStmt = make_shared<ReturnStmtNode>(pos);
    if(expr)
        retStmt->setExp(dynamic_pointer_cast<ExpressionNode>(expr->deepCopy()));
    return retStmt;
}

void ReturnStmtNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "ReturnStmtNode", this, pos);

    tab++;
    if(expr)
        expr->toAstInfoString(ss, tab);

}

void ReturnStmtNode::setExp(const SmartExpr& exp) {
    exp->parent = shared_from_this();
    expr = exp;
}
/*********************
 *Define Of ExpressionStmtNode
 *********************/
SmartNode ExpressionStmtNode::deepCopy() {
    shared_ptr<ExpressionStmtNode> exprStmt = make_shared<ExpressionStmtNode>(pos);
    if(expr)
        exprStmt->setExpr(dynamic_pointer_cast<ExpressionNode>(expr->deepCopy()));
    return exprStmt;
}

void ExpressionStmtNode::setExpr(const SmartExpr& exp) {
    exp->parent = shared_from_this();
    expr = exp;
}

void ExpressionStmtNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "ExpressionStmtNode", this, pos);

    tab++;
    if(expr) expr->toAstInfoString(ss, tab);
}

/*********************
 *Define Of AssignStmtNode
 *********************/
 AssignStmtNode::AssignStmtNode(const position& pos): Node(pos){}

 SmartNode AssignStmtNode::deepCopy() {
     shared_ptr<AssignStmtNode> assStmt = make_shared<AssignStmtNode>(pos);
     assStmt->setRhs(dynamic_pointer_cast<ExpressionNode>(rhs->deepCopy()));
     assStmt->setLhs(dynamic_pointer_cast<ExpressionNode>(lhs->deepCopy()));
     return assStmt;
 }

 void AssignStmtNode::setLhs(const SmartExpr &expr) {
    expr->parent = shared_from_this();
    lhs = expr;
 }

 void AssignStmtNode::setRhs(const SmartExpr &expr) {
    expr->parent = shared_from_this();
    rhs = expr;
 }

 void AssignStmtNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "AssignStmtNode", this, pos);

    tab++;
    lhs->toAstInfoString(ss, tab);
    rhs->toAstInfoString(ss, tab);
 }

/*********************
 *Define Of CommonLValueNode
 *********************/
SmartNode CommonLValueNode::deepCopy() {
    auto clvalue = make_shared<CommonLValueNode>(pos, id);
    clvalue->mustConvert = mustConvert;
    return clvalue;
}

void CommonLValueNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "CommonLValueNode", this, pos, false);
    dealAttr(ss, "Id", id);
    dealAttr(ss, "isMustConv", mustConvert?"true":"false");
    ss<<'\n';
}

/*********************
 *Define Of ArrayRefLValueNode
 *********************/
SmartNode ArrayRefLValueNode::deepCopy() {
    shared_ptr<ArrayRefLValueNode> arrRef = make_shared<ArrayRefLValueNode>(pos, id);
    for(const auto& index : arrayRef)
        arrRef->addArrayRef(dynamic_pointer_cast<ExpressionNode>(index->deepCopy()));
    arrRef->mustConvert = mustConvert;
    return arrRef;
}

void ArrayRefLValueNode::addArrayRef(const SmartExpr& node) {
    node->parent = shared_from_this();
    arrayRef.push_back(node);
}

void ArrayRefLValueNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "ArrayRefLValueNode", this, pos, false);
    dealAttr(ss, "Id", id);
    dealAttr(ss, "isMustConv", mustConvert?"true":"false");
    ss<<'\n';

    tab++;
    for(const auto& index: arrayRef)
        index->toAstInfoString(ss, tab);
}

/*********************
 *Define Of FuncCallNode
 *********************/
SmartNode FuncCallNode::deepCopy() {
    shared_ptr<FuncCallNode> funCall = make_shared<FuncCallNode>(pos, func_name);
    for(const auto& para : pparams)
        funCall->addParam(dynamic_pointer_cast<ExpressionNode>(para->deepCopy()));
    funCall->mustConvert = mustConvert;
    return funCall;
}

void FuncCallNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "FuncCallNode", this, pos, false);
    dealAttr(ss, "CallFuncName", func_name);
    dealAttr(ss, "isMustConv", mustConvert?"true":"false");
    ss<<'\n';

    tab++;
    for(const auto& para: pparams)
        para->toAstInfoString(ss, tab);
}

/*********************
 *Define Of IntValueNode
 *********************/
void IntValueNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "IntLiteral", this, pos, false);
    dealAttr(ss, "Value", to_string(v));
    ss<<'\n';
}

/*********************
 *Define Of FloatValueNode
 *********************/
void FloatValueNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, "FloatLiteral", this, pos, false);
    dealAttr(ss, "Value", to_string(v));
    ss<<'\n';

}

/*********************
 *Define Of UnaryExpNode
 *********************/
SmartNode UnaryExpNode::deepCopy() {
    shared_ptr<UnaryExpNode> uary = make_shared<UnaryExpNode>(pos, unary_op);
    uary->setExprNode(dynamic_pointer_cast<ExpressionNode>(node->deepCopy()));
    return uary;
}

void UnaryExpNode::setExprNode(const SmartExpr& n) {
    n->parent = shared_from_this();
    node = n;
}

void UnaryExpNode::toAstInfoString(std::stringstream &ss, int tab) {
    dealRetract(ss, tab);
    dealNodeOutput(ss, "UnaryOpNode", this, pos, false);
    dealAttr(ss, "op", toOperator(unary_op));
    ss<<"\n";
    tab++;
    node->toAstInfoString(ss, tab);
}

/*********************
 *Define Of BinaryOpExprNode
 *********************/
SmartNode BinaryOpExprNode::deepCopy() {
    shared_ptr<BinaryOpExprNode> binary = make_shared<BinaryOpExprNode>(pos, oper);
    binary->setLhs(dynamic_pointer_cast<ExpressionNode>(lhs->deepCopy()));
    binary->setRhs(dynamic_pointer_cast<ExpressionNode>(rhs->deepCopy()));
    return binary;
}

void BinaryOpExprNode::toAstInfoString(std::stringstream &ss, int tab) {

    dealRetract(ss, tab);
    dealNodeOutput(ss, toOperator(oper), this, pos);
    tab++;
    lhs->toAstInfoString(ss, tab);
    rhs->toAstInfoString(ss, tab);
}

void BinaryOpExprNode::setLhs(const SmartExpr& ls) {
    ls->parent = shared_from_this();
    lhs = ls;
}

void BinaryOpExprNode::setRhs(const SmartExpr& rs) {
    rs->parent = shared_from_this();
    rhs = rs;
}

SmartNode CommonFormalParaNode::deepCopy() {
    return std::make_shared<CommonFormalParaNode>(pos, ty, name);
}

SmartNode ArrayFormalParaNode::deepCopy() {
    std::shared_ptr<ArrayFormalParaNode> arrFormal = std::make_shared<ArrayFormalParaNode>(pos, ty, name);
    for(const auto& dim : arrayDims)
        arrFormal->addArrayDimExpr(std::dynamic_pointer_cast<ExpressionNode>(dim->deepCopy()));
    return arrFormal;
}

SmartNode IntValueNode::deepCopy() {
    return std::make_shared<IntValueNode>(pos, v);
}

SmartNode FloatValueNode::deepCopy() {
    return std::make_shared<FloatValueNode>(pos, v);
}

#undef POSITION
