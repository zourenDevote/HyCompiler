
    #include <vector>
    #include <utility>


// Generated from ./grammarrule/Parser.g4 by ANTLR 4.8


#include "ParserListener.h"
#include "ParserVisitor.h"

#include "ParserParser.h"


using namespace antlrcpp;
using namespace antlr4;

ParserParser::ParserParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

ParserParser::~ParserParser() {
  delete _interpreter;
}

std::string ParserParser::getGrammarFileName() const {
  return "Parser.g4";
}

const std::vector<std::string>& ParserParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& ParserParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- Comp_unitContext ------------------------------------------------------------------

ParserParser::Comp_unitContext::Comp_unitContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Comp_unitContext::EOF() {
  return getToken(ParserParser::EOF, 0);
}

std::vector<ParserParser::DeclContext *> ParserParser::Comp_unitContext::decl() {
  return getRuleContexts<ParserParser::DeclContext>();
}

ParserParser::DeclContext* ParserParser::Comp_unitContext::decl(size_t i) {
  return getRuleContext<ParserParser::DeclContext>(i);
}

std::vector<ParserParser::Func_defContext *> ParserParser::Comp_unitContext::func_def() {
  return getRuleContexts<ParserParser::Func_defContext>();
}

ParserParser::Func_defContext* ParserParser::Comp_unitContext::func_def(size_t i) {
  return getRuleContext<ParserParser::Func_defContext>(i);
}


size_t ParserParser::Comp_unitContext::getRuleIndex() const {
  return ParserParser::RuleComp_unit;
}

void ParserParser::Comp_unitContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterComp_unit(this);
}

void ParserParser::Comp_unitContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitComp_unit(this);
}


antlrcpp::Any ParserParser::Comp_unitContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitComp_unit(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Comp_unitContext* ParserParser::comp_unit() {
  Comp_unitContext *_localctx = _tracker.createInstance<Comp_unitContext>(_ctx, getState());
  enterRule(_localctx, 0, ParserParser::RuleComp_unit);

              dynamic_cast<Comp_unitContext *>(_localctx)->decl_vec =  std::vector<std::pair<int, DeclContext*>>();
              dynamic_cast<Comp_unitContext *>(_localctx)->func_def_vec =  std::vector<std::pair<int, Func_defContext*>>();
          
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::Int)
      | (1ULL << ParserParser::Float)
      | (1ULL << ParserParser::Void)
      | (1ULL << ParserParser::Const))) != 0)) {
      setState(70);
      _errHandler->sync(this);
      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 0, _ctx)) {
      case 1: {
        setState(64);
        dynamic_cast<Comp_unitContext *>(_localctx)->declContext = decl();
        _localctx->decl_vec.push_back({_localctx->i, dynamic_cast<Comp_unitContext *>(_localctx)->declContext}); _localctx->i++;
        break;
      }

      case 2: {
        setState(67);
        dynamic_cast<Comp_unitContext *>(_localctx)->func_defContext = func_def();
        _localctx->func_def_vec.push_back({_localctx->i, dynamic_cast<Comp_unitContext *>(_localctx)->func_defContext}); _localctx->i++;
        break;
      }

      }
      setState(74);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(75);
    match(ParserParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclContext ------------------------------------------------------------------

ParserParser::DeclContext::DeclContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::Const_declContext* ParserParser::DeclContext::const_decl() {
  return getRuleContext<ParserParser::Const_declContext>(0);
}

ParserParser::Var_declContext* ParserParser::DeclContext::var_decl() {
  return getRuleContext<ParserParser::Var_declContext>(0);
}


size_t ParserParser::DeclContext::getRuleIndex() const {
  return ParserParser::RuleDecl;
}

void ParserParser::DeclContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDecl(this);
}

void ParserParser::DeclContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDecl(this);
}


antlrcpp::Any ParserParser::DeclContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitDecl(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::DeclContext* ParserParser::decl() {
  DeclContext *_localctx = _tracker.createInstance<DeclContext>(_ctx, getState());
  enterRule(_localctx, 2, ParserParser::RuleDecl);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(79);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ParserParser::Const: {
        enterOuterAlt(_localctx, 1);
        setState(77);
        const_decl();
        break;
      }

      case ParserParser::Int:
      case ParserParser::Float: {
        enterOuterAlt(_localctx, 2);
        setState(78);
        var_decl();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Const_declContext ------------------------------------------------------------------

ParserParser::Const_declContext::Const_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Const_declContext::Const() {
  return getToken(ParserParser::Const, 0);
}

ParserParser::BtypeContext* ParserParser::Const_declContext::btype() {
  return getRuleContext<ParserParser::BtypeContext>(0);
}

std::vector<ParserParser::Const_defContext *> ParserParser::Const_declContext::const_def() {
  return getRuleContexts<ParserParser::Const_defContext>();
}

ParserParser::Const_defContext* ParserParser::Const_declContext::const_def(size_t i) {
  return getRuleContext<ParserParser::Const_defContext>(i);
}


size_t ParserParser::Const_declContext::getRuleIndex() const {
  return ParserParser::RuleConst_decl;
}

void ParserParser::Const_declContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConst_decl(this);
}

void ParserParser::Const_declContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConst_decl(this);
}


antlrcpp::Any ParserParser::Const_declContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitConst_decl(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Const_declContext* ParserParser::const_decl() {
  Const_declContext *_localctx = _tracker.createInstance<Const_declContext>(_ctx, getState());
  enterRule(_localctx, 4, ParserParser::RuleConst_decl);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(81);
    match(ParserParser::Const);
    setState(82);
    btype();
    setState(83);
    const_def();
    setState(88);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::T__0) {
      setState(84);
      match(ParserParser::T__0);
      setState(85);
      const_def();
      setState(90);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(91);
    match(ParserParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BtypeContext ------------------------------------------------------------------

ParserParser::BtypeContext::BtypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::BtypeContext::Int() {
  return getToken(ParserParser::Int, 0);
}

tree::TerminalNode* ParserParser::BtypeContext::Float() {
  return getToken(ParserParser::Float, 0);
}


size_t ParserParser::BtypeContext::getRuleIndex() const {
  return ParserParser::RuleBtype;
}

void ParserParser::BtypeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBtype(this);
}

void ParserParser::BtypeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBtype(this);
}


antlrcpp::Any ParserParser::BtypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitBtype(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::BtypeContext* ParserParser::btype() {
  BtypeContext *_localctx = _tracker.createInstance<BtypeContext>(_ctx, getState());
  enterRule(_localctx, 6, ParserParser::RuleBtype);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(93);
    _la = _input->LA(1);
    if (!(_la == ParserParser::Int

    || _la == ParserParser::Float)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Const_defContext ------------------------------------------------------------------

ParserParser::Const_defContext::Const_defContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Const_defContext::Id() {
  return getToken(ParserParser::Id, 0);
}

tree::TerminalNode* ParserParser::Const_defContext::Assign() {
  return getToken(ParserParser::Assign, 0);
}

ParserParser::Const_init_valContext* ParserParser::Const_defContext::const_init_val() {
  return getRuleContext<ParserParser::Const_init_valContext>(0);
}

std::vector<tree::TerminalNode *> ParserParser::Const_defContext::Lsb() {
  return getTokens(ParserParser::Lsb);
}

tree::TerminalNode* ParserParser::Const_defContext::Lsb(size_t i) {
  return getToken(ParserParser::Lsb, i);
}

std::vector<ParserParser::Const_expContext *> ParserParser::Const_defContext::const_exp() {
  return getRuleContexts<ParserParser::Const_expContext>();
}

ParserParser::Const_expContext* ParserParser::Const_defContext::const_exp(size_t i) {
  return getRuleContext<ParserParser::Const_expContext>(i);
}

std::vector<tree::TerminalNode *> ParserParser::Const_defContext::Rsb() {
  return getTokens(ParserParser::Rsb);
}

tree::TerminalNode* ParserParser::Const_defContext::Rsb(size_t i) {
  return getToken(ParserParser::Rsb, i);
}


size_t ParserParser::Const_defContext::getRuleIndex() const {
  return ParserParser::RuleConst_def;
}

void ParserParser::Const_defContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConst_def(this);
}

void ParserParser::Const_defContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConst_def(this);
}


antlrcpp::Any ParserParser::Const_defContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitConst_def(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Const_defContext* ParserParser::const_def() {
  Const_defContext *_localctx = _tracker.createInstance<Const_defContext>(_ctx, getState());
  enterRule(_localctx, 8, ParserParser::RuleConst_def);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(95);
    match(ParserParser::Id);
    setState(102);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::Lsb) {
      setState(96);
      match(ParserParser::Lsb);
      setState(97);
      const_exp();
      setState(98);
      match(ParserParser::Rsb);
      setState(104);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(105);
    match(ParserParser::Assign);
    setState(106);
    const_init_val();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Const_init_valContext ------------------------------------------------------------------

ParserParser::Const_init_valContext::Const_init_valContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::Const_expContext* ParserParser::Const_init_valContext::const_exp() {
  return getRuleContext<ParserParser::Const_expContext>(0);
}

std::vector<ParserParser::Const_init_valContext *> ParserParser::Const_init_valContext::const_init_val() {
  return getRuleContexts<ParserParser::Const_init_valContext>();
}

ParserParser::Const_init_valContext* ParserParser::Const_init_valContext::const_init_val(size_t i) {
  return getRuleContext<ParserParser::Const_init_valContext>(i);
}


size_t ParserParser::Const_init_valContext::getRuleIndex() const {
  return ParserParser::RuleConst_init_val;
}

void ParserParser::Const_init_valContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConst_init_val(this);
}

void ParserParser::Const_init_valContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConst_init_val(this);
}


antlrcpp::Any ParserParser::Const_init_valContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitConst_init_val(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Const_init_valContext* ParserParser::const_init_val() {
  Const_init_valContext *_localctx = _tracker.createInstance<Const_init_valContext>(_ctx, getState());
  enterRule(_localctx, 10, ParserParser::RuleConst_init_val);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(121);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ParserParser::SLp:
      case ParserParser::Add:
      case ParserParser::Sub:
      case ParserParser::Not:
      case ParserParser::Id:
      case ParserParser::IntConst:
      case ParserParser::FloatConst: {
        enterOuterAlt(_localctx, 1);
        setState(108);
        const_exp();
        break;
      }

      case ParserParser::T__2: {
        enterOuterAlt(_localctx, 2);
        setState(109);
        match(ParserParser::T__2);
        setState(118);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << ParserParser::T__2)
          | (1ULL << ParserParser::SLp)
          | (1ULL << ParserParser::Add)
          | (1ULL << ParserParser::Sub)
          | (1ULL << ParserParser::Not)
          | (1ULL << ParserParser::Id)
          | (1ULL << ParserParser::IntConst)
          | (1ULL << ParserParser::FloatConst))) != 0)) {
          setState(110);
          const_init_val();
          setState(115);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == ParserParser::T__0) {
            setState(111);
            match(ParserParser::T__0);
            setState(112);
            const_init_val();
            setState(117);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(120);
        match(ParserParser::T__3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Var_declContext ------------------------------------------------------------------

ParserParser::Var_declContext::Var_declContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::BtypeContext* ParserParser::Var_declContext::btype() {
  return getRuleContext<ParserParser::BtypeContext>(0);
}

std::vector<ParserParser::Var_defContext *> ParserParser::Var_declContext::var_def() {
  return getRuleContexts<ParserParser::Var_defContext>();
}

ParserParser::Var_defContext* ParserParser::Var_declContext::var_def(size_t i) {
  return getRuleContext<ParserParser::Var_defContext>(i);
}


size_t ParserParser::Var_declContext::getRuleIndex() const {
  return ParserParser::RuleVar_decl;
}

void ParserParser::Var_declContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVar_decl(this);
}

void ParserParser::Var_declContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVar_decl(this);
}


antlrcpp::Any ParserParser::Var_declContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitVar_decl(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Var_declContext* ParserParser::var_decl() {
  Var_declContext *_localctx = _tracker.createInstance<Var_declContext>(_ctx, getState());
  enterRule(_localctx, 12, ParserParser::RuleVar_decl);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(123);
    btype();
    setState(124);
    var_def();
    setState(129);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::T__0) {
      setState(125);
      match(ParserParser::T__0);
      setState(126);
      var_def();
      setState(131);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(132);
    match(ParserParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Var_defContext ------------------------------------------------------------------

ParserParser::Var_defContext::Var_defContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Var_defContext::Id() {
  return getToken(ParserParser::Id, 0);
}

std::vector<tree::TerminalNode *> ParserParser::Var_defContext::Lsb() {
  return getTokens(ParserParser::Lsb);
}

tree::TerminalNode* ParserParser::Var_defContext::Lsb(size_t i) {
  return getToken(ParserParser::Lsb, i);
}

std::vector<ParserParser::Const_expContext *> ParserParser::Var_defContext::const_exp() {
  return getRuleContexts<ParserParser::Const_expContext>();
}

ParserParser::Const_expContext* ParserParser::Var_defContext::const_exp(size_t i) {
  return getRuleContext<ParserParser::Const_expContext>(i);
}

std::vector<tree::TerminalNode *> ParserParser::Var_defContext::Rsb() {
  return getTokens(ParserParser::Rsb);
}

tree::TerminalNode* ParserParser::Var_defContext::Rsb(size_t i) {
  return getToken(ParserParser::Rsb, i);
}

tree::TerminalNode* ParserParser::Var_defContext::Assign() {
  return getToken(ParserParser::Assign, 0);
}

ParserParser::Init_valContext* ParserParser::Var_defContext::init_val() {
  return getRuleContext<ParserParser::Init_valContext>(0);
}


size_t ParserParser::Var_defContext::getRuleIndex() const {
  return ParserParser::RuleVar_def;
}

void ParserParser::Var_defContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVar_def(this);
}

void ParserParser::Var_defContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVar_def(this);
}


antlrcpp::Any ParserParser::Var_defContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitVar_def(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Var_defContext* ParserParser::var_def() {
  Var_defContext *_localctx = _tracker.createInstance<Var_defContext>(_ctx, getState());
  enterRule(_localctx, 14, ParserParser::RuleVar_def);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(156);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(134);
      match(ParserParser::Id);
      setState(141);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ParserParser::Lsb) {
        setState(135);
        match(ParserParser::Lsb);
        setState(136);
        const_exp();
        setState(137);
        match(ParserParser::Rsb);
        setState(143);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(144);
      match(ParserParser::Id);
      setState(151);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ParserParser::Lsb) {
        setState(145);
        match(ParserParser::Lsb);
        setState(146);
        const_exp();
        setState(147);
        match(ParserParser::Rsb);
        setState(153);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(154);
      match(ParserParser::Assign);
      setState(155);
      init_val();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Init_valContext ------------------------------------------------------------------

ParserParser::Init_valContext::Init_valContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::ExpContext* ParserParser::Init_valContext::exp() {
  return getRuleContext<ParserParser::ExpContext>(0);
}

std::vector<ParserParser::Init_valContext *> ParserParser::Init_valContext::init_val() {
  return getRuleContexts<ParserParser::Init_valContext>();
}

ParserParser::Init_valContext* ParserParser::Init_valContext::init_val(size_t i) {
  return getRuleContext<ParserParser::Init_valContext>(i);
}


size_t ParserParser::Init_valContext::getRuleIndex() const {
  return ParserParser::RuleInit_val;
}

void ParserParser::Init_valContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterInit_val(this);
}

void ParserParser::Init_valContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitInit_val(this);
}


antlrcpp::Any ParserParser::Init_valContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitInit_val(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Init_valContext* ParserParser::init_val() {
  Init_valContext *_localctx = _tracker.createInstance<Init_valContext>(_ctx, getState());
  enterRule(_localctx, 16, ParserParser::RuleInit_val);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(171);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ParserParser::SLp:
      case ParserParser::Add:
      case ParserParser::Sub:
      case ParserParser::Not:
      case ParserParser::Id:
      case ParserParser::IntConst:
      case ParserParser::FloatConst: {
        enterOuterAlt(_localctx, 1);
        setState(158);
        exp();
        break;
      }

      case ParserParser::T__2: {
        enterOuterAlt(_localctx, 2);
        setState(159);
        match(ParserParser::T__2);
        setState(168);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << ParserParser::T__2)
          | (1ULL << ParserParser::SLp)
          | (1ULL << ParserParser::Add)
          | (1ULL << ParserParser::Sub)
          | (1ULL << ParserParser::Not)
          | (1ULL << ParserParser::Id)
          | (1ULL << ParserParser::IntConst)
          | (1ULL << ParserParser::FloatConst))) != 0)) {
          setState(160);
          init_val();
          setState(165);
          _errHandler->sync(this);
          _la = _input->LA(1);
          while (_la == ParserParser::T__0) {
            setState(161);
            match(ParserParser::T__0);
            setState(162);
            init_val();
            setState(167);
            _errHandler->sync(this);
            _la = _input->LA(1);
          }
        }
        setState(170);
        match(ParserParser::T__3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Func_defContext ------------------------------------------------------------------

ParserParser::Func_defContext::Func_defContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::Func_typeContext* ParserParser::Func_defContext::func_type() {
  return getRuleContext<ParserParser::Func_typeContext>(0);
}

tree::TerminalNode* ParserParser::Func_defContext::Id() {
  return getToken(ParserParser::Id, 0);
}

tree::TerminalNode* ParserParser::Func_defContext::SLp() {
  return getToken(ParserParser::SLp, 0);
}

tree::TerminalNode* ParserParser::Func_defContext::SRp() {
  return getToken(ParserParser::SRp, 0);
}

ParserParser::BlockContext* ParserParser::Func_defContext::block() {
  return getRuleContext<ParserParser::BlockContext>(0);
}

ParserParser::Func_f_paramsContext* ParserParser::Func_defContext::func_f_params() {
  return getRuleContext<ParserParser::Func_f_paramsContext>(0);
}


size_t ParserParser::Func_defContext::getRuleIndex() const {
  return ParserParser::RuleFunc_def;
}

void ParserParser::Func_defContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunc_def(this);
}

