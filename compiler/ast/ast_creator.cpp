/**
 * Author:  Devotes
 * Date:    2022/5/29
 *
 * FixLog:
 *
 */

#include "ast_creator.h"
#include "../common.h"
#include "ast.h"
#include <memory>
#include "../utility/hy_utility.h"

// Terminal Node define
using namespace antlrcpp;

position getPositions(antlr4::ParserRuleContext *context){
    antlr4::Token *start = context->getStart();
    position pos{static_cast<int>(start->getLine()),
                 static_cast<int>(start->getCharPositionInLine()),
                 start->getTokenSource()->getSourceName()};
    return pos;
}

antlrcpp::Any NewAstCreator::visitBtype(ParserParser::BtypeContext *context) {
    if(context->Int())
        return INT;
    else
        return FLOAT;
}

antlrcpp::Any NewAstCreator::visitFunc_type(ParserParser::Func_typeContext *context) {
    if(context->Int())
        return INT;
    else if(context->Float())
        return FLOAT;
    else
        return VOID;
}

antlrcpp::Any NewAstCreator::visitUnary_op(ParserParser::Unary_opContext *context) {
    if(context->Add())
        return ADD_OP;
    else if(context->Sub())
        return SUB_OP;
    return NOT;
}

antlrcpp::Any NewAstCreator::visitSem_op(ParserParser::Sem_opContext *context) {
    if(context->Mul())
        return MUL_OP;
    else if(context->Div())
        return DIV_OP;
    return MOD_OP;
}

antlrcpp::Any NewAstCreator::visitOp_as(ParserParser::Op_asContext *context) {
    if(context->Add())
        return ADD_OP;
    return SUB_OP;
}

antlrcpp::Any NewAstCreator::visitCom_op(ParserParser::Com_opContext *context) {
    if(context->Gt())
        return GT;
    else if(context->Ge())
        return GE;
    else if(context->Lt())
        return LT;
    return LE;
}

antlrcpp::Any NewAstCreator::visitCom_op_eqm(ParserParser::Com_op_eqmContext *context) {
    if(context->Equal())
        return EQUAL_OP;
    return NEQUAL_OP;
}

antlrcpp::Any NewAstCreator::visitL_val(ParserParser::L_valContext *context) {

    if(!context->exp().empty()){
        std::shared_ptr<ArrayRefLValueNode> a_l_value(new ArrayRefLValueNode(getPositions(context), context->Id()->getText()));
        for(auto& exp: context->exp()){
            a_l_value->addArrayRef(visitExp(exp).as<SmartExpr>());
        }
        return std::dynamic_pointer_cast<ExpressionNode>(a_l_value);
    }else{
        std::shared_ptr<CommonLValueNode> l_value(new CommonLValueNode(getPositions(context), context->Id()->getText()));
        return std::dynamic_pointer_cast<ExpressionNode>(l_value);
    }
}

antlrcpp::Any NewAstCreator::visitPrimary_exp(ParserParser::Primary_expContext *context) {

    if(context->exp()){
        return visitExp(context->exp());
    }else if(context->l_val()){
        return visitL_val(context->l_val());
    }else if(context->IntConst()){
        int v;
        v = strToInt(context->IntConst()->getText());
        auto expr = std::make_shared<IntValueNode>(getPositions(context), v);
        return std::dynamic_pointer_cast<ExpressionNode>(expr);
    }else if(context->FloatConst()){
        double v;
        v = strToFloat(context->FloatConst()->getText());
        auto expr = std::make_shared<FloatValueNode>(getPositions(context), (float)v);
        return std::dynamic_pointer_cast<ExpressionNode>(expr);
    }
    assert(false && __LINE__);
}

