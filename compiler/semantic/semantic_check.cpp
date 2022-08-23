/*
 * Author:   Devotes
 * Date:     2022/5/31
 *
 * FixLog:
 *
 */
#include "semantic_check.h"
#include <iostream>
#include "../error/hy_error.h"

#define OUT_MSG(out, msg) out<<msg<<endl;

using namespace std;

/*********************
 *系统函数
 *********************/
static map<string, Type> std_func =
        {
         {"getint", INT},
         {"getfloat", FLOAT},
         {"getch", INT},
         {"getarray", INT},
         {"getfarray", INT},
         {"putint", VOID},
         {"putch",VOID},
         {"putfloat", VOID},
         {"putarray", VOID},
         {"putfarray", VOID},
         {"starttime", VOID},
         {"stoptime", VOID}
        };


/*********************
 *语义分析器的构造器
 *********************/
SemanticChecker::SemanticChecker():out(cerr),sym_table_manager(){}

SemanticChecker::SemanticChecker(std::ostream &out):out(out),sym_table_manager(){}

/*********************
 *语义分析的辅助方法
 *  检查一个表达式是否可以编译
 *  时计算
 *********************/
bool SemanticChecker::checkCanEmit(const std::shared_ptr<ExpressionNode>& node, double &v) {

    visit(node);
    if(node->type() == COMMON_LVALUE){
        auto lvalue = dynamic_pointer_cast<CommonLValueNode>(node);
        if(!has_error){
            CommonVarSymbol symbol = sym_table_manager.getCommonVarSymbol(lvalue->id);
            if(symbol.can_calcu && symbol.is_const){
                if(symbol.type == INT)
                    v = (double)((int)symbol.v);
                else
                    v = symbol.v;
                return true;
            }
        }
        return false;
    }
    else if(node->type() == INT_ITER_NODE){
        auto add = dynamic_pointer_cast<IntValueNode>(node);
        v = (double)(add->v);
        return true;
    }
    else if(node->type() == FLOAT_ITER_NODE){
        auto add = dynamic_pointer_cast<FloatValueNode>(node);
        v = (double)add->v;
        return true;
    }
    else if(node->type() == UNART_NODE){
        auto unary_exp = dynamic_pointer_cast<UnaryExpNode>(node);
        if(checkCanEmit(unary_exp->node, v)){
            if(unary_exp->unary_op == SUB_OP)
                v = -v;
            else if(unary_exp->unary_op == NOT)
                v = !v;
            return true;
        }
        return false;
    }
    else if(node->type() == BINARY_OP_NODE){
        auto binary = dynamic_pointer_cast<BinaryOpExprNode>(node);
        double v1;
        double v2;
        if(checkCanEmit(binary->lhs, v1) && checkCanEmit(binary->rhs, v2)){
            switch (binary->oper) {
                case ADD_OP:
                    v = v1 + v2;
                    break;
                case SUB_OP:
                    v = v1 - v2;
                    break;
                case MUL_OP:
                    v = v1 * v2;
                    break;
                case DIV_OP:
                    v = v1/v2;
                    break;
                case MOD_OP:
                    v = int(v1)%int(v2);
                    break;
                case LE:
                    v = v1 <= v2;
                    break;
                case LT:
                    v = v1 < v2;
                    break;
                case GE:
                    v = v1 >= v2;
                    break;
                case GT:
                    v = v1 > v2;
                    break;
                case AND:
                    v = v1 && v2;
                    break;
                case OR:
                    v = v1 || v2;
                    break;
                case EQUAL_OP:
                    v = v1 == v2;
                    break;
                case NEQUAL_OP:
                    v = v1 != v2;
                    break;
                default:{}
            }
            return true;
        }
        return false;
    }
    return false;
}

int SemanticChecker::condAlwaysTrueOrFalse(const std::shared_ptr<ExpressionNode>& node) {
    double v;
    if(checkCanEmit(node, v)){
        if(v == 0)
            return 1;       // false
        else
            return -1;      // true
    }
    return 0;               // uncertainty
}

/*********************
 *语义分析的辅助方法
 * 获取表达式的类型
 *********************/
