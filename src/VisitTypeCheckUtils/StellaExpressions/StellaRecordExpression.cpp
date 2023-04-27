#include "../StellaExpression.h"
#include <iostream>

StellaRecordExpression::StellaRecordExpression(int size) {
  this->expressions =
      std::vector<std::pair<std::string, StellaExpression *>>(size, {"", NULL});
  this->type = STELLA_EXPRESSION_TYPE_RECORD;
}

StellaType StellaRecordExpression::getStellaType() {
  StellaType type = StellaType(STELLA_DATA_TYPE_RECORD, this->expressions.size());

  for (int i = 0; i < this->expressions.size(); i++) {
    type.parseIdent(this->expressions[i].first);
    type.proxyType(this->expressions[i].second->getStellaType());
  }

  return type;
}

bool StellaRecordExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool areInnerExpressionsCorrect = true;
  for (int i = 0; i < this->expressions.size(); i++) {
    if (!this->expressions[i].second->isTypingCorrect()) {
      areInnerExpressionsCorrect = false;
      break;
    }
  }

  bool isCorrect = areInnerExpressionsCorrect;

  if (!isCorrect) {
    std::cout << "\tin record" << std::endl;
  }

  return isCorrect;
}

void StellaRecordExpression::proxyIdent(Stella::StellaIdent ident) {
  for (int i = 0; i < this->expressions.size(); i++) {
    if (this->expressions[i].second == NULL) {
      this->expressions[i].first = ident;
      return;
    }
    if (!this->expressions[i].second->isParsed()) {
      return this->expressions[i].second->proxyIdent(ident);
    }
  }
}

void StellaRecordExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  for (int i = 0; i < this->expressions.size(); i++) {
    if (!this->expressions[i].second->isParsed()) {
      return this->expressions[i].second->proxyExpressionTypeToken(typeToken);
    }
  }
}
void StellaRecordExpression::proxyType(StellaType type) {
  for (int i = 0; i < this->expressions.size(); i++) {
    if (!this->expressions[i].second->isParsed()) {
      return this->expressions[i].second->proxyType(type);
    }
  }
}

void StellaRecordExpression::proxyExpression(StellaExpression *expression) {
  for (int i = 0; i < this->expressions.size(); i++) {
    if (this->expressions[i].second == NULL) {
      this->expressions[i].second = expression;
      this->expressions[i].second->setContext(this->context);
      return;
    }
    if (!this->expressions[i].second->isParsed()) {
      return this->expressions[i].second->proxyExpression(expression);
      return;
    }
  }
}

bool StellaRecordExpression::isParsed() {
  for (int i = 0; i < this->expressions.size(); i++) {
    if (this->expressions[i].second == NULL ||
        !this->expressions[i].second->isParsed()) {
      return false;
    }
  }
  return true;
}