void ParserParser::Func_defContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunc_def(this);
}


antlrcpp::Any ParserParser::Func_defContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitFunc_def(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Func_defContext* ParserParser::func_def() {
  Func_defContext *_localctx = _tracker.createInstance<Func_defContext>(_ctx, getState());
  enterRule(_localctx, 18, ParserParser::RuleFunc_def);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(173);
    func_type();
    setState(174);
    match(ParserParser::Id);
    setState(175);
    match(ParserParser::SLp);
    setState(177);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ParserParser::Int

    || _la == ParserParser::Float) {
      setState(176);
      func_f_params();
    }
    setState(179);
    match(ParserParser::SRp);
    setState(180);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Func_typeContext ------------------------------------------------------------------

ParserParser::Func_typeContext::Func_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Func_typeContext::Void() {
  return getToken(ParserParser::Void, 0);
}

tree::TerminalNode* ParserParser::Func_typeContext::Int() {
  return getToken(ParserParser::Int, 0);
}

tree::TerminalNode* ParserParser::Func_typeContext::Float() {
  return getToken(ParserParser::Float, 0);
}


size_t ParserParser::Func_typeContext::getRuleIndex() const {
  return ParserParser::RuleFunc_type;
}

void ParserParser::Func_typeContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunc_type(this);
}

void ParserParser::Func_typeContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunc_type(this);
}


antlrcpp::Any ParserParser::Func_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitFunc_type(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Func_typeContext* ParserParser::func_type() {
  Func_typeContext *_localctx = _tracker.createInstance<Func_typeContext>(_ctx, getState());
  enterRule(_localctx, 20, ParserParser::RuleFunc_type);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(182);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::Int)
      | (1ULL << ParserParser::Float)
      | (1ULL << ParserParser::Void))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Func_f_paramsContext ------------------------------------------------------------------

ParserParser::Func_f_paramsContext::Func_f_paramsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::Func_f_paramContext *> ParserParser::Func_f_paramsContext::func_f_param() {
  return getRuleContexts<ParserParser::Func_f_paramContext>();
}

ParserParser::Func_f_paramContext* ParserParser::Func_f_paramsContext::func_f_param(size_t i) {
  return getRuleContext<ParserParser::Func_f_paramContext>(i);
}


size_t ParserParser::Func_f_paramsContext::getRuleIndex() const {
  return ParserParser::RuleFunc_f_params;
}

void ParserParser::Func_f_paramsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunc_f_params(this);
}

void ParserParser::Func_f_paramsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunc_f_params(this);
}


antlrcpp::Any ParserParser::Func_f_paramsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitFunc_f_params(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Func_f_paramsContext* ParserParser::func_f_params() {
  Func_f_paramsContext *_localctx = _tracker.createInstance<Func_f_paramsContext>(_ctx, getState());
  enterRule(_localctx, 22, ParserParser::RuleFunc_f_params);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(184);
    func_f_param();
    setState(189);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::T__0) {
      setState(185);
      match(ParserParser::T__0);
      setState(186);
      func_f_param();
      setState(191);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Func_f_paramContext ------------------------------------------------------------------

ParserParser::Func_f_paramContext::Func_f_paramContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::BtypeContext* ParserParser::Func_f_paramContext::btype() {
  return getRuleContext<ParserParser::BtypeContext>(0);
}

tree::TerminalNode* ParserParser::Func_f_paramContext::Id() {
  return getToken(ParserParser::Id, 0);
}

std::vector<tree::TerminalNode *> ParserParser::Func_f_paramContext::Lsb() {
  return getTokens(ParserParser::Lsb);
}

tree::TerminalNode* ParserParser::Func_f_paramContext::Lsb(size_t i) {
  return getToken(ParserParser::Lsb, i);
}

std::vector<tree::TerminalNode *> ParserParser::Func_f_paramContext::Rsb() {
  return getTokens(ParserParser::Rsb);
}

tree::TerminalNode* ParserParser::Func_f_paramContext::Rsb(size_t i) {
  return getToken(ParserParser::Rsb, i);
}

std::vector<ParserParser::Const_expContext *> ParserParser::Func_f_paramContext::const_exp() {
  return getRuleContexts<ParserParser::Const_expContext>();
}

ParserParser::Const_expContext* ParserParser::Func_f_paramContext::const_exp(size_t i) {
  return getRuleContext<ParserParser::Const_expContext>(i);
}


size_t ParserParser::Func_f_paramContext::getRuleIndex() const {
  return ParserParser::RuleFunc_f_param;
}

void ParserParser::Func_f_paramContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunc_f_param(this);
}

void ParserParser::Func_f_paramContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunc_f_param(this);
}


