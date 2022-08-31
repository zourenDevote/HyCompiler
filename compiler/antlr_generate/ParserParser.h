
    #include <vector>
    #include <utility>


// Generated from ./grammarrule/Parser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  ParserParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, Int = 5, Float = 6, Void = 7, 
    Const = 8, If = 9, Else = 10, While = 11, Break = 12, Continue = 13, 
    Return = 14, Assign = 15, Lsb = 16, Rsb = 17, SLp = 18, SRp = 19, Add = 20, 
    Sub = 21, Mul = 22, Div = 23, Mod = 24, Equal = 25, Nequal = 26, Gt = 27, 
    Lt = 28, Le = 29, Ge = 30, Not = 31, And = 32, Or = 33, Id = 34, IntConst = 35, 
    FloatConst = 36, Ws = 37, BlockComment = 38, LineComment = 39, Eof = 40
  };

  enum {
    RuleComp_unit = 0, RuleDecl = 1, RuleConst_decl = 2, RuleBtype = 3, 
    RuleConst_def = 4, RuleConst_init_val = 5, RuleVar_decl = 6, RuleVar_def = 7, 
    RuleInit_val = 8, RuleFunc_def = 9, RuleFunc_type = 10, RuleFunc_f_params = 11, 
    RuleFunc_f_param = 12, RuleBlock = 13, RuleStmt = 14, RuleExp = 15, 
    RuleL_val = 16, RulePrimary_exp = 17, RuleUnary_exp = 18, RuleUnary_op = 19, 
    RuleFunc_r_params = 20, RuleMul_exp = 21, RuleSem_op = 22, RuleAdd_exp = 23, 
    RuleOp_as = 24, RuleRel_exp = 25, RuleCom_op = 26, RuleEq_exp = 27, 
    RuleCom_op_eqm = 28, RuleL_and_exp = 29, RuleL_or_exp = 30, RuleConst_exp = 31
  };

  ParserParser(antlr4::TokenStream *input);
  ~ParserParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class Comp_unitContext;
  class DeclContext;
  class Const_declContext;
  class BtypeContext;
  class Const_defContext;
  class Const_init_valContext;
  class Var_declContext;
  class Var_defContext;
  class Init_valContext;
  class Func_defContext;
  class Func_typeContext;
  class Func_f_paramsContext;
  class Func_f_paramContext;
  class BlockContext;
  class StmtContext;
  class ExpContext;
  class L_valContext;
  class Primary_expContext;
  class Unary_expContext;
  class Unary_opContext;
  class Func_r_paramsContext;
  class Mul_expContext;
  class Sem_opContext;
  class Add_expContext;
  class Op_asContext;
  class Rel_expContext;
  class Com_opContext;
  class Eq_expContext;
  class Com_op_eqmContext;
  class L_and_expContext;
  class L_or_expContext;
  class Const_expContext; 

  class  Comp_unitContext : public antlr4::ParserRuleContext {
  public:
    std::vector<std::pair<int, DeclContext*>> decl_vec;
    std::vector<std::pair<int, Func_defContext*>> func_def_vec;
    int i = 0;
    ParserParser::DeclContext *declContext = nullptr;;
    ParserParser::Func_defContext *func_defContext = nullptr;;
    Comp_unitContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<DeclContext *> decl();
    DeclContext* decl(size_t i);
    std::vector<Func_defContext *> func_def();
    Func_defContext* func_def(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Comp_unitContext* comp_unit();

  class  DeclContext : public antlr4::ParserRuleContext {
  public:
    DeclContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Const_declContext *const_decl();
    Var_declContext *var_decl();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DeclContext* decl();

  class  Const_declContext : public antlr4::ParserRuleContext {
  public:
    Const_declContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Const();
    BtypeContext *btype();
    std::vector<Const_defContext *> const_def();
    Const_defContext* const_def(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Const_declContext* const_decl();

  class  BtypeContext : public antlr4::ParserRuleContext {
  public:
    BtypeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Int();
    antlr4::tree::TerminalNode *Float();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BtypeContext* btype();

  class  Const_defContext : public antlr4::ParserRuleContext {
  public:
    Const_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *Assign();
    Const_init_valContext *const_init_val();
    std::vector<antlr4::tree::TerminalNode *> Lsb();
    antlr4::tree::TerminalNode* Lsb(size_t i);
    std::vector<Const_expContext *> const_exp();
    Const_expContext* const_exp(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Rsb();
    antlr4::tree::TerminalNode* Rsb(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Const_defContext* const_def();

  class  Const_init_valContext : public antlr4::ParserRuleContext {
  public:
    Const_init_valContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Const_expContext *const_exp();
    std::vector<Const_init_valContext *> const_init_val();
    Const_init_valContext* const_init_val(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Const_init_valContext* const_init_val();

  class  Var_declContext : public antlr4::ParserRuleContext {
  public:
    Var_declContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BtypeContext *btype();
    std::vector<Var_defContext *> var_def();
    Var_defContext* var_def(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Var_declContext* var_decl();

  class  Var_defContext : public antlr4::ParserRuleContext {
  public:
    Var_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    std::vector<antlr4::tree::TerminalNode *> Lsb();
    antlr4::tree::TerminalNode* Lsb(size_t i);
    std::vector<Const_expContext *> const_exp();
    Const_expContext* const_exp(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Rsb();
    antlr4::tree::TerminalNode* Rsb(size_t i);
    antlr4::tree::TerminalNode *Assign();
    Init_valContext *init_val();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Var_defContext* var_def();

  class  Init_valContext : public antlr4::ParserRuleContext {
  public:
    Init_valContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ExpContext *exp();
    std::vector<Init_valContext *> init_val();
    Init_valContext* init_val(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Init_valContext* init_val();

  class  Func_defContext : public antlr4::ParserRuleContext {
  public:
    Func_defContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Func_typeContext *func_type();
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *SLp();
    antlr4::tree::TerminalNode *SRp();
    BlockContext *block();
    Func_f_paramsContext *func_f_params();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Func_defContext* func_def();

  class  Func_typeContext : public antlr4::ParserRuleContext {
  public:
    Func_typeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Void();
    antlr4::tree::TerminalNode *Int();
    antlr4::tree::TerminalNode *Float();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Func_typeContext* func_type();

  class  Func_f_paramsContext : public antlr4::ParserRuleContext {
  public:
    Func_f_paramsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Func_f_paramContext *> func_f_param();
    Func_f_paramContext* func_f_param(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Func_f_paramsContext* func_f_params();

  class  Func_f_paramContext : public antlr4::ParserRuleContext {
  public:
    Func_f_paramContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    BtypeContext *btype();
    antlr4::tree::TerminalNode *Id();
    std::vector<antlr4::tree::TerminalNode *> Lsb();
    antlr4::tree::TerminalNode* Lsb(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Rsb();
    antlr4::tree::TerminalNode* Rsb(size_t i);
    std::vector<Const_expContext *> const_exp();
    Const_expContext* const_exp(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Func_f_paramContext* func_f_param();

  class  BlockContext : public antlr4::ParserRuleContext {
  public:
    std::vector<std::pair<int, DeclContext*>> decl_vec;
    std::vector<std::pair<int, StmtContext*>> stmt_vec;
    int i = 0;
    ParserParser::DeclContext *declContext = nullptr;;
    ParserParser::StmtContext *stmtContext = nullptr;;
    BlockContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<DeclContext *> decl();
    DeclContext* decl(size_t i);
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlockContext* block();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    L_valContext *l_val();
    antlr4::tree::TerminalNode *Assign();
    ExpContext *exp();
    Add_expContext *add_exp();
    BlockContext *block();
    antlr4::tree::TerminalNode *While();
    antlr4::tree::TerminalNode *SLp();
    L_or_expContext *l_or_exp();
    antlr4::tree::TerminalNode *SRp();
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);
    antlr4::tree::TerminalNode *If();
    antlr4::tree::TerminalNode *Else();
    antlr4::tree::TerminalNode *Break();
    antlr4::tree::TerminalNode *Continue();
    antlr4::tree::TerminalNode *Return();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  StmtContext* stmt();

  class  ExpContext : public antlr4::ParserRuleContext {
  public:
    ExpContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Add_expContext *add_exp();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ExpContext* exp();

  class  L_valContext : public antlr4::ParserRuleContext {
  public:
    L_valContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Id();
    std::vector<antlr4::tree::TerminalNode *> Lsb();
    antlr4::tree::TerminalNode* Lsb(size_t i);
    std::vector<ExpContext *> exp();
    ExpContext* exp(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Rsb();
    antlr4::tree::TerminalNode* Rsb(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  L_valContext* l_val();

  class  Primary_expContext : public antlr4::ParserRuleContext {
  public:
    Primary_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *SLp();
    ExpContext *exp();
    antlr4::tree::TerminalNode *SRp();
    L_valContext *l_val();
    antlr4::tree::TerminalNode *IntConst();
    antlr4::tree::TerminalNode *FloatConst();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Primary_expContext* primary_exp();

  class  Unary_expContext : public antlr4::ParserRuleContext {
  public:
    Unary_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Primary_expContext *primary_exp();
    std::vector<Unary_opContext *> unary_op();
    Unary_opContext* unary_op(size_t i);
    antlr4::tree::TerminalNode *Id();
    antlr4::tree::TerminalNode *SLp();
    antlr4::tree::TerminalNode *SRp();
    Func_r_paramsContext *func_r_params();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Unary_expContext* unary_exp();

  class  Unary_opContext : public antlr4::ParserRuleContext {
  public:
    Unary_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Add();
    antlr4::tree::TerminalNode *Sub();
    antlr4::tree::TerminalNode *Not();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Unary_opContext* unary_op();

  class  Func_r_paramsContext : public antlr4::ParserRuleContext {
  public:
    Func_r_paramsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<ExpContext *> exp();
    ExpContext* exp(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Func_r_paramsContext* func_r_params();

  class  Mul_expContext : public antlr4::ParserRuleContext {
  public:
    Mul_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Unary_expContext *> unary_exp();
    Unary_expContext* unary_exp(size_t i);
    std::vector<Sem_opContext *> sem_op();
    Sem_opContext* sem_op(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Mul_expContext* mul_exp();

  class  Sem_opContext : public antlr4::ParserRuleContext {
  public:
    Sem_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Mul();
    antlr4::tree::TerminalNode *Div();
    antlr4::tree::TerminalNode *Mod();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Sem_opContext* sem_op();

  class  Add_expContext : public antlr4::ParserRuleContext {
  public:
    Add_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Mul_expContext *> mul_exp();
    Mul_expContext* mul_exp(size_t i);
    std::vector<Op_asContext *> op_as();
    Op_asContext* op_as(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Add_expContext* add_exp();

  class  Op_asContext : public antlr4::ParserRuleContext {
  public:
    Op_asContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Add();
    antlr4::tree::TerminalNode *Sub();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Op_asContext* op_as();

  class  Rel_expContext : public antlr4::ParserRuleContext {
  public:
    Rel_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Add_expContext *> add_exp();
    Add_expContext* add_exp(size_t i);
    std::vector<Com_opContext *> com_op();
    Com_opContext* com_op(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Rel_expContext* rel_exp();

  class  Com_opContext : public antlr4::ParserRuleContext {
  public:
    Com_opContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Gt();
    antlr4::tree::TerminalNode *Lt();
    antlr4::tree::TerminalNode *Le();
    antlr4::tree::TerminalNode *Ge();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Com_opContext* com_op();

  class  Eq_expContext : public antlr4::ParserRuleContext {
  public:
    Eq_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Rel_expContext *> rel_exp();
    Rel_expContext* rel_exp(size_t i);
    std::vector<Com_op_eqmContext *> com_op_eqm();
    Com_op_eqmContext* com_op_eqm(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Eq_expContext* eq_exp();

  class  Com_op_eqmContext : public antlr4::ParserRuleContext {
  public:
    Com_op_eqmContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *Equal();
    antlr4::tree::TerminalNode *Nequal();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Com_op_eqmContext* com_op_eqm();

  class  L_and_expContext : public antlr4::ParserRuleContext {
  public:
    L_and_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<Eq_expContext *> eq_exp();
    Eq_expContext* eq_exp(size_t i);
    std::vector<antlr4::tree::TerminalNode *> And();
    antlr4::tree::TerminalNode* And(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  L_and_expContext* l_and_exp();

  class  L_or_expContext : public antlr4::ParserRuleContext {
  public:
    L_or_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<L_and_expContext *> l_and_exp();
    L_and_expContext* l_and_exp(size_t i);
    std::vector<antlr4::tree::TerminalNode *> Or();
    antlr4::tree::TerminalNode* Or(size_t i);

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  L_or_expContext* l_or_exp();

  class  Const_expContext : public antlr4::ParserRuleContext {
  public:
    Const_expContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    Add_expContext *add_exp();

    virtual void enterRule(antlr4::tree::ParseTreeListener *listener) override;
    virtual void exitRule(antlr4::tree::ParseTreeListener *listener) override;

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Const_expContext* const_exp();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