antlrcpp::Any NewAstCreator::visitUnary_exp(ParserParser::Unary_expContext *context) {


    std::shared_ptr<UnaryExpNode> node = nullptr;
    std::shared_ptr<UnaryExpNode> curr_node = nullptr;

    position pos = getPositions(context);

    if(!context->unary_op().empty()){
        node = std::make_shared<UnaryExpNode>(getPositions(context->unary_op(0)), visitUnary_op(context->unary_op(0)).as<Operator>());
        curr_node = node;

        int i = 1;
        int len = (int)context->unary_op().size();
        while(i < len){
            auto new_node = std::make_shared<UnaryExpNode>(getPositions(context->unary_op(i)), visitUnary_op(context->unary_op(i)).as<Operator>());
            curr_node->setExprNode(new_node);
            curr_node = new_node;
            i++;
        }
    }


    if(context->primary_exp()){
        auto primary_node = visitPrimary_exp(context->primary_exp()).as<SmartExpr>();
        if(curr_node){
            curr_node->setExprNode(primary_node);
            return std::dynamic_pointer_cast<ExpressionNode>(node);
        }
        return primary_node;
    }

    else if(context->Id() && context->SRp() && context->SLp()){
        auto func_call_node = std::make_shared<FuncCallNode>(getPositions(context), context->Id()->getText());
        if(context->func_r_params()){
            auto vec = visitFunc_r_params(context->func_r_params()).as<std::vector<SmartExpr>>();
            for(auto &node: vec){
                func_call_node->addParam(node);
            }
        }
        if(curr_node){
            curr_node->setExprNode(std::dynamic_pointer_cast<ExpressionNode>(func_call_node));
            return std::dynamic_pointer_cast<ExpressionNode>(node);
        }
        return std::dynamic_pointer_cast<ExpressionNode>(func_call_node);
    }

    assert(false && __LINE__);
}

antlrcpp::Any NewAstCreator::visitFunc_r_params(ParserParser::Func_r_paramsContext *context) {

    std::vector<SmartExpr> nodes{};

    for(auto& exp: context->exp()){
        auto node = visitExp(exp).as<SmartExpr>();
        nodes.push_back(node);
    }

    return nodes;
}

antlrcpp::Any NewAstCreator::visitMul_exp(ParserParser::Mul_expContext *context) {

    if(context->unary_exp().size() < 2){
        return visitUnary_exp(context->unary_exp(0));
    }else{
        int len = (int)context->sem_op().size();
        int i = 0;

        auto node = std::make_shared<BinaryOpExprNode>(getPositions(context), visitSem_op(context->sem_op(i)).as<Operator>());
        node->setLhs(visitUnary_exp(context->unary_exp(i)).as<SmartExpr>());

        i++;
        while(i < len){
            position pos1 = getPositions(context->unary_exp(i));
            position end_pos{pos1.start_line, pos1.start_column, pos1.file_name};

            auto father_node = std::make_shared<BinaryOpExprNode>(end_pos, visitSem_op(context->sem_op(i)).as<Operator>());
            node->setRhs(visitUnary_exp(context->unary_exp(i)).as<SmartExpr>());
            father_node->setLhs(std::dynamic_pointer_cast<ExpressionNode>(node));
            node = father_node;
            i++;
        }

        node->setRhs(visitUnary_exp(context->unary_exp(i)).as<SmartExpr>());

        return std::dynamic_pointer_cast<ExpressionNode>(node);
    }
}

antlrcpp::Any NewAstCreator::visitAdd_exp(ParserParser::Add_expContext *context) {

    if(context->mul_exp().size() < 2){
        return visitMul_exp(context->mul_exp(0));
    }else{

        int len = (int)context->op_as().size();
        int i = 0;

        auto node = std::make_shared<BinaryOpExprNode>(getPositions(context), visitOp_as(context->op_as(i)).as<Operator>());
        node->setLhs(visitMul_exp(context->mul_exp(i)).as<SmartExpr>());

        i++;
        while(i < len){
            position pos1 = getPositions(context->mul_exp(i));

            auto father_node = std::make_shared<BinaryOpExprNode>(pos1, visitOp_as(context->op_as(i)).as<Operator>());
            node->setRhs(visitMul_exp(context->mul_exp(i)).as<SmartExpr>());
            father_node->setLhs(std::dynamic_pointer_cast<ExpressionNode>(node));
            node = father_node;
            i++;
        }

        node->setRhs(visitMul_exp(context->mul_exp(i)).as<SmartExpr>());

        return std::dynamic_pointer_cast<ExpressionNode>(node);
    }
}