antlrcpp::Any ParserParser::Func_f_paramContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitFunc_f_param(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Func_f_paramContext* ParserParser::func_f_param() {
  Func_f_paramContext *_localctx = _tracker.createInstance<Func_f_paramContext>(_ctx, getState());
  enterRule(_localctx, 24, ParserParser::RuleFunc_f_param);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(192);
    btype();
    setState(193);
    match(ParserParser::Id);
    setState(205);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ParserParser::Lsb) {
      setState(194);
      match(ParserParser::Lsb);
      setState(195);
      match(ParserParser::Rsb);
      setState(202);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == ParserParser::Lsb) {
        setState(196);
        match(ParserParser::Lsb);
        setState(197);
        const_exp();
        setState(198);
        match(ParserParser::Rsb);
        setState(204);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

ParserParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::DeclContext *> ParserParser::BlockContext::decl() {
  return getRuleContexts<ParserParser::DeclContext>();
}

ParserParser::DeclContext* ParserParser::BlockContext::decl(size_t i) {
  return getRuleContext<ParserParser::DeclContext>(i);
}

std::vector<ParserParser::StmtContext *> ParserParser::BlockContext::stmt() {
  return getRuleContexts<ParserParser::StmtContext>();
}

ParserParser::StmtContext* ParserParser::BlockContext::stmt(size_t i) {
  return getRuleContext<ParserParser::StmtContext>(i);
}


size_t ParserParser::BlockContext::getRuleIndex() const {
  return ParserParser::RuleBlock;
}

void ParserParser::BlockContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBlock(this);
}

void ParserParser::BlockContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBlock(this);
}


antlrcpp::Any ParserParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::BlockContext* ParserParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 26, ParserParser::RuleBlock);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(207);
    match(ParserParser::T__2);
    setState(216);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::T__1)
      | (1ULL << ParserParser::T__2)
      | (1ULL << ParserParser::Int)
      | (1ULL << ParserParser::Float)
      | (1ULL << ParserParser::Const)
      | (1ULL << ParserParser::If)
      | (1ULL << ParserParser::While)
      | (1ULL << ParserParser::Break)
      | (1ULL << ParserParser::Continue)
      | (1ULL << ParserParser::Return)
      | (1ULL << ParserParser::SLp)
      | (1ULL << ParserParser::Add)
      | (1ULL << ParserParser::Sub)
      | (1ULL << ParserParser::Not)
      | (1ULL << ParserParser::Id)
      | (1ULL << ParserParser::IntConst)
      | (1ULL << ParserParser::FloatConst))) != 0)) {
      setState(214);
      _errHandler->sync(this);
      switch (_input->LA(1)) {
        case ParserParser::Int:
        case ParserParser::Float:
        case ParserParser::Const: {
          setState(208);
          dynamic_cast<BlockContext *>(_localctx)->declContext = decl();
          _localctx->decl_vec.push_back({_localctx->i, dynamic_cast<BlockContext *>(_localctx)->declContext}); _localctx->i++;
          break;
        }

        case ParserParser::T__1:
        case ParserParser::T__2:
        case ParserParser::If:
        case ParserParser::While:
        case ParserParser::Break:
        case ParserParser::Continue:
        case ParserParser::Return:
        case ParserParser::SLp:
        case ParserParser::Add:
        case ParserParser::Sub:
        case ParserParser::Not:
        case ParserParser::Id:
        case ParserParser::IntConst:
        case ParserParser::FloatConst: {
          setState(211);
          dynamic_cast<BlockContext *>(_localctx)->stmtContext = stmt();
          _localctx->stmt_vec.push_back({_localctx->i, dynamic_cast<BlockContext *>(_localctx)->stmtContext}); _localctx->i++;
          break;
        }

      default:
        throw NoViableAltException(this);
      }
      setState(218);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(219);
    match(ParserParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

ParserParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::L_valContext* ParserParser::StmtContext::l_val() {
  return getRuleContext<ParserParser::L_valContext>(0);
}

tree::TerminalNode* ParserParser::StmtContext::Assign() {
  return getToken(ParserParser::Assign, 0);
}

ParserParser::ExpContext* ParserParser::StmtContext::exp() {
  return getRuleContext<ParserParser::ExpContext>(0);
}

ParserParser::Add_expContext* ParserParser::StmtContext::add_exp() {
  return getRuleContext<ParserParser::Add_expContext>(0);
}

ParserParser::BlockContext* ParserParser::StmtContext::block() {
  return getRuleContext<ParserParser::BlockContext>(0);
}

tree::TerminalNode* ParserParser::StmtContext::While() {
  return getToken(ParserParser::While, 0);
}

tree::TerminalNode* ParserParser::StmtContext::SLp() {
  return getToken(ParserParser::SLp, 0);
}

ParserParser::L_or_expContext* ParserParser::StmtContext::l_or_exp() {
  return getRuleContext<ParserParser::L_or_expContext>(0);
}

tree::TerminalNode* ParserParser::StmtContext::SRp() {
  return getToken(ParserParser::SRp, 0);
}

std::vector<ParserParser::StmtContext *> ParserParser::StmtContext::stmt() {
  return getRuleContexts<ParserParser::StmtContext>();
}

ParserParser::StmtContext* ParserParser::StmtContext::stmt(size_t i) {
  return getRuleContext<ParserParser::StmtContext>(i);
}

tree::TerminalNode* ParserParser::StmtContext::If() {
  return getToken(ParserParser::If, 0);
}

tree::TerminalNode* ParserParser::StmtContext::Else() {
  return getToken(ParserParser::Else, 0);
}

tree::TerminalNode* ParserParser::StmtContext::Break() {
  return getToken(ParserParser::Break, 0);
}

tree::TerminalNode* ParserParser::StmtContext::Continue() {
  return getToken(ParserParser::Continue, 0);
}

tree::TerminalNode* ParserParser::StmtContext::Return() {
  return getToken(ParserParser::Return, 0);
}


size_t ParserParser::StmtContext::getRuleIndex() const {
  return ParserParser::RuleStmt;
}

void ParserParser::StmtContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterStmt(this);
}

void ParserParser::StmtContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitStmt(this);
}


antlrcpp::Any ParserParser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitStmt(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::StmtContext* ParserParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 28, ParserParser::RuleStmt);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(255);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 24, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(221);
      l_val();
      setState(222);
      match(ParserParser::Assign);
      setState(223);
      exp();
      setState(224);
      match(ParserParser::T__1);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(227);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << ParserParser::SLp)
        | (1ULL << ParserParser::Add)
        | (1ULL << ParserParser::Sub)
        | (1ULL << ParserParser::Not)
        | (1ULL << ParserParser::Id)
        | (1ULL << ParserParser::IntConst)
        | (1ULL << ParserParser::FloatConst))) != 0)) {
        setState(226);
        add_exp();
      }
      setState(229);
      match(ParserParser::T__1);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(230);
      block();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(231);
      match(ParserParser::While);
      setState(232);
      match(ParserParser::SLp);
      setState(233);
      l_or_exp();
      setState(234);
      match(ParserParser::SRp);
      setState(235);
      stmt();
      break;
    }

    case 5: {
      enterOuterAlt(_localctx, 5);
      setState(237);
      match(ParserParser::If);
      setState(238);
      match(ParserParser::SLp);
      setState(239);
      l_or_exp();
      setState(240);
      match(ParserParser::SRp);
      setState(241);
      stmt();
      setState(244);
      _errHandler->sync(this);

      switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 22, _ctx)) {
      case 1: {
        setState(242);
        match(ParserParser::Else);
        setState(243);
        stmt();
        break;
      }

      }
      break;
    }

    case 6: {
      enterOuterAlt(_localctx, 6);
      setState(246);
      match(ParserParser::Break);
      setState(247);
      match(ParserParser::T__1);
      break;
    }

    case 7: {
      enterOuterAlt(_localctx, 7);
      setState(248);
      match(ParserParser::Continue);
      setState(249);
      match(ParserParser::T__1);
      break;
    }

    case 8: {
      enterOuterAlt(_localctx, 8);
      setState(250);
      match(ParserParser::Return);
      setState(252);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << ParserParser::SLp)
        | (1ULL << ParserParser::Add)
        | (1ULL << ParserParser::Sub)
        | (1ULL << ParserParser::Not)
        | (1ULL << ParserParser::Id)
        | (1ULL << ParserParser::IntConst)
        | (1ULL << ParserParser::FloatConst))) != 0)) {
        setState(251);
        exp();
      }
      setState(254);
      match(ParserParser::T__1);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExpContext ------------------------------------------------------------------

ParserParser::ExpContext::ExpContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::Add_expContext* ParserParser::ExpContext::add_exp() {
  return getRuleContext<ParserParser::Add_expContext>(0);
}


size_t ParserParser::ExpContext::getRuleIndex() const {
  return ParserParser::RuleExp;
}

void ParserParser::ExpContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterExp(this);
}

void ParserParser::ExpContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitExp(this);
}


antlrcpp::Any ParserParser::ExpContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitExp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::ExpContext* ParserParser::exp() {
  ExpContext *_localctx = _tracker.createInstance<ExpContext>(_ctx, getState());
  enterRule(_localctx, 30, ParserParser::RuleExp);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(257);
    add_exp();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- L_valContext ------------------------------------------------------------------

ParserParser::L_valContext::L_valContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::L_valContext::Id() {
  return getToken(ParserParser::Id, 0);
}

std::vector<tree::TerminalNode *> ParserParser::L_valContext::Lsb() {
  return getTokens(ParserParser::Lsb);
}

tree::TerminalNode* ParserParser::L_valContext::Lsb(size_t i) {
  return getToken(ParserParser::Lsb, i);
}

std::vector<ParserParser::ExpContext *> ParserParser::L_valContext::exp() {
  return getRuleContexts<ParserParser::ExpContext>();
}

ParserParser::ExpContext* ParserParser::L_valContext::exp(size_t i) {
  return getRuleContext<ParserParser::ExpContext>(i);
}

std::vector<tree::TerminalNode *> ParserParser::L_valContext::Rsb() {
  return getTokens(ParserParser::Rsb);
}

tree::TerminalNode* ParserParser::L_valContext::Rsb(size_t i) {
  return getToken(ParserParser::Rsb, i);
}


size_t ParserParser::L_valContext::getRuleIndex() const {
  return ParserParser::RuleL_val;
}

void ParserParser::L_valContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterL_val(this);
}

