
    #include <vector>
    #include <utility>


// Generated from ./grammarrule/Parser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"
#include "ParserVisitor.h"


/**
 * This class provides an empty implementation of ParserVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ParserBaseVisitor : public ParserVisitor {
public:

  virtual antlrcpp::Any visitComp_unit(ParserParser::Comp_unitContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDecl(ParserParser::DeclContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConst_decl(ParserParser::Const_declContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBtype(ParserParser::BtypeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConst_def(ParserParser::Const_defContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConst_init_val(ParserParser::Const_init_valContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVar_decl(ParserParser::Var_declContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitVar_def(ParserParser::Var_defContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitInit_val(ParserParser::Init_valContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunc_def(ParserParser::Func_defContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunc_type(ParserParser::Func_typeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunc_f_params(ParserParser::Func_f_paramsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunc_f_param(ParserParser::Func_f_paramContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitBlock(ParserParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitStmt(ParserParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitExp(ParserParser::ExpContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitL_val(ParserParser::L_valContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitPrimary_exp(ParserParser::Primary_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_exp(ParserParser::Unary_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnary_op(ParserParser::Unary_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitFunc_r_params(ParserParser::Func_r_paramsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMul_exp(ParserParser::Mul_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSem_op(ParserParser::Sem_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitAdd_exp(ParserParser::Add_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitOp_as(ParserParser::Op_asContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRel_exp(ParserParser::Rel_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCom_op(ParserParser::Com_opContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitEq_exp(ParserParser::Eq_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitCom_op_eqm(ParserParser::Com_op_eqmContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitL_and_exp(ParserParser::L_and_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitL_or_exp(ParserParser::L_or_expContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitConst_exp(ParserParser::Const_expContext *ctx) override {
    return visitChildren(ctx);
  }


};

