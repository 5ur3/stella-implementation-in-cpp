#include "../StellaExpression.h"
#include <iostream>

StellaSuccExpression::StellaSuccExpression() {
  this->type = STELLA_EXPRESSION_TYPE_SUCC;
}
StellaType StellaSuccExpression::getStellaType() {
  return StellaType(STELLA_DATA_TYPE_NAT);
}
bool StellaSuccExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->expression->getStellaType().castsTo(
                 StellaType(STELLA_DATA_TYPE_NAT))) {
    std::cout << "Type error: succ expression type is not Nat" << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin succ" << std::endl;
  }

  return isCorrect;
}
void StellaSuccExpression::proxyExpressionTypeToken(StellaDataType typeToken) {
  this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaSuccExpression::proxyType(StellaType type) {
  this->expression->proxyType(type);
}
void StellaSuccExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}
void StellaSuccExpression::proxyIdent(Stella::StellaIdent ident) {
  this->expression->proxyIdent(ident);
}
bool StellaSuccExpression::isParsed() {
  return this->expression != NULL && this->expression->isParsed();
}
