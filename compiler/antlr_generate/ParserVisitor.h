
    #include <vector>
    #include <utility>


// Generated from ./grammarrule/Parser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "ParserParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ParserParser.
 */
class  ParserVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ParserParser.
   */
    virtual antlrcpp::Any visitComp_unit(ParserParser::Comp_unitContext *context) = 0;

    virtual antlrcpp::Any visitDecl(ParserParser::DeclContext *context) = 0;

    virtual antlrcpp::Any visitConst_decl(ParserParser::Const_declContext *context) = 0;

    virtual antlrcpp::Any visitBtype(ParserParser::BtypeContext *context) = 0;

    virtual antlrcpp::Any visitConst_def(ParserParser::Const_defContext *context) = 0;

    virtual antlrcpp::Any visitConst_init_val(ParserParser::Const_init_valContext *context) = 0;

    virtual antlrcpp::Any visitVar_decl(ParserParser::Var_declContext *context) = 0;

    virtual antlrcpp::Any visitVar_def(ParserParser::Var_defContext *context) = 0;

    virtual antlrcpp::Any visitInit_val(ParserParser::Init_valContext *context) = 0;

    virtual antlrcpp::Any visitFunc_def(ParserParser::Func_defContext *context) = 0;

    virtual antlrcpp::Any visitFunc_type(ParserParser::Func_typeContext *context) = 0;

    virtual antlrcpp::Any visitFunc_f_params(ParserParser::Func_f_paramsContext *context) = 0;

    virtual antlrcpp::Any visitFunc_f_param(ParserParser::Func_f_paramContext *context) = 0;

    virtual antlrcpp::Any visitBlock(ParserParser::BlockContext *context) = 0;

    virtual antlrcpp::Any visitStmt(ParserParser::StmtContext *context) = 0;

    virtual antlrcpp::Any visitExp(ParserParser::ExpContext *context) = 0;

    virtual antlrcpp::Any visitL_val(ParserParser::L_valContext *context) = 0;

    virtual antlrcpp::Any visitPrimary_exp(ParserParser::Primary_expContext *context) = 0;

    virtual antlrcpp::Any visitUnary_exp(ParserParser::Unary_expContext *context) = 0;

    virtual antlrcpp::Any visitUnary_op(ParserParser::Unary_opContext *context) = 0;

    virtual antlrcpp::Any visitFunc_r_params(ParserParser::Func_r_paramsContext *context) = 0;

    virtual antlrcpp::Any visitMul_exp(ParserParser::Mul_expContext *context) = 0;

    virtual antlrcpp::Any visitSem_op(ParserParser::Sem_opContext *context) = 0;

    virtual antlrcpp::Any visitAdd_exp(ParserParser::Add_expContext *context) = 0;

    virtual antlrcpp::Any visitOp_as(ParserParser::Op_asContext *context) = 0;

    virtual antlrcpp::Any visitRel_exp(ParserParser::Rel_expContext *context) = 0;

    virtual antlrcpp::Any visitCom_op(ParserParser::Com_opContext *context) = 0;

    virtual antlrcpp::Any visitEq_exp(ParserParser::Eq_expContext *context) = 0;

    virtual antlrcpp::Any visitCom_op_eqm(ParserParser::Com_op_eqmContext *context) = 0;

    virtual antlrcpp::Any visitL_and_exp(ParserParser::L_and_expContext *context) = 0;

    virtual antlrcpp::Any visitL_or_exp(ParserParser::L_or_expContext *context) = 0;

    virtual antlrcpp::Any visitConst_exp(ParserParser::Const_expContext *context) = 0;


};