antlrcpp::Any NewAstCreator::visitRel_exp(ParserParser::Rel_expContext *context) {

    if(context->add_exp().size() < 2){
        return visitAdd_exp(context->add_exp(0));
    }else{

        int len = (int)context->com_op().size();
        int i = 0;

        auto node = std::make_shared<BinaryOpExprNode>(getPositions(context), visitCom_op(context->com_op(i)).as<Operator>());
        node->setLhs(visitAdd_exp(context->add_exp(i)).as<SmartExpr>());

        i++;
        while(i < len){

            position pos1 = getPositions(context->add_exp(i));

            auto father_node = std::make_shared<BinaryOpExprNode>(pos1, visitCom_op(context->com_op(i)).as<Operator>());
            node->setRhs(visitAdd_exp(context->add_exp(i)).as<SmartExpr>());
            father_node->setLhs(std::dynamic_pointer_cast<ExpressionNode>(node));
            node = father_node;
            i++;
        }
        node->setRhs(visitAdd_exp(context->add_exp(i)).as<SmartExpr>());

        return std::dynamic_pointer_cast<ExpressionNode>(node);
    }
}

antlrcpp::Any NewAstCreator::visitEq_exp(ParserParser::Eq_expContext *context) {

    if(context->rel_exp().size() < 2){
        return visitRel_exp(context->rel_exp(0));
    }
    else{
        int len = (int)context->com_op_eqm().size();
        int i = 0;

        auto node = std::make_shared<BinaryOpExprNode>(getPositions(context), visitCom_op_eqm(context->com_op_eqm(i)).as<Operator>());
        node->setLhs(visitRel_exp(context->rel_exp(i)).as<SmartExpr>());

        i++;
        while(i < len){

            position pos1 = getPositions(context->rel_exp(i));

            auto father_node = std::make_shared<BinaryOpExprNode>(pos1, visitCom_op_eqm(context->com_op_eqm(i)).as<Operator>());
            node->setRhs(visitRel_exp(context->rel_exp(i)).as<SmartExpr>());
            father_node->setLhs(node);
            node = father_node;
            i++;

        }
        node->setRhs(visitRel_exp(context->rel_exp(i)).as<SmartExpr>());

        return std::dynamic_pointer_cast<ExpressionNode>(node);
    }

}

antlrcpp::Any NewAstCreator::visitL_and_exp(ParserParser::L_and_expContext *context) {

    if(context->eq_exp().size() < 2){
        return visitEq_exp(context->eq_exp(0));
    }else{

        int len = (int)context->eq_exp().size()-1;
        int i = 0;

        auto node = std::make_shared<BinaryOpExprNode>(getPositions(context), AND);
        node->setLhs(visitEq_exp(context->eq_exp(i)).as<SmartExpr>());

        i++;
        while(i < len){
            position pos1 = getPositions(context->eq_exp(i));

            auto father_node = std::make_shared<BinaryOpExprNode>(pos1, AND);
            node->setRhs(visitEq_exp(context->eq_exp(i)).as<SmartExpr>());
            father_node->setLhs(node);
            node = father_node;
            i++;
        }
        node->setRhs(visitEq_exp(context->eq_exp(i)).as<SmartExpr>());

        return std::dynamic_pointer_cast<ExpressionNode>(node);
    }

}