Type SemanticChecker::checkExprType(const std::shared_ptr<ExpressionNode>& node) {

    if(node->type() == BINARY_OP_NODE){
        auto binary = dynamic_pointer_cast<BinaryOpExprNode>(node);
        visit(binary->lhs);
        visit(binary->rhs);
        Type t1 = binary->lhs->expr_type;
        Type t2 = binary->rhs->expr_type;
        if(t1 == VOID || t2 == VOID){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, "", EXPR_TYPE_ERROR))
            return VOID;
        }
        if(t1 == t2) return t1;
        else if(t1 == FLOAT || t2 == FLOAT){
            return FLOAT;
        }
        return INT;
    }else
        visit(node);
    return node->expr_type;
}

/*********************
 *语义检查 1
 *  检查变量是否重复定义
 *********************/
bool SemanticChecker::checkReDefine(const std::shared_ptr<VarDef>& node) {
    CommonVarSymbol symbol;
    symbol.id = node->id;
    symbol.type = node->var_type;
    symbol.is_const = node->is_const;
    if(node->initial){
        double v = 0;
        symbol.can_calcu = checkCanEmit(node->initial, v);
        symbol.v = v;
        if(node->initial->expr_type == VOID){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, "", ASSIGN_ERROR))
            return false;
        }
    }
    if(sym_table_manager.isInGlobalScope()){
        if(sym_table_manager.isInVarST(node->id) || sym_table_manager.isInArrayST(node->id) || sym_table_manager.isInFuncST(node->id))
            has_error = true;
    }else{
        if(sym_table_manager.isInCurrentVarST(node->id) || sym_table_manager.isInCurrentArrayST(node->id) || sym_table_manager.isInFuncCallST(node->id))
            has_error = true;
    }
    if(has_error){
        OUT_MSG(out, message_error(node->pos,node->id, REDEFINE))
        return false;
    }

    if(symbol.is_const){
        if(!node->initial){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, node->id, NO_INIT_VAL))
            return false;
        }
        // 全局变量声明
        if(sym_table_manager.isInGlobalScope()){
            if(node->var_type == INT)
                node->initial = std::make_shared<IntValueNode>(node->pos, (int)symbol.v);
            else
                node->initial = std::make_shared<FloatValueNode>(node->pos, symbol.v);
        }else{
            if(symbol.can_calcu){
                if(node->var_type == INT)
                    node->initial = make_shared<IntValueNode>(node->pos, (int)symbol.v);
                else
                    node->initial = make_shared<FloatValueNode>(node->pos, symbol.v);
            }
        }
    }else{
        if(sym_table_manager.isInGlobalScope()){
            float v;
            if(node->initial){
                if(symbol.can_calcu){
                    v = symbol.v;
                }
            }else
                v = symbol.v = 0;
            if(node->var_type == INT)
                node->setInitialNode(make_shared<IntValueNode>(node->pos, (int)v));
            else
                node->setInitialNode(make_shared<FloatValueNode>(node->pos, v));
        }
        else{
            if(symbol.can_calcu){
                if(node->var_type == INT)
                    node->initial = make_shared<IntValueNode>(node->pos, (int)symbol.v);
                else
                    node->initial = make_shared<FloatValueNode>(node->pos, symbol.v);
            }
        }
    }
    sym_table_manager.addVarSymbol(symbol);
    return true;

}

