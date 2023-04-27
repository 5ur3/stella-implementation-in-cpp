#ifndef STELLA_EXPRESSION_HEADER
#define STELLA_EXPRESSION_HEADER

#include "../Stella/Absyn.H"
#include "StellaType.h"
#include <map>

enum StellaExpressionType {
  STELLA_EXPRESSION_TYPE_CONST_INT = 0,
  STELLA_EXPRESSION_TYPE_CONST_BOOL = 1,
  STELLA_EXPRESSION_TYPE_VAR = 2,
  STELLA_EXPRESSION_TYPE_SUCC = 3,
  STELLA_EXPRESSION_TYPE_NAT_REC = 4,
  STELLA_EXPRESSION_TYPE_ABSTRACTION = 5,
  STELLA_EXPRESSION_TYPE_APPLICATION = 6,
  STELLA_EXPRESSION_TYPE_CONDITION = 7,

  STELLA_EXPRESSION_TYPE_CONST_UNIT = 8,
  STELLA_EXPRESSION_TYPE_TUPLE = 9,
  STELLA_EXPRESSION_TYPE_DOT_TUPLE = 10,
  STELLA_EXPRESSION_TYPE_INL_INR = 11,
  STELLA_EXPRESSION_TYPE_MATCH = 12,
  STELLA_EXPRESSION_TYPE_MATCH_CASE = 13,
  STELLA_EXPRESSION_TYPE_PATTERN_INL_INR = 14,
  STELLA_EXPRESSION_TYPE_PATTERN_VAR = 15,

  STELLA_EXPRESSION_TYPE_SEQUENCE = 16,
  STELLA_EXPRESSION_TYPE_REF = 17,
  STELLA_EXPRESSION_TYPE_DEREF = 18,
  STELLA_EXPRESSION_TYPE_ASSIGNMENT = 19,
  STELLA_EXPRESSION_TYPE_PANIC = 20,
  STELLA_EXPRESSION_TYPE_RECORD = 21,
  STELLA_EXPRESSION_TYPE_DOT_RECORD = 22
};

// Representation of any stella expression
class StellaExpression {
public:
  StellaExpressionType type;

  // Context gets read only in StellaVarExpression to determine it's type.
  // In other StellaExpression implementations it is just passed down the tree.
  //
  // Functions and abstractions not only pass the context down the tree,
  // they also add its params to the context of encosed expression
  std::map<Stella::StellaIdent, StellaType> context;

  // These methods have to be overriden for every StellaExpression
  // implementation
  virtual StellaType getStellaType() = 0;
  virtual bool isTypingCorrect() = 0;

  // Some leaf StellaExpression implementation, such as
  // StellaConstIntExpression, have no inner expression, no ident and no
  // user-defined type declarations.
  //
  // For those expressions, default
  // implementations of methods below are written here.
  virtual void proxyIdent(Stella::StellaIdent ident){};
  virtual void proxyExpressionTypeToken(StellaDataType typeToken){};
  virtual void proxyType(StellaType type){};
  virtual void proxyExpression(StellaExpression *expression){};
  virtual bool isParsed() { return true; };
  virtual bool isPattern() { return false; };

  // Context setter methods are common among all StellaExpression
  // implementations
  void setContext(std::map<Stella::StellaIdent, StellaType> context) {
    this->context = context;
  };
  void addContext(Stella::StellaIdent ident, StellaType type) {
    if (!this->context.count(ident)) {
      this->context.insert({ident, type});
    } else {
      this->context[ident] = type;
    }
  };
};

// All StellaExpression declarations are written below
class StellaConstIntExpression : public StellaExpression {
public:
  StellaConstIntExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
};

class StellaConstBoolExpression : public StellaExpression {
public:
  StellaConstBoolExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
};

class StellaConstUnitExpression : public StellaExpression {
public:
  StellaConstUnitExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
};

class StellaVarExpression : public StellaExpression {
public:
  Stella::StellaIdent ident;

  StellaVarExpression(Stella::StellaIdent ident);
  StellaType getStellaType();
  bool isTypingCorrect();
};

class StellaSuccExpression : public StellaExpression {
public:
  StellaExpression *expression = NULL;
  StellaSuccExpression();
  StellaType getStellaType();