antlrcpp::Any NewAstCreator::visitL_or_exp(ParserParser::L_or_expContext *context) {

    if(context->l_and_exp().size() < 2){
        return visitL_and_exp(context->l_and_exp(0));
    }else{

        int len = (int)context->l_and_exp().size() - 1;
        int i = 0;

        auto node = std::make_shared<BinaryOpExprNode>(getPositions(context), OR);
        node->setLhs(visitL_and_exp(context->l_and_exp(i)).as<SmartExpr>());

        i++;
        while(i < len){

            position pos1 = getPositions(context->l_and_exp(i));
            position end_pos{pos1.start_line, pos1.start_column, pos1.file_name};

            auto father_node = std::make_shared<BinaryOpExprNode>(end_pos, OR);
            father_node->setLhs(std::dynamic_pointer_cast<ExpressionNode>(node));
            node->setRhs(visitL_and_exp(context->l_and_exp(i)).as<SmartExpr>());
            node = father_node;
            i++;
        }
        node->setRhs(visitL_and_exp(context->l_and_exp(i)).as<SmartExpr>());

        return std::dynamic_pointer_cast<ExpressionNode>(node);
    }
}

antlrcpp::Any NewAstCreator::visitExp(ParserParser::ExpContext *context) {
    return visitAdd_exp(context->add_exp());
}

antlrcpp::Any NewAstCreator::visitConst_exp(ParserParser::Const_expContext *context) {

    return visitAdd_exp(context->add_exp());
}

antlrcpp::Any NewAstCreator::visitStmt(ParserParser::StmtContext *context) {

    if( context->l_val() && context->Assign() && context->exp()){
        auto node = std::make_shared<AssignStmtNode>(getPositions(context));
        node->setRhs(visitExp(context->exp()).as<SmartExpr>());
        node->setLhs(visitL_val(context->l_val()).as<SmartExpr>());
        return std::dynamic_pointer_cast<Node>(node);
    }
    else if(context->block()){
        return visitBlock(context->block());
    }
    else if(context->While() && context->l_or_exp()){
        auto node = std::make_shared<WhileStmtNode>(getPositions(context), visitL_or_exp(context->l_or_exp()).as<SmartExpr>());
        node->setLocParent();
        node->setStmt(visitStmt(context->stmt(0)).as<SmartNode>());
        return std::dynamic_pointer_cast<Node>(node);
    }
    else if(context->If() && context->l_or_exp()){
        auto node = std::make_shared<IfStmtNode>(getPositions(context), visitL_or_exp(context->l_or_exp()).as<SmartExpr>());
        node->setLocParent();
        node->setCorrectStmt(visitStmt(context->stmt(0)).as<SmartNode>());
        if(context->Else()){
            node->setElseStmt(visitStmt(context->stmt(1)).as<SmartNode>());
        }
        return std::dynamic_pointer_cast<Node>(node);
    }
    else if(context->Break()){
        auto node = std::make_shared<BreakStmtNode>(getPositions(context));
        return std::dynamic_pointer_cast<Node>(node);
    }
    else if(context->Continue()){
        auto node = std::make_shared<ContinueStmtNode>(getPositions(context));
        return std::dynamic_pointer_cast<Node>(node);
    }
    else if(context->Return()){
        auto node = std::make_shared<ReturnStmtNode>(getPositions(context));
        if(context->exp())
            node->setExp(visitExp(context->exp()).as<SmartExpr>());
        return std::dynamic_pointer_cast<Node>(node);
    }

    auto exp = std::make_shared<ExpressionStmtNode>(getPositions(context));
    if(context->add_exp()){
        exp->setExpr(visitAdd_exp(context->add_exp()).as<SmartExpr>());
    }

    return std::dynamic_pointer_cast<Node>(exp);
}

antlrcpp::Any NewAstCreator::visitBlock(ParserParser::BlockContext *context) {

    auto node = std::make_shared<CodeBlockNode>(getPositions(context));
    int i1 = 0;
    int i2 = 0;
    int len1 = (int)context->stmt_vec.size();
    int len2 = (int)context->decl_vec.size();

    while(i1 < len1 || i2 < len2){
        if(i1 < len1 && i2 < len2){
            if(context->stmt_vec[i1].first < context->decl_vec[i2].first){
                node->addStmt(visitStmt(context->stmt_vec[i1].second).as<SmartNode>());
                i1++;
            }
            else{
                auto vec = visitDecl(context->decl_vec[i2].second).as<std::vector<SmartNode>>();
                for(auto& elem: vec){
                    node->addStmt(elem);
                }
                i2++;
            }
        }
        else if(i1 < len1){
            node->addStmt(visitStmt(context->stmt_vec[i1].second).as<SmartNode>());
            i1++;
        }
        else if(i2 < len2){
            auto vec = visitDecl(context->decl_vec[i2].second).as<std::vector<SmartNode>>();
            for(auto& elem: vec){
                node->addStmt(elem);
            }
            i2++;
        }
    }
    return std::dynamic_pointer_cast<Node>(node);
}

