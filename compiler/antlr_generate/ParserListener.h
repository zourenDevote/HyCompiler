
    #include <vector>
    #include <utility>


// Generated from ./grammarrule/Parser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "ParserParser.h"


/**
 * This interface defines an abstract listener for a parse tree produced by ParserParser.
 */
class  ParserListener : public antlr4::tree::ParseTreeListener {
public:

  virtual void enterComp_unit(ParserParser::Comp_unitContext *ctx) = 0;
  virtual void exitComp_unit(ParserParser::Comp_unitContext *ctx) = 0;

  virtual void enterDecl(ParserParser::DeclContext *ctx) = 0;
  virtual void exitDecl(ParserParser::DeclContext *ctx) = 0;

  virtual void enterConst_decl(ParserParser::Const_declContext *ctx) = 0;
  virtual void exitConst_decl(ParserParser::Const_declContext *ctx) = 0;

  virtual void enterBtype(ParserParser::BtypeContext *ctx) = 0;
  virtual void exitBtype(ParserParser::BtypeContext *ctx) = 0;

  virtual void enterConst_def(ParserParser::Const_defContext *ctx) = 0;
  virtual void exitConst_def(ParserParser::Const_defContext *ctx) = 0;

  virtual void enterConst_init_val(ParserParser::Const_init_valContext *ctx) = 0;
  virtual void exitConst_init_val(ParserParser::Const_init_valContext *ctx) = 0;

  virtual void enterVar_decl(ParserParser::Var_declContext *ctx) = 0;
  virtual void exitVar_decl(ParserParser::Var_declContext *ctx) = 0;

  virtual void enterVar_def(ParserParser::Var_defContext *ctx) = 0;
  virtual void exitVar_def(ParserParser::Var_defContext *ctx) = 0;

  virtual void enterInit_val(ParserParser::Init_valContext *ctx) = 0;
  virtual void exitInit_val(ParserParser::Init_valContext *ctx) = 0;

  virtual void enterFunc_def(ParserParser::Func_defContext *ctx) = 0;
  virtual void exitFunc_def(ParserParser::Func_defContext *ctx) = 0;

  virtual void enterFunc_type(ParserParser::Func_typeContext *ctx) = 0;
  virtual void exitFunc_type(ParserParser::Func_typeContext *ctx) = 0;

  virtual void enterFunc_f_params(ParserParser::Func_f_paramsContext *ctx) = 0;
  virtual void exitFunc_f_params(ParserParser::Func_f_paramsContext *ctx) = 0;

  virtual void enterFunc_f_param(ParserParser::Func_f_paramContext *ctx) = 0;
  virtual void exitFunc_f_param(ParserParser::Func_f_paramContext *ctx) = 0;

  virtual void enterBlock(ParserParser::BlockContext *ctx) = 0;
  virtual void exitBlock(ParserParser::BlockContext *ctx) = 0;

  virtual void enterStmt(ParserParser::StmtContext *ctx) = 0;
  virtual void exitStmt(ParserParser::StmtContext *ctx) = 0;

  virtual void enterExp(ParserParser::ExpContext *ctx) = 0;
  virtual void exitExp(ParserParser::ExpContext *ctx) = 0;

  virtual void enterL_val(ParserParser::L_valContext *ctx) = 0;
  virtual void exitL_val(ParserParser::L_valContext *ctx) = 0;

  virtual void enterPrimary_exp(ParserParser::Primary_expContext *ctx) = 0;
  virtual void exitPrimary_exp(ParserParser::Primary_expContext *ctx) = 0;

  virtual void enterUnary_exp(ParserParser::Unary_expContext *ctx) = 0;
  virtual void exitUnary_exp(ParserParser::Unary_expContext *ctx) = 0;

  virtual void enterUnary_op(ParserParser::Unary_opContext *ctx) = 0;
  virtual void exitUnary_op(ParserParser::Unary_opContext *ctx) = 0;

  virtual void enterFunc_r_params(ParserParser::Func_r_paramsContext *ctx) = 0;
  virtual void exitFunc_r_params(ParserParser::Func_r_paramsContext *ctx) = 0;

  virtual void enterMul_exp(ParserParser::Mul_expContext *ctx) = 0;
  virtual void exitMul_exp(ParserParser::Mul_expContext *ctx) = 0;

  virtual void enterSem_op(ParserParser::Sem_opContext *ctx) = 0;
  virtual void exitSem_op(ParserParser::Sem_opContext *ctx) = 0;

  virtual void enterAdd_exp(ParserParser::Add_expContext *ctx) = 0;
  virtual void exitAdd_exp(ParserParser::Add_expContext *ctx) = 0;

  virtual void enterOp_as(ParserParser::Op_asContext *ctx) = 0;
  virtual void exitOp_as(ParserParser::Op_asContext *ctx) = 0;

  virtual void enterRel_exp(ParserParser::Rel_expContext *ctx) = 0;
  virtual void exitRel_exp(ParserParser::Rel_expContext *ctx) = 0;

  virtual void enterCom_op(ParserParser::Com_opContext *ctx) = 0;
  virtual void exitCom_op(ParserParser::Com_opContext *ctx) = 0;

  virtual void enterEq_exp(ParserParser::Eq_expContext *ctx) = 0;
  virtual void exitEq_exp(ParserParser::Eq_expContext *ctx) = 0;

  virtual void enterCom_op_eqm(ParserParser::Com_op_eqmContext *ctx) = 0;
  virtual void exitCom_op_eqm(ParserParser::Com_op_eqmContext *ctx) = 0;

  virtual void enterL_and_exp(ParserParser::L_and_expContext *ctx) = 0;
  virtual void exitL_and_exp(ParserParser::L_and_expContext *ctx) = 0;

  virtual void enterL_or_exp(ParserParser::L_or_expContext *ctx) = 0;
  virtual void exitL_or_exp(ParserParser::L_or_expContext *ctx) = 0;

  virtual void enterConst_exp(ParserParser::Const_expContext *ctx) = 0;
  virtual void exitConst_exp(ParserParser::Const_expContext *ctx) = 0;


};