bool SemanticChecker::checkReDefine(const std::shared_ptr<ArrayVarDef>& node) {
    ArrayVarSymbol symbol;
    symbol.id = node->id;
    symbol.is_const = node->is_const;
    symbol.type = node->var_type;
    std::vector<SmartExpr> arrayDim;
    for(const auto& expr: node->arrayDimVec){
        double v = 0;
        if(!checkCanEmit(expr, v)){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, node->id, ARRAY_DIM_NOT_CONST))
            return false;
        }
        if(v <= 0){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, node->id, ARRAY_DIM_LT_ZERO))
            return false;
        }
        if(expr->expr_type == FLOAT){
            has_error = true;
            OUT_MSG(out, message_error(expr->pos, "", ARRAR_DIM_DEFINE_WITH_FLOAT))
            return false;
        }
        // 将表达式全部更换为IntValueNode;
        auto dim = make_shared<IntValueNode>(expr->pos, (int)v);
        dim->expr_type = INT;
        dim->parent = node;
        arrayDim.push_back(dynamic_pointer_cast<ExpressionNode>(dim));
        symbol.arrDims.push_back((int)v);
    }

    node->arrayDimVec.assign(arrayDim.begin(), arrayDim.end());

    if(sym_table_manager.isInGlobalScope()){
        if(sym_table_manager.isInVarST(node->id) || sym_table_manager.isInArrayST(node->id) || sym_table_manager.isInFuncST(node->id))
            has_error = true;
    }else{
        if(sym_table_manager.isInCurrentVarST(node->id) || sym_table_manager.isInCurrentArrayST(node->id) || sym_table_manager.isInFuncCallST(node->id))
            has_error = true;
    }
    if(has_error){
        OUT_MSG(out, message_error(node->pos,node->id, REDEFINE))
        return false;
    }

    // 检查是否是常量，是否已经初始化
    if(symbol.is_const){
        if(!node->initial){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, node->id, NO_INIT_VAL))
            return false;
        }
    }

    arrayType = node->var_type;
    if(node->initial)
        visit(node->initial);
    arrayType = UNDEF;

    if(has_error)
        return false;
    sym_table_manager.addArrayVarSymbol(symbol);
    return true;
}

bool SemanticChecker::checkReDefine(const std::shared_ptr<FuncDef>& node) {

    FuncSymbol symbol;
    symbol.id = node->func_name;
    symbol.ret_type = node->ret_type;
    for(const auto& arg: node->parameters){
        FormalParaSymbol formal;
        if(arg->type() == COMMON_FORMAL_PARA){
            formal.type = dynamic_pointer_cast<CommonFormalParaNode>(arg)->ty;
        }else if(arg->type() == ARRAY_FORMAL_PARA){
            auto array_para = dynamic_pointer_cast<ArrayFormalParaNode>(arg);
            formal.type = array_para->ty;
            for(const auto& expr: array_para->arrayDims){
                double v = 0;
                if(!checkCanEmit(expr, v)){
                    has_error = true;
                    OUT_MSG(out, message_error(node->pos, node->func_name, ARRAY_DIM_NOT_CONST))
                    return false;
                }
                if(v <= 0){
                    has_error = true;
                    OUT_MSG(out, message_error(node->pos, node->func_name, ARRAY_DIM_LT_ZERO))
                    return false;
                }
                if(expr->expr_type == FLOAT){
                    has_error = true;
                    OUT_MSG(out, message_error(expr->pos, "", ARRAR_DIM_DEFINE_WITH_FLOAT))
                    return false;
                }
                formal.array_dim.push_back((int)v);
            }
        }
        symbol.args.push_back(formal);
    }
    if(sym_table_manager.isInCurrentVarST(node->func_name) || sym_table_manager.isInCurrentArrayST(node->func_name) || sym_table_manager.isInFuncST(node->func_name))
        has_error = true;

    if(has_error){
        OUT_MSG(out, message_error(node->pos,node->func_name, REDEFINE))
        return false;
    }
    sym_table_manager.addFuncSymbol(symbol);
    return true;
}

/*********************
 *语义检查 2
 *  检查在变量使用时是否
 *  已经定义，然后是是否是
 *  全局变量然后还与函数重名
 *  于此同时，他还会更新Node的
 *  类型
 *********************/
bool SemanticChecker::checkVarDecl(const std::shared_ptr<CommonLValueNode>& node) {
    if(sym_table_manager.isInGlobalScope()){
        if(sym_table_manager.isInFuncST(node->id)){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, node->id, VAR_USE_WITHOUT_DECL))
            return false;
        }
    }
    if(sym_table_manager.isInVarST(node->id)){
        node->expr_type = sym_table_manager.getCommonVarSymbol(node->id).type;
        return true;
    }else if(sym_table_manager.isInArrayST(node->id)){
        node->expr_type = sym_table_manager.getArrayVarSymbol(node->id).type;
        return true;
    }
    has_error = true;
    OUT_MSG(out, message_error(node->pos, node->id, VAR_USE_WITHOUT_DECL))
    return false;
}