antlrcpp::Any NewAstCreator::visitFunc_f_param(ParserParser::Func_f_paramContext *context) {

    if((int)context->Rsb().size() > 0 && (int)context->Lsb().size() > 0){
        auto node = std::make_shared<ArrayFormalParaNode>(getPositions(context), visitBtype(context->btype()).as<Type>(), context->Id()->getText());
        for(auto exp: context->const_exp()){
            node->addArrayDimExpr(visitConst_exp(exp).as<SmartExpr>());
        }
        return std::dynamic_pointer_cast<FormalParameter>(node);
    }else{
        auto node = std::make_shared<CommonFormalParaNode>(getPositions(context), visitBtype(context->btype()).as<Type>(), context->Id()->getText());
        return std::dynamic_pointer_cast<FormalParameter>(node);
    }
}

antlrcpp::Any NewAstCreator::visitFunc_f_params(ParserParser::Func_f_paramsContext *context) {

    std::vector<std::shared_ptr<FormalParameter>> nodes{};
    for(auto& fparams: context->func_f_param()){
        nodes.push_back(visitFunc_f_param(fparams).as<std::shared_ptr<FormalParameter>>());
    }

    return nodes;
}

antlrcpp::Any NewAstCreator::visitFunc_def(ParserParser::Func_defContext *context) {

    auto node = std::make_shared<FuncDef>(getPositions(context), context->Id()->getText(), visitFunc_type(context->func_type()).as<Type>());

    if(context->func_f_params()){
        auto vec = visitFunc_f_params(context->func_f_params()).as<std::vector<std::shared_ptr<FormalParameter>>>();
        for(auto& fparam: vec){
            node->addParameter(fparam);
        }
    }
    node->setCodeBlock(visitBlock(context->block()).as<SmartNode>());

    return std::dynamic_pointer_cast<Node>(node);

}

antlrcpp::Any NewAstCreator::visitInit_val(ParserParser::Init_valContext *context) {
    std::shared_ptr<InitializedNode> node(new InitializedNode(getPositions(context)));
    if(context->exp()){
        node->setExpr(visitExp(context->exp()).as<SmartExpr>());
    }else{
        for(auto& init: context->init_val()){
            node->addInitialExp(visitInit_val(init).as<InitialNode>());
        }
    }
    return node;
}

antlrcpp::Any NewAstCreator::visitVar_def(ParserParser::Var_defContext *context) {
    if(!context->const_exp().empty()){
        std::shared_ptr<ArrayVarDef> array_var(new ArrayVarDef(getPositions(context), context->Id()->getText()));
        for(const auto& exp: context->const_exp()){
            array_var->addArrayDimExpr(visitConst_exp(exp).as<SmartExpr>());
        }
        if(context->Assign() && context->init_val())
            array_var->setInitialNode(visitInit_val(context->init_val()).as<InitialNode>());
        return std::dynamic_pointer_cast<Node>(array_var);
    }else{
        std::shared_ptr<VarDef> var(new VarDef(getPositions(context), context->Id()->getText()));
        if(context->Assign() && context->init_val())
            var->setInitialNode(visitExp(context->init_val()->exp()).as<SmartExpr>());
        return std::dynamic_pointer_cast<Node>(var);
    }
}