void ParserParser::L_valContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitL_val(this);
}


antlrcpp::Any ParserParser::L_valContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitL_val(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::L_valContext* ParserParser::l_val() {
  L_valContext *_localctx = _tracker.createInstance<L_valContext>(_ctx, getState());
  enterRule(_localctx, 32, ParserParser::RuleL_val);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(259);
    match(ParserParser::Id);
    setState(266);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::Lsb) {
      setState(260);
      match(ParserParser::Lsb);
      setState(261);
      exp();
      setState(262);
      match(ParserParser::Rsb);
      setState(268);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Primary_expContext ------------------------------------------------------------------

ParserParser::Primary_expContext::Primary_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Primary_expContext::SLp() {
  return getToken(ParserParser::SLp, 0);
}

ParserParser::ExpContext* ParserParser::Primary_expContext::exp() {
  return getRuleContext<ParserParser::ExpContext>(0);
}

tree::TerminalNode* ParserParser::Primary_expContext::SRp() {
  return getToken(ParserParser::SRp, 0);
}

ParserParser::L_valContext* ParserParser::Primary_expContext::l_val() {
  return getRuleContext<ParserParser::L_valContext>(0);
}

tree::TerminalNode* ParserParser::Primary_expContext::IntConst() {
  return getToken(ParserParser::IntConst, 0);
}

tree::TerminalNode* ParserParser::Primary_expContext::FloatConst() {
  return getToken(ParserParser::FloatConst, 0);
}


size_t ParserParser::Primary_expContext::getRuleIndex() const {
  return ParserParser::RulePrimary_exp;
}

void ParserParser::Primary_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterPrimary_exp(this);
}

void ParserParser::Primary_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitPrimary_exp(this);
}


antlrcpp::Any ParserParser::Primary_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitPrimary_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Primary_expContext* ParserParser::primary_exp() {
  Primary_expContext *_localctx = _tracker.createInstance<Primary_expContext>(_ctx, getState());
  enterRule(_localctx, 34, ParserParser::RulePrimary_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(275);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ParserParser::SLp: {
        enterOuterAlt(_localctx, 1);
        setState(269);
        match(ParserParser::SLp);
        setState(270);
        exp();
        setState(271);
        match(ParserParser::SRp);
        break;
      }

      case ParserParser::Id: {
        enterOuterAlt(_localctx, 2);
        setState(273);
        l_val();
        break;
      }

      case ParserParser::IntConst:
      case ParserParser::FloatConst: {
        enterOuterAlt(_localctx, 3);
        setState(274);
        _la = _input->LA(1);
        if (!(_la == ParserParser::IntConst

        || _la == ParserParser::FloatConst)) {
        _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Unary_expContext ------------------------------------------------------------------

ParserParser::Unary_expContext::Unary_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::Primary_expContext* ParserParser::Unary_expContext::primary_exp() {
  return getRuleContext<ParserParser::Primary_expContext>(0);
}

std::vector<ParserParser::Unary_opContext *> ParserParser::Unary_expContext::unary_op() {
  return getRuleContexts<ParserParser::Unary_opContext>();
}

ParserParser::Unary_opContext* ParserParser::Unary_expContext::unary_op(size_t i) {
  return getRuleContext<ParserParser::Unary_opContext>(i);
}

tree::TerminalNode* ParserParser::Unary_expContext::Id() {
  return getToken(ParserParser::Id, 0);
}

tree::TerminalNode* ParserParser::Unary_expContext::SLp() {
  return getToken(ParserParser::SLp, 0);
}

tree::TerminalNode* ParserParser::Unary_expContext::SRp() {
  return getToken(ParserParser::SRp, 0);
}

ParserParser::Func_r_paramsContext* ParserParser::Unary_expContext::func_r_params() {
  return getRuleContext<ParserParser::Func_r_paramsContext>(0);
}


size_t ParserParser::Unary_expContext::getRuleIndex() const {
  return ParserParser::RuleUnary_exp;
}

void ParserParser::Unary_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnary_exp(this);
}

void ParserParser::Unary_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnary_exp(this);
}


antlrcpp::Any ParserParser::Unary_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitUnary_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Unary_expContext* ParserParser::unary_exp() {
  Unary_expContext *_localctx = _tracker.createInstance<Unary_expContext>(_ctx, getState());
  enterRule(_localctx, 36, ParserParser::RuleUnary_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(296);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 30, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(280);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << ParserParser::Add)
        | (1ULL << ParserParser::Sub)
        | (1ULL << ParserParser::Not))) != 0)) {
        setState(277);
        unary_op();
        setState(282);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(283);
      primary_exp();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(287);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << ParserParser::Add)
        | (1ULL << ParserParser::Sub)
        | (1ULL << ParserParser::Not))) != 0)) {
        setState(284);
        unary_op();
        setState(289);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(290);
      match(ParserParser::Id);
      setState(291);
      match(ParserParser::SLp);
      setState(293);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << ParserParser::SLp)
        | (1ULL << ParserParser::Add)
        | (1ULL << ParserParser::Sub)
        | (1ULL << ParserParser::Not)
        | (1ULL << ParserParser::Id)
        | (1ULL << ParserParser::IntConst)
        | (1ULL << ParserParser::FloatConst))) != 0)) {
        setState(292);
        func_r_params();
      }
      setState(295);
      match(ParserParser::SRp);
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Unary_opContext ------------------------------------------------------------------

ParserParser::Unary_opContext::Unary_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Unary_opContext::Add() {
  return getToken(ParserParser::Add, 0);
}

tree::TerminalNode* ParserParser::Unary_opContext::Sub() {
  return getToken(ParserParser::Sub, 0);
}

tree::TerminalNode* ParserParser::Unary_opContext::Not() {
  return getToken(ParserParser::Not, 0);
}


size_t ParserParser::Unary_opContext::getRuleIndex() const {
  return ParserParser::RuleUnary_op;
}

void ParserParser::Unary_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnary_op(this);
}

void ParserParser::Unary_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnary_op(this);
}


antlrcpp::Any ParserParser::Unary_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitUnary_op(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Unary_opContext* ParserParser::unary_op() {
  Unary_opContext *_localctx = _tracker.createInstance<Unary_opContext>(_ctx, getState());
  enterRule(_localctx, 38, ParserParser::RuleUnary_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(298);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::Add)
      | (1ULL << ParserParser::Sub)
      | (1ULL << ParserParser::Not))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Func_r_paramsContext ------------------------------------------------------------------

ParserParser::Func_r_paramsContext::Func_r_paramsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::ExpContext *> ParserParser::Func_r_paramsContext::exp() {
  return getRuleContexts<ParserParser::ExpContext>();
}

ParserParser::ExpContext* ParserParser::Func_r_paramsContext::exp(size_t i) {
  return getRuleContext<ParserParser::ExpContext>(i);
}


size_t ParserParser::Func_r_paramsContext::getRuleIndex() const {
  return ParserParser::RuleFunc_r_params;
}

void ParserParser::Func_r_paramsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFunc_r_params(this);
}

void ParserParser::Func_r_paramsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFunc_r_params(this);
}


antlrcpp::Any ParserParser::Func_r_paramsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitFunc_r_params(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Func_r_paramsContext* ParserParser::func_r_params() {
  Func_r_paramsContext *_localctx = _tracker.createInstance<Func_r_paramsContext>(_ctx, getState());
  enterRule(_localctx, 40, ParserParser::RuleFunc_r_params);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(300);
    exp();
    setState(305);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::T__0) {
      setState(301);
      match(ParserParser::T__0);
      setState(302);
      exp();
      setState(307);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Mul_expContext ------------------------------------------------------------------

ParserParser::Mul_expContext::Mul_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::Unary_expContext *> ParserParser::Mul_expContext::unary_exp() {
  return getRuleContexts<ParserParser::Unary_expContext>();
}

ParserParser::Unary_expContext* ParserParser::Mul_expContext::unary_exp(size_t i) {
  return getRuleContext<ParserParser::Unary_expContext>(i);
}

std::vector<ParserParser::Sem_opContext *> ParserParser::Mul_expContext::sem_op() {
  return getRuleContexts<ParserParser::Sem_opContext>();
}

ParserParser::Sem_opContext* ParserParser::Mul_expContext::sem_op(size_t i) {
  return getRuleContext<ParserParser::Sem_opContext>(i);
}


size_t ParserParser::Mul_expContext::getRuleIndex() const {
  return ParserParser::RuleMul_exp;
}

void ParserParser::Mul_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterMul_exp(this);
}

void ParserParser::Mul_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitMul_exp(this);
}


antlrcpp::Any ParserParser::Mul_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitMul_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Mul_expContext* ParserParser::mul_exp() {
  Mul_expContext *_localctx = _tracker.createInstance<Mul_expContext>(_ctx, getState());
  enterRule(_localctx, 42, ParserParser::RuleMul_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(308);
    unary_exp();
    setState(314);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::Mul)
      | (1ULL << ParserParser::Div)
      | (1ULL << ParserParser::Mod))) != 0)) {
      setState(309);
      sem_op();
      setState(310);
      unary_exp();
      setState(316);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Sem_opContext ------------------------------------------------------------------

ParserParser::Sem_opContext::Sem_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Sem_opContext::Mul() {
  return getToken(ParserParser::Mul, 0);
}

tree::TerminalNode* ParserParser::Sem_opContext::Div() {
  return getToken(ParserParser::Div, 0);
}

tree::TerminalNode* ParserParser::Sem_opContext::Mod() {
  return getToken(ParserParser::Mod, 0);
}


size_t ParserParser::Sem_opContext::getRuleIndex() const {
  return ParserParser::RuleSem_op;
}

void ParserParser::Sem_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSem_op(this);
}

void ParserParser::Sem_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSem_op(this);
}


antlrcpp::Any ParserParser::Sem_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitSem_op(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Sem_opContext* ParserParser::sem_op() {
  Sem_opContext *_localctx = _tracker.createInstance<Sem_opContext>(_ctx, getState());
  enterRule(_localctx, 44, ParserParser::RuleSem_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(317);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::Mul)
      | (1ULL << ParserParser::Div)
      | (1ULL << ParserParser::Mod))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Add_expContext ------------------------------------------------------------------

ParserParser::Add_expContext::Add_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::Mul_expContext *> ParserParser::Add_expContext::mul_exp() {
  return getRuleContexts<ParserParser::Mul_expContext>();
}

ParserParser::Mul_expContext* ParserParser::Add_expContext::mul_exp(size_t i) {
  return getRuleContext<ParserParser::Mul_expContext>(i);
}

std::vector<ParserParser::Op_asContext *> ParserParser::Add_expContext::op_as() {
  return getRuleContexts<ParserParser::Op_asContext>();
}

ParserParser::Op_asContext* ParserParser::Add_expContext::op_as(size_t i) {
  return getRuleContext<ParserParser::Op_asContext>(i);
}


size_t ParserParser::Add_expContext::getRuleIndex() const {
  return ParserParser::RuleAdd_exp;
}

void ParserParser::Add_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAdd_exp(this);
}