bool SemanticChecker::checkVarDecl(const std::shared_ptr<ArrayRefLValueNode>& node) {
    if(sym_table_manager.isInGlobalScope()){
        if(sym_table_manager.isInFuncST(node->id)){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, node->id, REDEFINE))
            return false;
        }
    }
    if(sym_table_manager.isInArrayST(node->id)){
        node->expr_type = sym_table_manager.getArrayVarSymbol(node->id).type;
        return true;
    }
    has_error = true;
    OUT_MSG(out, message_error(node->pos, node->id, VAR_USE_WITHOUT_DECL))
    return false;
}

/*********************
 *语义检查 3
 *  函数匹配检查，只检查有没有
 *  这个函数,报错处理，报错没
 *  有匹配到任何函数
 *********************/
bool SemanticChecker::checkFuncMatch(const std::shared_ptr<FuncCallNode>& node) {

    for(auto& para: node->pparams)
        visit(para);
    if(sym_table_manager.isInFuncST(node->func_name)){
        sym_table_manager.addFuncCallSymbol(node->func_name);
        node->expr_type = sym_table_manager.getFuncVarSymbol(node->func_name).ret_type;
        return true;
    }
    else if(std_func.find(node->func_name) != std_func.end()){
        node->expr_type = std_func[node->func_name];
        if(node->func_name == "starttime"){
            node->func_name = "_sysy_starttime";
            auto para = make_shared<IntValueNode>(node->pos, node->pos.start_line);
            para->parent = node;
            node->pparams.push_back(para);
        }else if(node->func_name == "stoptime"){
            node->func_name = "_sysy_stoptime";
            auto para = make_shared<IntValueNode>(node->pos, node->pos.start_line);
            para->parent = node;
            node->pparams.push_back(para);
        }
        return true;
    }
    else{
        has_error = true;
        OUT_MSG(out, message_error(node->pos, node->func_name, FUNC_NOT_MATCH_ANY))
        return false;
    }
}

/*********************
 *语义检查:6
 *  检查初始化列表的表达式
 *  首先进行语义检查，然后进行常量折叠优化
 *********************/
bool SemanticChecker::checkInitial(const std::shared_ptr<InitializedNode>& node) {
    if(node->expr){
        double v;
        bool res = checkCanEmit(node->expr, v);
        if(sym_table_manager.isInGlobalScope())
            assert(res);
        if(res){
            if(arrayType == INT){
                node->setExpr(make_shared<IntValueNode>(node->expr->pos, (int)v));
            }else if(arrayType == FLOAT){
                node->setExpr(make_shared<FloatValueNode>(node->expr->pos, (float)v));
            }else{
                has_error = true;
                OUT_MSG(out, message_error(node->expr->pos, "", ASSIGN_WITH_VOID))
                return false;
            }
        }
    }
    for(auto& init: node->inits){
        if(!checkInitial(init))
            return false;
    }
    return true;
}

void SemanticChecker::visit(const std::shared_ptr<InitializedNode>& node) {
        checkInitial(node);
        if(node->expr)
            visit(node->expr);
        for(auto& init: node->inits)
            visit(init);
}

/*********************
 *语义检查:8
 *
 *********************/
bool SemanticChecker::checkArrRefOutOfRange(const std::shared_ptr<ArrayRefLValueNode>& node) {
    if(checkVarDecl(node)){
        ArrayVarSymbol symbol = sym_table_manager.getArrayVarSymbol(node->id);
        if(symbol.arrDims.size() < node->arrayRef.size()){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, "", ARRAY_OFF_OUTRANGE))
            return false;
        }
        return true;
    }
    return false;
}

/*********************
 *语义检查:
 *  重写Visitor对抽象语法
 *树进行语义检查
 *********************/
 void SemanticChecker::visit(const std::shared_ptr<CompUnitNode>& node){

     for(const auto& child: node->elems){
         if(child->type() == VAR_DEF){
             visit(dynamic_pointer_cast<VarDef>(child));
         }
         else if(child->type() == ARRAY_VAR_DEF){
             visit(dynamic_pointer_cast<ArrayVarDef>(child));
         }
         else if(child->type() == FUNC_DEF){
             visit(dynamic_pointer_cast<FuncDef>(child));
         }
         else{
             assert(false);
         }
         if(has_error)
             return;
     }
 }

void SemanticChecker::visit(const std::shared_ptr<VarDef>& node) {
    checkReDefine(node);
 }

void SemanticChecker::visit(const std::shared_ptr<ArrayVarDef>& node) {
    checkReDefine(node);
 }

