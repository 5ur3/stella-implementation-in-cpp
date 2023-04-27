#include "../StellaExpression.h"
#include <iostream>

StellaMatchCaseExpression::StellaMatchCaseExpression() {
  this->type = STELLA_EXPRESSION_TYPE_MATCH_CASE;
}

StellaType StellaMatchCaseExpression::getStellaType() {
  return expression->getStellaType();
}

bool StellaMatchCaseExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;

  if (!this->match->isTypingCorrect() || !this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->match->isPattern()) {
    std::cout << "Type error: pattern expression is not a pattern" << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin match case" << std::endl;
  }

  return isCorrect;
}

void StellaMatchCaseExpression::proxyIdent(Stella::StellaIdent ident) {
  if (!this->match->isParsed()) {
    return this->match->proxyIdent(ident);
  }
  this->expression->proxyIdent(ident);
}

void StellaMatchCaseExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  if (!this->match->isParsed()) {
    return this->match->proxyExpressionTypeToken(typeToken);
  }
  this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaMatchCaseExpression::proxyType(
    StellaType type) {
  if (!this->match->isParsed()) {
    return this->match->proxyType(type);
  }
  this->expression->proxyType(type);
}

std::pair<std::string, StellaType> getPatternVar(StellaExpression *pattern) {
  switch (pattern->type) {
  case STELLA_EXPRESSION_TYPE_PATTERN_VAR: {
    StellaPatternVarExpression *expr = (StellaPatternVarExpression *)pattern;
    return {expr->ident, expr->getStellaType()};
  }
  case STELLA_EXPRESSION_TYPE_PATTERN_INL_INR: {
    StellaPatternInlInrExpression *expr =
        (StellaPatternInlInrExpression *)pattern;
    return getPatternVar(expr->expression);
  }
  default: {
    return {"", StellaType(STELLA_DATA_TYPE_INVALID)};
  }
  }
}

void StellaMatchCaseExpression::proxyExpression(StellaExpression *expression) {
  if (this->match == NULL) {
    this->match = expression;
    this->match->setContext(this->context);
  } else if (!this->match->isParsed()) {
    this->match->proxyExpression(expression);
  } else if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
    auto patternVar = getPatternVar(this->match);
    this->expression->addContext(patternVar.first, patternVar.second);
  } else {
    this->expression->proxyExpression(expression);
  }
}

bool StellaMatchCaseExpression::isParsed() {
  return this->match != NULL && this->expression != NULL &&
         this->match->isParsed() && this->expression->isParsed();
}