void ParserParser::Add_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAdd_exp(this);
}


antlrcpp::Any ParserParser::Add_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitAdd_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Add_expContext* ParserParser::add_exp() {
  Add_expContext *_localctx = _tracker.createInstance<Add_expContext>(_ctx, getState());
  enterRule(_localctx, 46, ParserParser::RuleAdd_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(319);
    mul_exp();
    setState(325);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::Add

    || _la == ParserParser::Sub) {
      setState(320);
      op_as();
      setState(321);
      mul_exp();
      setState(327);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Op_asContext ------------------------------------------------------------------

ParserParser::Op_asContext::Op_asContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Op_asContext::Add() {
  return getToken(ParserParser::Add, 0);
}

tree::TerminalNode* ParserParser::Op_asContext::Sub() {
  return getToken(ParserParser::Sub, 0);
}


size_t ParserParser::Op_asContext::getRuleIndex() const {
  return ParserParser::RuleOp_as;
}

void ParserParser::Op_asContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterOp_as(this);
}

void ParserParser::Op_asContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitOp_as(this);
}


antlrcpp::Any ParserParser::Op_asContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitOp_as(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Op_asContext* ParserParser::op_as() {
  Op_asContext *_localctx = _tracker.createInstance<Op_asContext>(_ctx, getState());
  enterRule(_localctx, 48, ParserParser::RuleOp_as);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(328);
    _la = _input->LA(1);
    if (!(_la == ParserParser::Add

    || _la == ParserParser::Sub)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Rel_expContext ------------------------------------------------------------------

ParserParser::Rel_expContext::Rel_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::Add_expContext *> ParserParser::Rel_expContext::add_exp() {
  return getRuleContexts<ParserParser::Add_expContext>();
}

ParserParser::Add_expContext* ParserParser::Rel_expContext::add_exp(size_t i) {
  return getRuleContext<ParserParser::Add_expContext>(i);
}

std::vector<ParserParser::Com_opContext *> ParserParser::Rel_expContext::com_op() {
  return getRuleContexts<ParserParser::Com_opContext>();
}

ParserParser::Com_opContext* ParserParser::Rel_expContext::com_op(size_t i) {
  return getRuleContext<ParserParser::Com_opContext>(i);
}


size_t ParserParser::Rel_expContext::getRuleIndex() const {
  return ParserParser::RuleRel_exp;
}

void ParserParser::Rel_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRel_exp(this);
}

void ParserParser::Rel_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRel_exp(this);
}


antlrcpp::Any ParserParser::Rel_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitRel_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Rel_expContext* ParserParser::rel_exp() {
  Rel_expContext *_localctx = _tracker.createInstance<Rel_expContext>(_ctx, getState());
  enterRule(_localctx, 50, ParserParser::RuleRel_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(330);
    add_exp();
    setState(336);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::Gt)
      | (1ULL << ParserParser::Lt)
      | (1ULL << ParserParser::Le)
      | (1ULL << ParserParser::Ge))) != 0)) {
      setState(331);
      com_op();
      setState(332);
      add_exp();
      setState(338);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Com_opContext ------------------------------------------------------------------

ParserParser::Com_opContext::Com_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Com_opContext::Gt() {
  return getToken(ParserParser::Gt, 0);
}

tree::TerminalNode* ParserParser::Com_opContext::Lt() {
  return getToken(ParserParser::Lt, 0);
}

tree::TerminalNode* ParserParser::Com_opContext::Le() {
  return getToken(ParserParser::Le, 0);
}

tree::TerminalNode* ParserParser::Com_opContext::Ge() {
  return getToken(ParserParser::Ge, 0);
}


size_t ParserParser::Com_opContext::getRuleIndex() const {
  return ParserParser::RuleCom_op;
}

void ParserParser::Com_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCom_op(this);
}

void ParserParser::Com_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCom_op(this);
}


antlrcpp::Any ParserParser::Com_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitCom_op(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Com_opContext* ParserParser::com_op() {
  Com_opContext *_localctx = _tracker.createInstance<Com_opContext>(_ctx, getState());
  enterRule(_localctx, 52, ParserParser::RuleCom_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(339);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << ParserParser::Gt)
      | (1ULL << ParserParser::Lt)
      | (1ULL << ParserParser::Le)
      | (1ULL << ParserParser::Ge))) != 0))) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Eq_expContext ------------------------------------------------------------------

ParserParser::Eq_expContext::Eq_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::Rel_expContext *> ParserParser::Eq_expContext::rel_exp() {
  return getRuleContexts<ParserParser::Rel_expContext>();
}

ParserParser::Rel_expContext* ParserParser::Eq_expContext::rel_exp(size_t i) {
  return getRuleContext<ParserParser::Rel_expContext>(i);
}

std::vector<ParserParser::Com_op_eqmContext *> ParserParser::Eq_expContext::com_op_eqm() {
  return getRuleContexts<ParserParser::Com_op_eqmContext>();
}

ParserParser::Com_op_eqmContext* ParserParser::Eq_expContext::com_op_eqm(size_t i) {
  return getRuleContext<ParserParser::Com_op_eqmContext>(i);
}


size_t ParserParser::Eq_expContext::getRuleIndex() const {
  return ParserParser::RuleEq_exp;
}

void ParserParser::Eq_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterEq_exp(this);
}

void ParserParser::Eq_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitEq_exp(this);
}


antlrcpp::Any ParserParser::Eq_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitEq_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Eq_expContext* ParserParser::eq_exp() {
  Eq_expContext *_localctx = _tracker.createInstance<Eq_expContext>(_ctx, getState());
  enterRule(_localctx, 54, ParserParser::RuleEq_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(341);
    rel_exp();
    setState(347);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::Equal

    || _la == ParserParser::Nequal) {
      setState(342);
      com_op_eqm();
      setState(343);
      rel_exp();
      setState(349);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Com_op_eqmContext ------------------------------------------------------------------

ParserParser::Com_op_eqmContext::Com_op_eqmContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ParserParser::Com_op_eqmContext::Equal() {
  return getToken(ParserParser::Equal, 0);
}

tree::TerminalNode* ParserParser::Com_op_eqmContext::Nequal() {
  return getToken(ParserParser::Nequal, 0);
}


size_t ParserParser::Com_op_eqmContext::getRuleIndex() const {
  return ParserParser::RuleCom_op_eqm;
}

void ParserParser::Com_op_eqmContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterCom_op_eqm(this);
}

void ParserParser::Com_op_eqmContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitCom_op_eqm(this);
}


antlrcpp::Any ParserParser::Com_op_eqmContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitCom_op_eqm(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Com_op_eqmContext* ParserParser::com_op_eqm() {
  Com_op_eqmContext *_localctx = _tracker.createInstance<Com_op_eqmContext>(_ctx, getState());
  enterRule(_localctx, 56, ParserParser::RuleCom_op_eqm);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(350);
    _la = _input->LA(1);
    if (!(_la == ParserParser::Equal

    || _la == ParserParser::Nequal)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- L_and_expContext ------------------------------------------------------------------

ParserParser::L_and_expContext::L_and_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::Eq_expContext *> ParserParser::L_and_expContext::eq_exp() {
  return getRuleContexts<ParserParser::Eq_expContext>();
}

ParserParser::Eq_expContext* ParserParser::L_and_expContext::eq_exp(size_t i) {
  return getRuleContext<ParserParser::Eq_expContext>(i);
}

std::vector<tree::TerminalNode *> ParserParser::L_and_expContext::And() {
  return getTokens(ParserParser::And);
}

tree::TerminalNode* ParserParser::L_and_expContext::And(size_t i) {
  return getToken(ParserParser::And, i);
}


size_t ParserParser::L_and_expContext::getRuleIndex() const {
  return ParserParser::RuleL_and_exp;
}

void ParserParser::L_and_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterL_and_exp(this);
}

void ParserParser::L_and_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitL_and_exp(this);
}


antlrcpp::Any ParserParser::L_and_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitL_and_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::L_and_expContext* ParserParser::l_and_exp() {
  L_and_expContext *_localctx = _tracker.createInstance<L_and_expContext>(_ctx, getState());
  enterRule(_localctx, 58, ParserParser::RuleL_and_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(352);
    eq_exp();
    setState(357);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::And) {
      setState(353);
      match(ParserParser::And);
      setState(354);
      eq_exp();
      setState(359);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- L_or_expContext ------------------------------------------------------------------

ParserParser::L_or_expContext::L_or_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ParserParser::L_and_expContext *> ParserParser::L_or_expContext::l_and_exp() {
  return getRuleContexts<ParserParser::L_and_expContext>();
}

ParserParser::L_and_expContext* ParserParser::L_or_expContext::l_and_exp(size_t i) {
  return getRuleContext<ParserParser::L_and_expContext>(i);
}

std::vector<tree::TerminalNode *> ParserParser::L_or_expContext::Or() {
  return getTokens(ParserParser::Or);
}

tree::TerminalNode* ParserParser::L_or_expContext::Or(size_t i) {
  return getToken(ParserParser::Or, i);
}


size_t ParserParser::L_or_expContext::getRuleIndex() const {
  return ParserParser::RuleL_or_exp;
}

void ParserParser::L_or_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterL_or_exp(this);
}

void ParserParser::L_or_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitL_or_exp(this);
}


antlrcpp::Any ParserParser::L_or_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitL_or_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::L_or_expContext* ParserParser::l_or_exp() {
  L_or_expContext *_localctx = _tracker.createInstance<L_or_expContext>(_ctx, getState());
  enterRule(_localctx, 60, ParserParser::RuleL_or_exp);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(360);
    l_and_exp();
    setState(365);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == ParserParser::Or) {
      setState(361);
      match(ParserParser::Or);
      setState(362);
      l_and_exp();
      setState(367);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Const_expContext ------------------------------------------------------------------

ParserParser::Const_expContext::Const_expContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ParserParser::Add_expContext* ParserParser::Const_expContext::add_exp() {
  return getRuleContext<ParserParser::Add_expContext>(0);
}


size_t ParserParser::Const_expContext::getRuleIndex() const {
  return ParserParser::RuleConst_exp;
}

void ParserParser::Const_expContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConst_exp(this);
}

void ParserParser::Const_expContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<ParserListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConst_exp(this);
}


antlrcpp::Any ParserParser::Const_expContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ParserVisitor*>(visitor))
    return parserVisitor->visitConst_exp(this);
  else
    return visitor->visitChildren(this);
}