/******************************************************
 *语义检查:
 *  对于函数声明的检查，首先会检查是否重复声明函数，然后检查函数的
 *
 *
 ******************************************************/
void SemanticChecker::visit(const std::shared_ptr<FuncDef>& node) {
     if(!checkReDefine(node)){
        return;
     }
     // 进入新的符号表
     sym_table_manager.entryCurrentSymbol(node->func_name);
     for(const auto& para: node->parameters){
         if(para->type() == COMMON_FORMAL_PARA){
             auto formal = dynamic_pointer_cast<CommonFormalParaNode>(para);
             CommonVarSymbol symbol;
             symbol.type = formal->ty;
             symbol.id = formal->name;
             symbol.can_calcu = false;
             symbol.is_const = false;
             sym_table_manager.addVarSymbol(symbol);
         }
         else if(para->type() == ARRAY_FORMAL_PARA){
             auto formal = dynamic_pointer_cast<ArrayFormalParaNode>(para);
             ArrayVarSymbol symbol;
             symbol.type = formal->ty;
             symbol.id = formal->name;
             symbol.arrDims.push_back(-1);
             symbol.is_const = false;
             vector<SmartExpr> array_dim;
             for(const auto& expr: formal->arrayDims){
                 double v = 0;
                 if(!checkCanEmit(expr, v)){
                     has_error = true;
                     OUT_MSG(out, message_error(formal->pos, formal->name, ARRAY_DIM_NOT_CONST))
                     return;
                 }
                 if(v <= 0){
                     has_error = true;
                     OUT_MSG(out, message_error(formal->pos, formal->name, ARRAY_DIM_LT_ZERO))
                     return;
                 }

                 auto dim = make_shared<IntValueNode>(expr->pos, (int)v);
                 array_dim.push_back(dim);
                 symbol.arrDims.push_back((int)v);
             }
             formal->arrayDims.assign(array_dim.begin(), array_dim.end());
             sym_table_manager.addArrayVarSymbol(symbol);
         }
     }
     visit(dynamic_pointer_cast<CodeBlockNode>(node->block));
     sym_table_manager.leaveCurrentSymbol();

     // 离开新的符号表
 }

void SemanticChecker::visit(const std::shared_ptr<CommonFormalParaNode>& node) {}

void SemanticChecker::visit(const std::shared_ptr<ArrayFormalParaNode>& node) {}

void SemanticChecker::visitStmtNode(const std::shared_ptr<Node>& child) {
    if(child->type() == VAR_DEF){
        visit(dynamic_pointer_cast<VarDef>(child));
    }
    else if(child->type() == ARRAY_VAR_DEF){
        visit(dynamic_pointer_cast<ArrayVarDef>(child));
    }
    else if(child->type() == IF_STATEMENT){
        visit(dynamic_pointer_cast<IfStmtNode>(child));
    }
    else if(child->type() == WHILE_STATEMENT){
        visit(dynamic_pointer_cast<WhileStmtNode>(child));
    }
    else if(child->type() == ASSIGN_STATEMENT){
        visit(dynamic_pointer_cast<AssignStmtNode>(child));
    }
    else if(child->type() == EXPRESSION_STATEMENT){
        visit(dynamic_pointer_cast<ExpressionStmtNode>(child));
    }
    else if(child->type() == CONTINUE_STATEMENT){
        visit(dynamic_pointer_cast<ContinueStmtNode>(child));
    }
    else if(child->type() == BREAK_STATEMENT){
        visit(dynamic_pointer_cast<BreakStmtNode>(child));
    }
    else if(child->type() == RETURN_STATEMENT){
        visit(dynamic_pointer_cast<ReturnStmtNode>(child));
    }
    else if(child->type() == CODE_BLOCK){
        sym_table_manager.entryCurrentSymbol();
        visit(dynamic_pointer_cast<CodeBlockNode>(child));
        sym_table_manager.leaveCurrentSymbol();
    }
 }

void SemanticChecker::visit(const std::shared_ptr<CodeBlockNode>& node) {
     for(const auto& child: node->stmts){
         if(!child)
             continue;
         visitStmtNode(child);
         if(has_error)
             return;
     }
 }

