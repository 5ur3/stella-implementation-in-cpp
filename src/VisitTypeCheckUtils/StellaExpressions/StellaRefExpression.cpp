#include "../StellaExpression.h"
#include <iostream>

StellaRefExpression::StellaRefExpression() {
  this->type = STELLA_EXPRESSION_TYPE_REF;
}
StellaType StellaRefExpression::getStellaType() {
  return StellaType(STELLA_DATA_TYPE_REF, this->expression->getStellaType());
}
bool StellaRefExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin new" << std::endl;
  }

  return isCorrect;
}
void StellaRefExpression::proxyExpressionTypeToken(StellaDataType typeToken) {
  this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaRefExpression::proxyType(StellaType type) {
  this->expression->proxyType(type);
}
void StellaRefExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}
void StellaRefExpression::proxyIdent(Stella::StellaIdent ident) {
  this->expression->proxyIdent(ident);
}
bool StellaRefExpression::isParsed() {
  return this->expression != NULL && this->expression->isParsed();
}