  bool isTypingCorrect();
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  void proxyIdent(Stella::StellaIdent ident);
  bool isParsed();
};

class StellaNatRecExpression : public StellaExpression {
public:
  StellaExpression *n = NULL;
  StellaExpression *z = NULL;
  StellaExpression *s = NULL;

  StellaNatRecExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  void proxyIdent(Stella::StellaIdent ident);
  bool isParsed();
};

class StellaAbstractionExpression : public StellaExpression {
public:
  Stella::StellaIdent paramIdent = "";
  StellaType stellaType = StellaType(STELLA_DATA_TYPE_FUN);
  StellaExpression *expression = NULL;

  StellaAbstractionExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  void proxyIdent(Stella::StellaIdent ident);
  bool isParsed();
};

class StellaApplicationExpression : public StellaExpression {
public:
  StellaExpression *expression1 = NULL;
  StellaExpression *expression2 = NULL;

  StellaApplicationExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaDotTupleExpression : public StellaExpression {
public:
  StellaExpression *expression = NULL;
  int index = 0;

  StellaDotTupleExpression(int index);
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaTupleExpression : public StellaExpression {
public:
  StellaExpression *expression1 = NULL;
  StellaExpression *expression2 = NULL;

  StellaTupleExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaConditionExpression : public StellaExpression {
public:
  StellaExpression *condition = NULL;
  StellaExpression *expression1 = NULL;
  StellaExpression *expression2 = NULL;

  StellaConditionExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaInlInrExpression : public StellaExpression {
public:
  int side = 0;
  StellaExpression *expression = NULL;

  StellaInlInrExpression(int side);
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaMatchExpression : public StellaExpression {
public:
  StellaExpression *matchExpression = NULL;
  std::vector<StellaExpression *> expressions;

  StellaMatchExpression(int size);
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaMatchCaseExpression : public StellaExpression {
public:
  StellaExpression *match = NULL;
  StellaExpression *expression = NULL;

  StellaMatchCaseExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaPatternInlInrExpression : public StellaExpression {
public:
  int side = 0;
  StellaExpression *expression = NULL;

  StellaPatternInlInrExpression(int side);
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
  bool isPattern();
};

class StellaPatternVarExpression : public StellaExpression {
public:
  Stella::StellaIdent ident;

  StellaPatternVarExpression(Stella::StellaIdent ident);
  StellaType getStellaType();
  bool isTypingCorrect();
  bool isPattern();
};

class StellaSequenceExpression : public StellaExpression {
public:
  StellaExpression *expression1 = NULL;
  StellaExpression *expression2 = NULL;

  StellaSequenceExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaRefExpression : public StellaExpression {
public:
  StellaExpression *expression = NULL;
  StellaRefExpression();
  StellaType getStellaType();

  bool isTypingCorrect();
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  void proxyIdent(Stella::StellaIdent ident);
  bool isParsed();
};

class StellaDerefExpression : public StellaExpression {
public:
  StellaExpression *expression = NULL;
  StellaDerefExpression();
  StellaType getStellaType();

  bool isTypingCorrect();
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  void proxyIdent(Stella::StellaIdent ident);
  bool isParsed();
};

class StellaAssignmentExpression : public StellaExpression {
public:
  StellaExpression *expression1 = NULL;
  StellaExpression *expression2 = NULL;

  StellaAssignmentExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaPanicExpression : public StellaExpression {
public:
  StellaPanicExpression();
  StellaType getStellaType();
  bool isTypingCorrect();
};

class StellaRecordExpression : public StellaExpression {
public:
  std::vector<std::pair<std::string, StellaExpression *>> expressions;

  StellaRecordExpression(int size);
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

class StellaDotRecordExpression : public StellaExpression {
public:
  StellaExpression *expression = NULL;
  std::string field;

  StellaDotRecordExpression(std::string field);
  StellaType getStellaType();
  bool isTypingCorrect();
  void proxyIdent(Stella::StellaIdent ident);
  void proxyExpressionTypeToken(StellaDataType typeToken);
  void proxyType(StellaType type);
  void proxyExpression(StellaExpression *expression);
  bool isParsed();
};

#endif