void SemanticChecker::visit(const std::shared_ptr<IfStmtNode>& node) {
    visit(node->l_or_expr);
    if(node->l_or_expr->expr_type == VOID){
        has_error = true;
        OUT_MSG(out, message_error(node->pos, "", EXPR_TYPE_ERROR))
    }
    isCorrect = true;
    visitStmtNode(node->correct_stmt);
    isCorrect = false;

    if(node->else_stmt)
        visitStmtNode(node->else_stmt);

    // 语义检查完毕之后进行优化
    if(!has_error){
        auto parent = node->parent;
        int v = condAlwaysTrueOrFalse(node->l_or_expr);
        if(v == 1){
            if(parent->type() == CODE_BLOCK){
                if(node->else_stmt)
                    dynamic_pointer_cast<CodeBlockNode>(parent)->replaceStmt(node, node->else_stmt);
                else{
                    dynamic_pointer_cast<CodeBlockNode>(parent)->removeStmt(node);
                }
            }
            else if(parent->type() == IF_STATEMENT){
                auto if_stmt = dynamic_pointer_cast<IfStmtNode>(parent);
                if(isCorrect){
                    if(node->else_stmt){
                        if_stmt->correct_stmt = node->else_stmt;
                    }else{
                        if_stmt->correct_stmt = nullptr;
                    }
                }else{
                    if(node->else_stmt)
                        if_stmt->else_stmt = node->else_stmt;
                    else
                        if_stmt->else_stmt = nullptr;
                }

            }else if(parent->type() == WHILE_STATEMENT){
                auto while_stmt = dynamic_pointer_cast<WhileStmtNode>(parent);
                if(node->else_stmt){
                    while_stmt->stmt = node->else_stmt;
               }else{
                    while_stmt->stmt = nullptr;
                }
            }
        }else if(v == -1){
            node->else_stmt = nullptr;
            if(parent->type() == CODE_BLOCK)
                dynamic_pointer_cast<CodeBlockNode>(parent)->replaceStmt(node, node->correct_stmt);
            else if(parent->type() == IF_STATEMENT){
                auto if_stmt = dynamic_pointer_cast<IfStmtNode>(parent);
                if(isCorrect){
                    if_stmt->correct_stmt = node->correct_stmt;
                }else{
                    if_stmt->else_stmt = node->correct_stmt;
                }
            }else if(parent->type() == WHILE_STATEMENT){
                auto while_stmt = dynamic_pointer_cast<WhileStmtNode>(parent);
                while_stmt->stmt = node->correct_stmt;
            }
        }
    }
 }

void SemanticChecker::visit(const std::shared_ptr<WhileStmtNode>& node) {
    visit(node->l_or_exp);
    if(node->l_or_exp->expr_type == VOID){
        has_error = true;
        OUT_MSG(out, message_error(node->pos, "", EXPR_TYPE_ERROR))
    }
    visitStmtNode(node->stmt);

    // 进行死代码删除优化
    int v = condAlwaysTrueOrFalse(node->l_or_exp);
    if(v == 0)
        return;
    auto parent = node->parent;
    if(v == 1){
        if(parent->type() == CODE_BLOCK)
            dynamic_pointer_cast<CodeBlockNode>(parent)->removeStmt(node);
        else if(parent->type() == IF_STATEMENT){
            auto if_stmt = dynamic_pointer_cast<IfStmtNode>(parent);
            if(isCorrect){
                if_stmt->correct_stmt = nullptr;
            }else{
                if_stmt->else_stmt = nullptr;
            }
        }
        else if(parent->type() == WHILE_STATEMENT){
            auto while_stmt = dynamic_pointer_cast<WhileStmtNode>(parent);
            while_stmt->stmt = node->stmt;
        }
    }
 }

void SemanticChecker::visit(const std::shared_ptr<BreakStmtNode>& node) {
    auto temp = dynamic_pointer_cast<Node>(node);
    while(temp){
        if(temp->type() == WHILE_STATEMENT)
            return;
        temp = temp->parent;
    }
    has_error = true;
    OUT_MSG(out, message_error(node->pos, "break", BAD_BREAK))
 }

