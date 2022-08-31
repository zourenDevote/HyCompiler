
// Generated from ./grammarrule/Parser.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  ParserLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, Int = 5, Float = 6, Void = 7, 
    Const = 8, If = 9, Else = 10, While = 11, Break = 12, Continue = 13, 
    Return = 14, Assign = 15, Lsb = 16, Rsb = 17, SLp = 18, SRp = 19, Add = 20, 
    Sub = 21, Mul = 22, Div = 23, Mod = 24, Equal = 25, Nequal = 26, Gt = 27, 
    Lt = 28, Le = 29, Ge = 30, Not = 31, And = 32, Or = 33, Id = 34, IntConst = 35, 
    FloatConst = 36, Ws = 37, BlockComment = 38, LineComment = 39, Eof = 40
  };

  ParserLexer(antlr4::CharStream *input);
  ~ParserLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