antlrcpp::Any NewAstCreator::visitVar_decl(ParserParser::Var_declContext *context) {
    std::vector<SmartNode> nodes{};
    Type type = visitBtype(context->btype()).as<Type>();
    for(auto& def: context->var_def()){
        auto node = visitVar_def(def).as<SmartNode>();
        if(node->type() == ARRAY_VAR_DEF)
            std::dynamic_pointer_cast<ArrayVarDef>(node)->var_type = type;
        else
            std::dynamic_pointer_cast<VarDef>(node)->var_type = type;
        nodes.push_back(node);
    }
    return nodes;
}

antlrcpp::Any NewAstCreator::visitConst_init_val(ParserParser::Const_init_valContext *context) {

    auto node = std::make_shared<InitializedNode>(getPositions(context));

    if(context->const_exp()){
        node->setExpr(visitConst_exp(context->const_exp()).as<SmartExpr>());
    }else{
        for(auto& init: context->const_init_val()){
            node->addInitialExp(visitConst_init_val(init).as<InitialNode>());
        }
    }

    return node;
}

antlrcpp::Any NewAstCreator::visitConst_def(ParserParser::Const_defContext *context) {

    if(!context->const_exp().empty()){
        auto array_var = std::make_shared<ArrayVarDef>(getPositions(context), context->Id()->getText());
        array_var->is_const = true;
        for(const auto& exp: context->const_exp()){
            array_var->addArrayDimExpr(visitConst_exp(exp).as<SmartExpr>());
        }
        array_var->setInitialNode(visitConst_init_val(context->const_init_val()).as<InitialNode>());
        return std::dynamic_pointer_cast<Node>(array_var);
    }else{
        auto var = std::make_shared<VarDef>(getPositions(context), context->Id()->getText());
        var->is_const = true;
        if(context->Assign() && context->const_init_val())
            var->setInitialNode(visitConst_exp(context->const_init_val()->const_exp()).as<SmartExpr>());
        return std::dynamic_pointer_cast<Node>(var);
    }
}

antlrcpp::Any NewAstCreator::visitConst_decl(ParserParser::Const_declContext *context) {

    std::vector<SmartNode> nodes{};
    Type type = visitBtype(context->btype()).as<Type>();
    for(auto& def: context->const_def()){
        SmartNode node = visitConst_def(def).as<SmartNode>();
        if(node->type() == ARRAY_VAR_DEF)
            std::dynamic_pointer_cast<ArrayVarDef>(node)->var_type = type;
        else
            std::dynamic_pointer_cast<VarDef>(node)->var_type = type;
        nodes.push_back(node);
    }
    return nodes;

}

antlrcpp::Any NewAstCreator::visitDecl(ParserParser::DeclContext *context) {

    if(context->const_decl())
        return visitConst_decl(context->const_decl());
    else
        return visitVar_decl(context->var_decl());

}

antlrcpp::Any NewAstCreator::visitComp_unit(ParserParser::Comp_unitContext *context) {

    auto node = std::make_shared<CompUnitNode>(getPositions(context));

    int i1 = 0;
    int i2 = 0;
    int len1 = (int)context->func_def_vec.size();
    int len2 = (int)context->decl_vec.size();

    while(i1 < len1 || i2 < len2){
        if(i1 < len1 && i2 < len2){
            if(context->func_def_vec[i1].first < context->decl_vec[i2].first){
                node->addElem(visitFunc_def(context->func_def_vec[i1].second).as<SmartNode>());
                i1++;
            }
            else{
                auto vec = visitDecl(context->decl_vec[i2].second).as<std::vector<SmartNode>>();
                for(auto& elem: vec){
                    node->addElem(elem);
                }
                i2++;
            }
        }
        else if(i1 < len1){
            node->addElem(visitFunc_def(context->func_def_vec[i1].second).as<SmartNode>());
            i1++;
        }
        else if(i2 < len2){
            auto vec = visitDecl(context->decl_vec[i2].second).as<std::vector<SmartNode>>();
            for(auto& elem: vec){
                node->addElem(elem);
            }
            i2++;
        }
    }
    return node;

}