void SemanticChecker::visit(const std::shared_ptr<ContinueStmtNode>& node) {
    auto temp = dynamic_pointer_cast<Node>(node);
    while(temp){
        if(temp->type() == WHILE_STATEMENT)
            return;
        temp = temp->parent;
    }
    has_error = true;
    OUT_MSG(out, message_error(node->pos, "break", BAD_CONTINUE))
 }

void SemanticChecker::visit(const std::shared_ptr<ReturnStmtNode>& node) {
     if(node->expr){
         visit(node->expr);
         Type ty = node->expr->expr_type;
         double v;
         position p = {0, 0, ""};
         if(checkCanEmit(node->expr, v)){
             if(ty == FLOAT)
                 node->expr = make_shared<FloatValueNode>(p, v);
             else
                 node->expr = make_shared<IntValueNode>(p, (int)v);
         }
     }
 }

void SemanticChecker::visit(const std::shared_ptr<ExpressionStmtNode>& node) {
     if(node->expr){
         visit(node->expr);
     }
 }

void SemanticChecker::visit(const std::shared_ptr<AssignStmtNode>& node) {
    visit(node->lhs);
    // COMMON_LVALUE或着ARRAY_REF_LVALUE是否是常量
    if(node->lhs->type() == COMMON_LVALUE){
        bool is_const = sym_table_manager.getCommonVarSymbol(dynamic_pointer_cast<CommonLValueNode>(node->lhs)->id).is_const;
        if(is_const){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, "", ASSIGN_CONST_VAR))
            return;
        }
    }
    else if(node->lhs->type() == ARRAY_REF_LVALUE){
        bool is_const = sym_table_manager.getArrayVarSymbol(dynamic_pointer_cast<ArrayRefLValueNode>(node->lhs)->id).is_const;
        if(is_const){
            has_error = true;
            OUT_MSG(out, message_error(node->pos, "", ASSIGN_CONST_VAR))
            return;
        }
    }

    // 检查右值
    visit(node->rhs);
    if(node->rhs->expr_type == VOID){
        has_error = true;
        OUT_MSG(out, message_error(node->pos, "", ASSIGN_ERROR))
        return;
    }

    // 进行常量折叠优化
    double v;
    if(checkCanEmit(node->rhs, v)){
        position p = {0,0,""};
        if(node->lhs->expr_type == FLOAT)
            node->rhs = make_shared<FloatValueNode>(p, (float)v);
        else
            node->rhs = make_shared<IntValueNode>(p, (int)v);
    }
    // 代数简化
//    else{
//        ExpressionNode* expr = node->rhs;
//        if(expr->type() == BINARY_OP_NODE){
//            ExpressionNode* lhs = ((BinaryOpExprNode*)expr)->lhs;
//            ExpressionNode* rhs = ((BinaryOpExprNode*)expr)->rhs;
//            double l1;
//            if(checkCanEmit(lhs, l1)){
//
//            }else if(checkCanEmit(rhs, l1)){
//
//            }
//        }
//    }

}

 /* 对表达式的节点进行语义检查 */
void SemanticChecker::visit(const std::shared_ptr<ExpressionNode>& node) {
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
        case INT_ITER_NODE:{
            visit(dynamic_pointer_cast<IntValueNode>(node));
            break;
        }
        case FLOAT_ITER_NODE:{
            visit(dynamic_pointer_cast<FloatValueNode>(node));
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

void SemanticChecker::visit(const std::shared_ptr<CommonLValueNode>& node) {
    checkVarDecl(node);
 }

void SemanticChecker::visit(const std::shared_ptr<ArrayRefLValueNode>& node) {
    checkArrRefOutOfRange(node);
    for(auto& expr: node->arrayRef)
        visit(expr);
 }

void SemanticChecker::visit(const std::shared_ptr<FuncCallNode>& node) {
    checkFuncMatch(node);
 }

void SemanticChecker::visit(const std::shared_ptr<IntValueNode>& node) {
    node->expr_type = INT;
}

void SemanticChecker::visit(const std::shared_ptr<FloatValueNode>& node) {
    node->expr_type = FLOAT;
}

void SemanticChecker::visit(const std::shared_ptr<UnaryExpNode>& node) {
    visit(node->node);
    node->expr_type = node->node->expr_type;
}

 void SemanticChecker::visit(const std::shared_ptr<BinaryOpExprNode>& node) {
     node->expr_type = checkExprType(node);
 }

