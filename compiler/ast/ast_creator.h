/**
 * Author:  Devotes
 * Date:    2022/5/29
 *
 * FixLog:
 *
 */

#ifndef COMPILER_AST_CREATOR_H
#define COMPILER_AST_CREATOR_H

#include "../antlr_generate/ParserVisitor.h"
#include "ast.h"


class NewAstCreator: public ParserVisitor{

public:
    NewAstCreator() = default;

    antlrcpp::Any       visitComp_unit(ParserParser::Comp_unitContext *context)                 override;
    antlrcpp::Any       visitDecl(ParserParser::DeclContext *context)                           override;
    antlrcpp::Any       visitFunc_def(ParserParser::Func_defContext *context)                   override;
    antlrcpp::Any       visitConst_def(ParserParser::Const_defContext *context)                 override;
    antlrcpp::Any       visitVar_def(ParserParser::Var_defContext *context)                     override;
    antlrcpp::Any       visitConst_decl(ParserParser::Const_declContext *context)               override;
    antlrcpp::Any       visitBtype(ParserParser::BtypeContext *context)                         override;
    antlrcpp::Any       visitConst_init_val(ParserParser::Const_init_valContext *context)       override;
    antlrcpp::Any       visitConst_exp(ParserParser::Const_expContext *context)                 override;
    antlrcpp::Any       visitVar_decl(ParserParser::Var_declContext *context)                   override;
    antlrcpp::Any       visitInit_val(ParserParser::Init_valContext *context)                   override;
    antlrcpp::Any       visitExp(ParserParser::ExpContext *context)                             override;
    antlrcpp::Any       visitFunc_type(ParserParser::Func_typeContext *context)                 override;
    antlrcpp::Any       visitFunc_f_params(ParserParser::Func_f_paramsContext *context)         override;
    antlrcpp::Any       visitFunc_f_param(ParserParser::Func_f_paramContext *context)           override;
    antlrcpp::Any       visitBlock(ParserParser::BlockContext *context)                         override;
    antlrcpp::Any       visitStmt(ParserParser::StmtContext *context)                           override;
    antlrcpp::Any       visitL_val(ParserParser::L_valContext *context)                         override;
    antlrcpp::Any       visitAdd_exp(ParserParser::Add_expContext *context)                     override;
    antlrcpp::Any       visitOp_as(ParserParser::Op_asContext *context)                         override;
    antlrcpp::Any       visitCom_op_eqm(ParserParser::Com_op_eqmContext *context)               override;
    antlrcpp::Any       visitCom_op(ParserParser::Com_opContext *context)                       override;
    antlrcpp::Any       visitSem_op(ParserParser::Sem_opContext *context)                       override;
    antlrcpp::Any       visitL_or_exp(ParserParser::L_or_expContext *context)                   override;
    antlrcpp::Any       visitL_and_exp(ParserParser::L_and_expContext *context)                 override;
    antlrcpp::Any       visitEq_exp(ParserParser::Eq_expContext *context)                       override;
    antlrcpp::Any       visitRel_exp(ParserParser::Rel_expContext *context)                     override;
    antlrcpp::Any       visitMul_exp(ParserParser::Mul_expContext *context)                     override;
    antlrcpp::Any       visitFunc_r_params(ParserParser::Func_r_paramsContext *context)         override;
    antlrcpp::Any       visitUnary_op(ParserParser::Unary_opContext *context)                   override;
    antlrcpp::Any       visitUnary_exp(ParserParser::Unary_expContext *context)                 override;
    antlrcpp::Any       visitPrimary_exp(ParserParser::Primary_expContext *context)             override;

};

#endif //COMPILER_AST_CREATOR_H
