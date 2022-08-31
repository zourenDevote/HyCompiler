
    #include <vector>
    #include <utility>


// Generated from ./grammarrule/Parser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "ParserListener.h"


/**
 * This class provides an empty implementation of ParserListener,
 * which can be extended to create a listener which only needs to handle a subset
 * of the available methods.
 */
class  ParserBaseListener : public ParserListener {
public:

  virtual void enterComp_unit(ParserParser::Comp_unitContext * /*ctx*/) override { }
  virtual void exitComp_unit(ParserParser::Comp_unitContext * /*ctx*/) override { }

  virtual void enterDecl(ParserParser::DeclContext * /*ctx*/) override { }
  virtual void exitDecl(ParserParser::DeclContext * /*ctx*/) override { }

  virtual void enterConst_decl(ParserParser::Const_declContext * /*ctx*/) override { }
  virtual void exitConst_decl(ParserParser::Const_declContext * /*ctx*/) override { }

  virtual void enterBtype(ParserParser::BtypeContext * /*ctx*/) override { }
  virtual void exitBtype(ParserParser::BtypeContext * /*ctx*/) override { }

  virtual void enterConst_def(ParserParser::Const_defContext * /*ctx*/) override { }
  virtual void exitConst_def(ParserParser::Const_defContext * /*ctx*/) override { }

  virtual void enterConst_init_val(ParserParser::Const_init_valContext * /*ctx*/) override { }
  virtual void exitConst_init_val(ParserParser::Const_init_valContext * /*ctx*/) override { }

  virtual void enterVar_decl(ParserParser::Var_declContext * /*ctx*/) override { }
  virtual void exitVar_decl(ParserParser::Var_declContext * /*ctx*/) override { }

  virtual void enterVar_def(ParserParser::Var_defContext * /*ctx*/) override { }
  virtual void exitVar_def(ParserParser::Var_defContext * /*ctx*/) override { }

  virtual void enterInit_val(ParserParser::Init_valContext * /*ctx*/) override { }
  virtual void exitInit_val(ParserParser::Init_valContext * /*ctx*/) override { }

  virtual void enterFunc_def(ParserParser::Func_defContext * /*ctx*/) override { }
  virtual void exitFunc_def(ParserParser::Func_defContext * /*ctx*/) override { }

  virtual void enterFunc_type(ParserParser::Func_typeContext * /*ctx*/) override { }
  virtual void exitFunc_type(ParserParser::Func_typeContext * /*ctx*/) override { }

  virtual void enterFunc_f_params(ParserParser::Func_f_paramsContext * /*ctx*/) override { }
  virtual void exitFunc_f_params(ParserParser::Func_f_paramsContext * /*ctx*/) override { }

  virtual void enterFunc_f_param(ParserParser::Func_f_paramContext * /*ctx*/) override { }
  virtual void exitFunc_f_param(ParserParser::Func_f_paramContext * /*ctx*/) override { }

  virtual void enterBlock(ParserParser::BlockContext * /*ctx*/) override { }
  virtual void exitBlock(ParserParser::BlockContext * /*ctx*/) override { }

  virtual void enterStmt(ParserParser::StmtContext * /*ctx*/) override { }
  virtual void exitStmt(ParserParser::StmtContext * /*ctx*/) override { }

  virtual void enterExp(ParserParser::ExpContext * /*ctx*/) override { }
  virtual void exitExp(ParserParser::ExpContext * /*ctx*/) override { }

  virtual void enterL_val(ParserParser::L_valContext * /*ctx*/) override { }
  virtual void exitL_val(ParserParser::L_valContext * /*ctx*/) override { }

  virtual void enterPrimary_exp(ParserParser::Primary_expContext * /*ctx*/) override { }
  virtual void exitPrimary_exp(ParserParser::Primary_expContext * /*ctx*/) override { }

  virtual void enterUnary_exp(ParserParser::Unary_expContext * /*ctx*/) override { }
  virtual void exitUnary_exp(ParserParser::Unary_expContext * /*ctx*/) override { }

  virtual void enterUnary_op(ParserParser::Unary_opContext * /*ctx*/) override { }
  virtual void exitUnary_op(ParserParser::Unary_opContext * /*ctx*/) override { }

  virtual void enterFunc_r_params(ParserParser::Func_r_paramsContext * /*ctx*/) override { }
  virtual void exitFunc_r_params(ParserParser::Func_r_paramsContext * /*ctx*/) override { }

  virtual void enterMul_exp(ParserParser::Mul_expContext * /*ctx*/) override { }
  virtual void exitMul_exp(ParserParser::Mul_expContext * /*ctx*/) override { }

  virtual void enterSem_op(ParserParser::Sem_opContext * /*ctx*/) override { }
  virtual void exitSem_op(ParserParser::Sem_opContext * /*ctx*/) override { }

  virtual void enterAdd_exp(ParserParser::Add_expContext * /*ctx*/) override { }
  virtual void exitAdd_exp(ParserParser::Add_expContext * /*ctx*/) override { }

  virtual void enterOp_as(ParserParser::Op_asContext * /*ctx*/) override { }
  virtual void exitOp_as(ParserParser::Op_asContext * /*ctx*/) override { }

  virtual void enterRel_exp(ParserParser::Rel_expContext * /*ctx*/) override { }
  virtual void exitRel_exp(ParserParser::Rel_expContext * /*ctx*/) override { }

  virtual void enterCom_op(ParserParser::Com_opContext * /*ctx*/) override { }
  virtual void exitCom_op(ParserParser::Com_opContext * /*ctx*/) override { }

  virtual void enterEq_exp(ParserParser::Eq_expContext * /*ctx*/) override { }
  virtual void exitEq_exp(ParserParser::Eq_expContext * /*ctx*/) override { }

  virtual void enterCom_op_eqm(ParserParser::Com_op_eqmContext * /*ctx*/) override { }
  virtual void exitCom_op_eqm(ParserParser::Com_op_eqmContext * /*ctx*/) override { }

  virtual void enterL_and_exp(ParserParser::L_and_expContext * /*ctx*/) override { }
  virtual void exitL_and_exp(ParserParser::L_and_expContext * /*ctx*/) override { }

  virtual void enterL_or_exp(ParserParser::L_or_expContext * /*ctx*/) override { }
  virtual void exitL_or_exp(ParserParser::L_or_expContext * /*ctx*/) override { }

  virtual void enterConst_exp(ParserParser::Const_expContext * /*ctx*/) override { }
  virtual void exitConst_exp(ParserParser::Const_expContext * /*ctx*/) override { }


  virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override { }
  virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override { }
  virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override { }

};

