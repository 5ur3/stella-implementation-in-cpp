#include "../StellaExpression.h"
#include <iostream>

StellaDerefExpression::StellaDerefExpression() {
  this->type = STELLA_EXPRESSION_TYPE_DEREF;
}
StellaType StellaDerefExpression::getStellaType() {
  return this->expression->getStellaType().getDerefType();
}
bool StellaDerefExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->expression->getStellaType().isRefType()) {
    std::cout << "Type error: dereferencing a " << this->expression->getStellaType().toString() << " type" << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin deref" << std::endl;
  }

  return isCorrect;
}
void StellaDerefExpression::proxyExpressionTypeToken(StellaDataType typeToken) {
  this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaDerefExpression::proxyType(StellaType type) {
  this->expression->proxyType(type);
}
void StellaDerefExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}
void StellaDerefExpression::proxyIdent(Stella::StellaIdent ident) {
  this->expression->proxyIdent(ident);
}
bool StellaDerefExpression::isParsed() {
  return this->expression != NULL && this->expression->isParsed();
}