ParserParser::Const_expContext* ParserParser::const_exp() {
  Const_expContext *_localctx = _tracker.createInstance<Const_expContext>(_ctx, getState());
  enterRule(_localctx, 62, ParserParser::RuleConst_exp);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(368);
    add_exp();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> ParserParser::_decisionToDFA;
atn::PredictionContextCache ParserParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN ParserParser::_atn;
std::vector<uint16_t> ParserParser::_serializedATN;

std::vector<std::string> ParserParser::_ruleNames = {
  "comp_unit", "decl", "const_decl", "btype", "const_def", "const_init_val", 
  "var_decl", "var_def", "init_val", "func_def", "func_type", "func_f_params", 
  "func_f_param", "block", "stmt", "exp", "l_val", "primary_exp", "unary_exp", 
  "unary_op", "func_r_params", "mul_exp", "sem_op", "add_exp", "op_as", 
  "rel_exp", "com_op", "eq_exp", "com_op_eqm", "l_and_exp", "l_or_exp", 
  "const_exp"
};

std::vector<std::string> ParserParser::_literalNames = {
  "", "','", "';'", "'{'", "'}'", "'int'", "'float'", "'void'", "'const'", 
  "'if'", "'else'", "'while'", "'break'", "'continue'", "'return'", "'='", 
  "'['", "']'", "'('", "')'", "'+'", "'-'", "'*'", "'/'", "'%'", "'=='", 
  "'!='", "'>'", "'<'", "'<='", "'>='", "'!'", "'&&'", "'||'"
};

std::vector<std::string> ParserParser::_symbolicNames = {
  "", "", "", "", "", "Int", "Float", "Void", "Const", "If", "Else", "While", 
  "Break", "Continue", "Return", "Assign", "Lsb", "Rsb", "SLp", "SRp", "Add", 
  "Sub", "Mul", "Div", "Mod", "Equal", "Nequal", "Gt", "Lt", "Le", "Ge", 
  "Not", "And", "Or", "Id", "IntConst", "FloatConst", "Ws", "BlockComment", 
  "LineComment", "Eof"
};

dfa::Vocabulary ParserParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> ParserParser::_tokenNames;

ParserParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x2a, 0x175, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 
    0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x3, 0x2, 
    0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x7, 0x2, 0x49, 0xa, 
    0x2, 0xc, 0x2, 0xe, 0x2, 0x4c, 0xb, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 
    0x3, 0x3, 0x5, 0x3, 0x52, 0xa, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x7, 0x4, 0x59, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x5c, 0xb, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 
    0x6, 0x3, 0x6, 0x3, 0x6, 0x7, 0x6, 0x67, 0xa, 0x6, 0xc, 0x6, 0xe, 0x6, 
    0x6a, 0xb, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x7, 0x7, 0x74, 0xa, 0x7, 0xc, 0x7, 0xe, 0x7, 
    0x77, 0xb, 0x7, 0x5, 0x7, 0x79, 0xa, 0x7, 0x3, 0x7, 0x5, 0x7, 0x7c, 
    0xa, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x7, 0x8, 0x82, 0xa, 
    0x8, 0xc, 0x8, 0xe, 0x8, 0x85, 0xb, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x7, 0x9, 0x8e, 0xa, 0x9, 0xc, 
    0x9, 0xe, 0x9, 0x91, 0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 
    0x3, 0x9, 0x7, 0x9, 0x98, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x9b, 0xb, 0x9, 
    0x3, 0x9, 0x3, 0x9, 0x5, 0x9, 0x9f, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 
    0xa, 0x3, 0xa, 0x3, 0xa, 0x7, 0xa, 0xa6, 0xa, 0xa, 0xc, 0xa, 0xe, 0xa, 
    0xa9, 0xb, 0xa, 0x5, 0xa, 0xab, 0xa, 0xa, 0x3, 0xa, 0x5, 0xa, 0xae, 
    0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0xb4, 0xa, 
    0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 
    0xd, 0x3, 0xd, 0x7, 0xd, 0xbe, 0xa, 0xd, 0xc, 0xd, 0xe, 0xd, 0xc1, 0xb, 
    0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 
    0xe, 0x3, 0xe, 0x7, 0xe, 0xcb, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 0xce, 0xb, 
    0xe, 0x5, 0xe, 0xd0, 0xa, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
    0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 0x7, 0xf, 0xd9, 0xa, 0xf, 0xc, 0xf, 0xe, 
    0xf, 0xdc, 0xb, 0xf, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0xe6, 0xa, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x5, 0x10, 0xf7, 0xa, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 
    0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x5, 0x10, 0xff, 0xa, 0x10, 0x3, 
    0x10, 0x5, 0x10, 0x102, 0xa, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 
    0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x7, 0x12, 0x10b, 0xa, 0x12, 
    0xc, 0x12, 0xe, 0x12, 0x10e, 0xb, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 
    0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0x116, 0xa, 0x13, 0x3, 0x14, 
    0x7, 0x14, 0x119, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 0x11c, 0xb, 0x14, 
    0x3, 0x14, 0x3, 0x14, 0x7, 0x14, 0x120, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 
    0x123, 0xb, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0x128, 
    0xa, 0x14, 0x3, 0x14, 0x5, 0x14, 0x12b, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x7, 0x16, 0x132, 0xa, 0x16, 0xc, 0x16, 
    0xe, 0x16, 0x135, 0xb, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x7, 0x17, 0x13b, 0xa, 0x17, 0xc, 0x17, 0xe, 0x17, 0x13e, 0xb, 0x17, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x7, 
    0x19, 0x146, 0xa, 0x19, 0xc, 0x19, 0xe, 0x19, 0x149, 0xb, 0x19, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x7, 0x1b, 
    0x151, 0xa, 0x1b, 0xc, 0x1b, 0xe, 0x1b, 0x154, 0xb, 0x1b, 0x3, 0x1c, 
    0x3, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x7, 0x1d, 0x15c, 
    0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0x15f, 0xb, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x7, 0x1f, 0x166, 0xa, 0x1f, 0xc, 0x1f, 
    0xe, 0x1f, 0x169, 0xb, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x7, 0x20, 
    0x16e, 0xa, 0x20, 0xc, 0x20, 0xe, 0x20, 0x171, 0xb, 0x20, 0x3, 0x21, 
    0x3, 0x21, 0x3, 0x21, 0x2, 0x2, 0x22, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 
    0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 
    0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 
    0x3e, 0x40, 0x2, 0xa, 0x3, 0x2, 0x7, 0x8, 0x3, 0x2, 0x7, 0x9, 0x3, 0x2, 
    0x25, 0x26, 0x4, 0x2, 0x16, 0x17, 0x21, 0x21, 0x3, 0x2, 0x18, 0x1a, 
    0x3, 0x2, 0x16, 0x17, 0x3, 0x2, 0x1d, 0x20, 0x3, 0x2, 0x1b, 0x1c, 0x2, 
    0x181, 0x2, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x4, 0x51, 0x3, 0x2, 0x2, 0x2, 
    0x6, 0x53, 0x3, 0x2, 0x2, 0x2, 0x8, 0x5f, 0x3, 0x2, 0x2, 0x2, 0xa, 0x61, 
    0x3, 0x2, 0x2, 0x2, 0xc, 0x7b, 0x3, 0x2, 0x2, 0x2, 0xe, 0x7d, 0x3, 0x2, 
    0x2, 0x2, 0x10, 0x9e, 0x3, 0x2, 0x2, 0x2, 0x12, 0xad, 0x3, 0x2, 0x2, 
    0x2, 0x14, 0xaf, 0x3, 0x2, 0x2, 0x2, 0x16, 0xb8, 0x3, 0x2, 0x2, 0x2, 
    0x18, 0xba, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xc2, 0x3, 0x2, 0x2, 0x2, 0x1c, 
    0xd1, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x101, 0x3, 0x2, 0x2, 0x2, 0x20, 0x103, 
    0x3, 0x2, 0x2, 0x2, 0x22, 0x105, 0x3, 0x2, 0x2, 0x2, 0x24, 0x115, 0x3, 
    0x2, 0x2, 0x2, 0x26, 0x12a, 0x3, 0x2, 0x2, 0x2, 0x28, 0x12c, 0x3, 0x2, 
    0x2, 0x2, 0x2a, 0x12e, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x136, 0x3, 0x2, 0x2, 
    0x2, 0x2e, 0x13f, 0x3, 0x2, 0x2, 0x2, 0x30, 0x141, 0x3, 0x2, 0x2, 0x2, 
    0x32, 0x14a, 0x3, 0x2, 0x2, 0x2, 0x34, 0x14c, 0x3, 0x2, 0x2, 0x2, 0x36, 
    0x155, 0x3, 0x2, 0x2, 0x2, 0x38, 0x157, 0x3, 0x2, 0x2, 0x2, 0x3a, 0x160, 
    0x3, 0x2, 0x2, 0x2, 0x3c, 0x162, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x16a, 0x3, 
    0x2, 0x2, 0x2, 0x40, 0x172, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x5, 0x4, 
    0x3, 0x2, 0x43, 0x44, 0x8, 0x2, 0x1, 0x2, 0x44, 0x49, 0x3, 0x2, 0x2, 
    0x2, 0x45, 0x46, 0x5, 0x14, 0xb, 0x2, 0x46, 0x47, 0x8, 0x2, 0x1, 0x2, 
    0x47, 0x49, 0x3, 0x2, 0x2, 0x2, 0x48, 0x42, 0x3, 0x2, 0x2, 0x2, 0x48, 
    0x45, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x48, 
    0x3, 0x2, 0x2, 0x2, 0x4a, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x4d, 0x3, 
    0x2, 0x2, 0x2, 0x4c, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x7, 0x2, 
    0x2, 0x3, 0x4e, 0x3, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x52, 0x5, 0x6, 0x4, 
    0x2, 0x50, 0x52, 0x5, 0xe, 0x8, 0x2, 0x51, 0x4f, 0x3, 0x2, 0x2, 0x2, 
    0x51, 0x50, 0x3, 0x2, 0x2, 0x2, 0x52, 0x5, 0x3, 0x2, 0x2, 0x2, 0x53, 
    0x54, 0x7, 0xa, 0x2, 0x2, 0x54, 0x55, 0x5, 0x8, 0x5, 0x2, 0x55, 0x5a, 
    0x5, 0xa, 0x6, 0x2, 0x56, 0x57, 0x7, 0x3, 0x2, 0x2, 0x57, 0x59, 0x5, 
    0xa, 0x6, 0x2, 0x58, 0x56, 0x3, 0x2, 0x2, 0x2, 0x59, 0x5c, 0x3, 0x2, 
    0x2, 0x2, 0x5a, 0x58, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x3, 0x2, 0x2, 
    0x2, 0x5b, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5a, 0x3, 0x2, 0x2, 0x2, 
    0x5d, 0x5e, 0x7, 0x4, 0x2, 0x2, 0x5e, 0x7, 0x3, 0x2, 0x2, 0x2, 0x5f, 
    0x60, 0x9, 0x2, 0x2, 0x2, 0x60, 0x9, 0x3, 0x2, 0x2, 0x2, 0x61, 0x68, 
    0x7, 0x24, 0x2, 0x2, 0x62, 0x63, 0x7, 0x12, 0x2, 0x2, 0x63, 0x64, 0x5, 
    0x40, 0x21, 0x2, 0x64, 0x65, 0x7, 0x13, 0x2, 0x2, 0x65, 0x67, 0x3, 0x2, 
    0x2, 0x2, 0x66, 0x62, 0x3, 0x2, 0x2, 0x2, 0x67, 0x6a, 0x3, 0x2, 0x2, 
    0x2, 0x68, 0x66, 0x3, 0x2, 0x2, 0x2, 0x68, 0x69, 0x3, 0x2, 0x2, 0x2, 
    0x69, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x68, 0x3, 0x2, 0x2, 0x2, 0x6b, 
    0x6c, 0x7, 0x11, 0x2, 0x2, 0x6c, 0x6d, 0x5, 0xc, 0x7, 0x2, 0x6d, 0xb, 
    0x3, 0x2, 0x2, 0x2, 0x6e, 0x7c, 0x5, 0x40, 0x21, 0x2, 0x6f, 0x78, 0x7, 
    0x5, 0x2, 0x2, 0x70, 0x75, 0x5, 0xc, 0x7, 0x2, 0x71, 0x72, 0x7, 0x3, 
    0x2, 0x2, 0x72, 0x74, 0x5, 0xc, 0x7, 0x2, 0x73, 0x71, 0x3, 0x2, 0x2, 
    0x2, 0x74, 0x77, 0x3, 0x2, 0x2, 0x2, 0x75, 0x73, 0x3, 0x2, 0x2, 0x2, 
    0x75, 0x76, 0x3, 0x2, 0x2, 0x2, 0x76, 0x79, 0x3, 0x2, 0x2, 0x2, 0x77, 
    0x75, 0x3, 0x2, 0x2, 0x2, 0x78, 0x70, 0x3, 0x2, 0x2, 0x2, 0x78, 0x79, 
    0x3, 0x2, 0x2, 0x2, 0x79, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7c, 0x7, 
    0x6, 0x2, 0x2, 0x7b, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x7b, 0x6f, 0x3, 0x2, 
    0x2, 0x2, 0x7c, 0xd, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7e, 0x5, 0x8, 0x5, 
    0x2, 0x7e, 0x83, 0x5, 0x10, 0x9, 0x2, 0x7f, 0x80, 0x7, 0x3, 0x2, 0x2, 
    0x80, 0x82, 0x5, 0x10, 0x9, 0x2, 0x81, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x82, 
    0x85, 0x3, 0x2, 0x2, 0x2, 0x83, 0x81, 0x3, 0x2, 0x2, 0x2, 0x83, 0x84, 
    0x3, 0x2, 0x2, 0x2, 0x84, 0x86, 0x3, 0x2, 0x2, 0x2, 0x85, 0x83, 0x3, 
    0x2, 0x2, 0x2, 0x86, 0x87, 0x7, 0x4, 0x2, 0x2, 0x87, 0xf, 0x3, 0x2, 
    0x2, 0x2, 0x88, 0x8f, 0x7, 0x24, 0x2, 0x2, 0x89, 0x8a, 0x7, 0x12, 0x2, 
    0x2, 0x8a, 0x8b, 0x5, 0x40, 0x21, 0x2, 0x8b, 0x8c, 0x7, 0x13, 0x2, 0x2, 
    0x8c, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x89, 0x3, 0x2, 0x2, 0x2, 0x8e, 
    0x91, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x8d, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x90, 
    0x3, 0x2, 0x2, 0x2, 0x90, 0x9f, 0x3, 0x2, 0x2, 0x2, 0x91, 0x8f, 0x3, 
    0x2, 0x2, 0x2, 0x92, 0x99, 0x7, 0x24, 0x2, 0x2, 0x93, 0x94, 0x7, 0x12, 
    0x2, 0x2, 0x94, 0x95, 0x5, 0x40, 0x21, 0x2, 0x95, 0x96, 0x7, 0x13, 0x2, 
    0x2, 0x96, 0x98, 0x3, 0x2, 0x2, 0x2, 0x97, 0x93, 0x3, 0x2, 0x2, 0x2, 
    0x98, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x99, 0x97, 0x3, 0x2, 0x2, 0x2, 0x99, 
    0x9a, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9c, 0x3, 0x2, 0x2, 0x2, 0x9b, 0x99, 
    0x3, 0x2, 0x2, 0x2, 0x9c, 0x9d, 0x7, 0x11, 0x2, 0x2, 0x9d, 0x9f, 0x5, 
    0x12, 0xa, 0x2, 0x9e, 0x88, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x92, 0x3, 0x2, 
    0x2, 0x2, 0x9f, 0x11, 0x3, 0x2, 0x2, 0x2, 0xa0, 0xae, 0x5, 0x20, 0x11, 
    0x2, 0xa1, 0xaa, 0x7, 0x5, 0x2, 0x2, 0xa2, 0xa7, 0x5, 0x12, 0xa, 0x2, 
    0xa3, 0xa4, 0x7, 0x3, 0x2, 0x2, 0xa4, 0xa6, 0x5, 0x12, 0xa, 0x2, 0xa5, 
    0xa3, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa5, 
    0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xa8, 0xab, 0x3, 
    0x2, 0x2, 0x2, 0xa9, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xa2, 0x3, 0x2, 
    0x2, 0x2, 0xaa, 0xab, 0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 0x3, 0x2, 0x2, 
    0x2, 0xac, 0xae, 0x7, 0x6, 0x2, 0x2, 0xad, 0xa0, 0x3, 0x2, 0x2, 0x2, 
    0xad, 0xa1, 0x3, 0x2, 0x2, 0x2, 0xae, 0x13, 0x3, 0x2, 0x2, 0x2, 0xaf, 
    0xb0, 0x5, 0x16, 0xc, 0x2, 0xb0, 0xb1, 0x7, 0x24, 0x2, 0x2, 0xb1, 0xb3, 
    0x7, 0x14, 0x2, 0x2, 0xb2, 0xb4, 0x5, 0x18, 0xd, 0x2, 0xb3, 0xb2, 0x3, 
    0x2, 0x2, 0x2, 0xb3, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xb5, 0x3, 0x2, 
    0x2, 0x2, 0xb5, 0xb6, 0x7, 0x15, 0x2, 0x2, 0xb6, 0xb7, 0x5, 0x1c, 0xf, 
    0x2, 0xb7, 0x15, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb9, 0x9, 0x3, 0x2, 0x2, 
    0xb9, 0x17, 0x3, 0x2, 0x2, 0x2, 0xba, 0xbf, 0x5, 0x1a, 0xe, 0x2, 0xbb, 
    0xbc, 0x7, 0x3, 0x2, 0x2, 0xbc, 0xbe, 0x5, 0x1a, 0xe, 0x2, 0xbd, 0xbb, 
    0x3, 0x2, 0x2, 0x2, 0xbe, 0xc1, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xbd, 0x3, 
    0x2, 0x2, 0x2, 0xbf, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xc0, 0x19, 0x3, 0x2, 
    0x2, 0x2, 0xc1, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc3, 0x5, 0x8, 0x5, 
    0x2, 0xc3, 0xcf, 0x7, 0x24, 0x2, 0x2, 0xc4, 0xc5, 0x7, 0x12, 0x2, 0x2, 
    0xc5, 0xcc, 0x7, 0x13, 0x2, 0x2, 0xc6, 0xc7, 0x7, 0x12, 0x2, 0x2, 0xc7, 
    0xc8, 0x5, 0x40, 0x21, 0x2, 0xc8, 0xc9, 0x7, 0x13, 0x2, 0x2, 0xc9, 0xcb, 
    0x3, 0x2, 0x2, 0x2, 0xca, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xce, 0x3, 
    0x2, 0x2, 0x2, 0xcc, 0xca, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xcd, 0x3, 0x2, 
    0x2, 0x2, 0xcd, 0xd0, 0x3, 0x2, 0x2, 0x2, 0xce, 0xcc, 0x3, 0x2, 0x2, 
    0x2, 0xcf, 0xc4, 0x3, 0x2, 0x2, 0x2, 0xcf, 0xd0, 0x3, 0x2, 0x2, 0x2, 
    0xd0, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xd1, 0xda, 0x7, 0x5, 0x2, 0x2, 0xd2, 
    0xd3, 0x5, 0x4, 0x3, 0x2, 0xd3, 0xd4, 0x8, 0xf, 0x1, 0x2, 0xd4, 0xd9, 
    0x3, 0x2, 0x2, 0x2, 0xd5, 0xd6, 0x5, 0x1e, 0x10, 0x2, 0xd6, 0xd7, 0x8, 
    0xf, 0x1, 0x2, 0xd7, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xd2, 0x3, 0x2, 
    0x2, 0x2, 0xd8, 0xd5, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xdc, 0x3, 0x2, 0x2, 
    0x2, 0xda, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 0x3, 0x2, 0x2, 0x2, 
    0xdb, 0xdd, 0x3, 0x2, 0x2, 0x2, 0xdc, 0xda, 0x3, 0x2, 0x2, 0x2, 0xdd, 
    0xde, 0x7, 0x6, 0x2, 0x2, 0xde, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe0, 
    0x5, 0x22, 0x12, 0x2, 0xe0, 0xe1, 0x7, 0x11, 0x2, 0x2, 0xe1, 0xe2, 0x5, 
    0x20, 0x11, 0x2, 0xe2, 0xe3, 0x7, 0x4, 0x2, 0x2, 0xe3, 0x102, 0x3, 0x2, 
    0x2, 0x2, 0xe4, 0xe6, 0x5, 0x30, 0x19, 0x2, 0xe5, 0xe4, 0x3, 0x2, 0x2, 
    0x2, 0xe5, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe7, 0x3, 0x2, 0x2, 0x2, 
    0xe7, 0x102, 0x7, 0x4, 0x2, 0x2, 0xe8, 0x102, 0x5, 0x1c, 0xf, 0x2, 0xe9, 
    0xea, 0x7, 0xd, 0x2, 0x2, 0xea, 0xeb, 0x7, 0x14, 0x2, 0x2, 0xeb, 0xec, 
    0x5, 0x3e, 0x20, 0x2, 0xec, 0xed, 0x7, 0x15, 0x2, 0x2, 0xed, 0xee, 0x5, 
    0x1e, 0x10, 0x2, 0xee, 0x102, 0x3, 0x2, 0x2, 0x2, 0xef, 0xf0, 0x7, 0xb, 
    0x2, 0x2, 0xf0, 0xf1, 0x7, 0x14, 0x2, 0x2, 0xf1, 0xf2, 0x5, 0x3e, 0x20, 
    0x2, 0xf2, 0xf3, 0x7, 0x15, 0x2, 0x2, 0xf3, 0xf6, 0x5, 0x1e, 0x10, 0x2, 
    0xf4, 0xf5, 0x7, 0xc, 0x2, 0x2, 0xf5, 0xf7, 0x5, 0x1e, 0x10, 0x2, 0xf6, 
    0xf4, 0x3, 0x2, 0x2, 0x2, 0xf6, 0xf7, 0x3, 0x2, 0x2, 0x2, 0xf7, 0x102, 
    0x3, 0x2, 0x2, 0x2, 0xf8, 0xf9, 0x7, 0xe, 0x2, 0x2, 0xf9, 0x102, 0x7, 
    0x4, 0x2, 0x2, 0xfa, 0xfb, 0x7, 0xf, 0x2, 0x2, 0xfb, 0x102, 0x7, 0x4, 
    0x2, 0x2, 0xfc, 0xfe, 0x7, 0x10, 0x2, 0x2, 0xfd, 0xff, 0x5, 0x20, 0x11, 
    0x2, 0xfe, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xfe, 0xff, 0x3, 0x2, 0x2, 0x2, 
    0xff, 0x100, 0x3, 0x2, 0x2, 0x2, 0x100, 0x102, 0x7, 0x4, 0x2, 0x2, 0x101, 
    0xdf, 0x3, 0x2, 0x2, 0x2, 0x101, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x101, 0xe8, 
    0x3, 0x2, 0x2, 0x2, 0x101, 0xe9, 0x3, 0x2, 0x2, 0x2, 0x101, 0xef, 0x3, 
    0x2, 0x2, 0x2, 0x101, 0xf8, 0x3, 0x2, 0x2, 0x2, 0x101, 0xfa, 0x3, 0x2, 
    0x2, 0x2, 0x101, 0xfc, 0x3, 0x2, 0x2, 0x2, 0x102, 0x1f, 0x3, 0x2, 0x2, 
    0x2, 0x103, 0x104, 0x5, 0x30, 0x19, 0x2, 0x104, 0x21, 0x3, 0x2, 0x2, 
    0x2, 0x105, 0x10c, 0x7, 0x24, 0x2, 0x2, 0x106, 0x107, 0x7, 0x12, 0x2, 
    0x2, 0x107, 0x108, 0x5, 0x20, 0x11, 0x2, 0x108, 0x109, 0x7, 0x13, 0x2, 
    0x2, 0x109, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x10a, 0x106, 0x3, 0x2, 0x2, 
    0x2, 0x10b, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x10c, 0x10a, 0x3, 0x2, 0x2, 
    0x2, 0x10c, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x23, 0x3, 0x2, 0x2, 0x2, 
    0x10e, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10f, 0x110, 0x7, 0x14, 0x2, 0x2, 
    0x110, 0x111, 0x5, 0x20, 0x11, 0x2, 0x111, 0x112, 0x7, 0x15, 0x2, 0x2, 
    0x112, 0x116, 0x3, 0x2, 0x2, 0x2, 0x113, 0x116, 0x5, 0x22, 0x12, 0x2, 
    0x114, 0x116, 0x9, 0x4, 0x2, 0x2, 0x115, 0x10f, 0x3, 0x2, 0x2, 0x2, 
    0x115, 0x113, 0x3, 0x2, 0x2, 0x2, 0x115, 0x114, 0x3, 0x2, 0x2, 0x2, 
    0x116, 0x25, 0x3, 0x2, 0x2, 0x2, 0x117, 0x119, 0x5, 0x28, 0x15, 0x2, 
    0x118, 0x117, 0x3, 0x2, 0x2, 0x2, 0x119, 0x11c, 0x3, 0x2, 0x2, 0x2, 
    0x11a, 0x118, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x11b, 0x3, 0x2, 0x2, 0x2, 
    0x11b, 0x11d, 0x3, 0x2, 0x2, 0x2, 0x11c, 0x11a, 0x3, 0x2, 0x2, 0x2, 
    0x11d, 0x12b, 0x5, 0x24, 0x13, 0x2, 0x11e, 0x120, 0x5, 0x28, 0x15, 0x2, 
    0x11f, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x120, 0x123, 0x3, 0x2, 0x2, 0x2, 
    0x121, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x121, 0x122, 0x3, 0x2, 0x2, 0x2, 
    0x122, 0x124, 0x3, 0x2, 0x2, 0x2, 0x123, 0x121, 0x3, 0x2, 0x2, 0x2, 
    0x124, 0x125, 0x7, 0x24, 0x2, 0x2, 0x125, 0x127, 0x7, 0x14, 0x2, 0x2, 
    0x126, 0x128, 0x5, 0x2a, 0x16, 0x2, 0x127, 0x126, 0x3, 0x2, 0x2, 0x2, 
    0x127, 0x128, 0x3, 0x2, 0x2, 0x2, 0x128, 0x129, 0x3, 0x2, 0x2, 0x2, 
    0x129, 0x12b, 0x7, 0x15, 0x2, 0x2, 0x12a, 0x11a, 0x3, 0x2, 0x2, 0x2, 
    0x12a, 0x121, 0x3, 0x2, 0x2, 0x2, 0x12b, 0x27, 0x3, 0x2, 0x2, 0x2, 0x12c, 
    0x12d, 0x9, 0x5, 0x2, 0x2, 0x12d, 0x29, 0x3, 0x2, 0x2, 0x2, 0x12e, 0x133, 
    0x5, 0x20, 0x11, 0x2, 0x12f, 0x130, 0x7, 0x3, 0x2, 0x2, 0x130, 0x132, 
    0x5, 0x20, 0x11, 0x2, 0x131, 0x12f, 0x3, 0x2, 0x2, 0x2, 0x132, 0x135, 
    0x3, 0x2, 0x2, 0x2, 0x133, 0x131, 0x3, 0x2, 0x2, 0x2, 0x133, 0x134, 
    0x3, 0x2, 0x2, 0x2, 0x134, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x135, 0x133, 0x3, 
    0x2, 0x2, 0x2, 0x136, 0x13c, 0x5, 0x26, 0x14, 0x2, 0x137, 0x138, 0x5, 
    0x2e, 0x18, 0x2, 0x138, 0x139, 0x5, 0x26, 0x14, 0x2, 0x139, 0x13b, 0x3, 
    0x2, 0x2, 0x2, 0x13a, 0x137, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x13e, 0x3, 
    0x2, 0x2, 0x2, 0x13c, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x13c, 0x13d, 0x3, 
    0x2, 0x2, 0x2, 0x13d, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x13e, 0x13c, 0x3, 0x2, 
    0x2, 0x2, 0x13f, 0x140, 0x9, 0x6, 0x2, 0x2, 0x140, 0x2f, 0x3, 0x2, 0x2, 
    0x2, 0x141, 0x147, 0x5, 0x2c, 0x17, 0x2, 0x142, 0x143, 0x5, 0x32, 0x1a, 
    0x2, 0x143, 0x144, 0x5, 0x2c, 0x17, 0x2, 0x144, 0x146, 0x3, 0x2, 0x2, 
    0x2, 0x145, 0x142, 0x3, 0x2, 0x2, 0x2, 0x146, 0x149, 0x3, 0x2, 0x2, 
    0x2, 0x147, 0x145, 0x3, 0x2, 0x2, 0x2, 0x147, 0x148, 0x3, 0x2, 0x2, 
    0x2, 0x148, 0x31, 0x3, 0x2, 0x2, 0x2, 0x149, 0x147, 0x3, 0x2, 0x2, 0x2, 
    0x14a, 0x14b, 0x9, 0x7, 0x2, 0x2, 0x14b, 0x33, 0x3, 0x2, 0x2, 0x2, 0x14c, 
    0x152, 0x5, 0x30, 0x19, 0x2, 0x14d, 0x14e, 0x5, 0x36, 0x1c, 0x2, 0x14e, 
    0x14f, 0x5, 0x30, 0x19, 0x2, 0x14f, 0x151, 0x3, 0x2, 0x2, 0x2, 0x150, 
    0x14d, 0x3, 0x2, 0x2, 0x2, 0x151, 0x154, 0x3, 0x2, 0x2, 0x2, 0x152, 
    0x150, 0x3, 0x2, 0x2, 0x2, 0x152, 0x153, 0x3, 0x2, 0x2, 0x2, 0x153, 
    0x35, 0x3, 0x2, 0x2, 0x2, 0x154, 0x152, 0x3, 0x2, 0x2, 0x2, 0x155, 0x156, 
    0x9, 0x8, 0x2, 0x2, 0x156, 0x37, 0x3, 0x2, 0x2, 0x2, 0x157, 0x15d, 0x5, 
    0x34, 0x1b, 0x2, 0x158, 0x159, 0x5, 0x3a, 0x1e, 0x2, 0x159, 0x15a, 0x5, 
    0x34, 0x1b, 0x2, 0x15a, 0x15c, 0x3, 0x2, 0x2, 0x2, 0x15b, 0x158, 0x3, 
    0x2, 0x2, 0x2, 0x15c, 0x15f, 0x3, 0x2, 0x2, 0x2, 0x15d, 0x15b, 0x3, 
    0x2, 0x2, 0x2, 0x15d, 0x15e, 0x3, 0x2, 0x2, 0x2, 0x15e, 0x39, 0x3, 0x2, 
    0x2, 0x2, 0x15f, 0x15d, 0x3, 0x2, 0x2, 0x2, 0x160, 0x161, 0x9, 0x9, 
    0x2, 0x2, 0x161, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x162, 0x167, 0x5, 0x38, 
    0x1d, 0x2, 0x163, 0x164, 0x7, 0x22, 0x2, 0x2, 0x164, 0x166, 0x5, 0x38, 
    0x1d, 0x2, 0x165, 0x163, 0x3, 0x2, 0x2, 0x2, 0x166, 0x169, 0x3, 0x2, 
    0x2, 0x2, 0x167, 0x165, 0x3, 0x2, 0x2, 0x2, 0x167, 0x168, 0x3, 0x2, 
    0x2, 0x2, 0x168, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x169, 0x167, 0x3, 0x2, 0x2, 
    0x2, 0x16a, 0x16f, 0x5, 0x3c, 0x1f, 0x2, 0x16b, 0x16c, 0x7, 0x23, 0x2, 
    0x2, 0x16c, 0x16e, 0x5, 0x3c, 0x1f, 0x2, 0x16d, 0x16b, 0x3, 0x2, 0x2, 
    0x2, 0x16e, 0x171, 0x3, 0x2, 0x2, 0x2, 0x16f, 0x16d, 0x3, 0x2, 0x2, 
    0x2, 0x16f, 0x170, 0x3, 0x2, 0x2, 0x2, 0x170, 0x3f, 0x3, 0x2, 0x2, 0x2, 
    0x171, 0x16f, 0x3, 0x2, 0x2, 0x2, 0x172, 0x173, 0x5, 0x30, 0x19, 0x2, 
    0x173, 0x41, 0x3, 0x2, 0x2, 0x2, 0x28, 0x48, 0x4a, 0x51, 0x5a, 0x68, 
    0x75, 0x78, 0x7b, 0x83, 0x8f, 0x99, 0x9e, 0xa7, 0xaa, 0xad, 0xb3, 0xbf, 
    0xcc, 0xcf, 0xd8, 0xda, 0xe5, 0xf6, 0xfe, 0x101, 0x10c, 0x115, 0x11a, 
    0x121, 0x127, 0x12a, 0x133, 0x13c, 0x147, 0x152, 0x15d, 0x167, 0x16f, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

ParserParser::Initializer ParserParser::_init;
