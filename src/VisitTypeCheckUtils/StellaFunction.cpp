#include "StellaFunction.h"
#include "StellaType.h"
#include <iostream>

StellaFunction::StellaFunction(
    std::map<Stella::StellaIdent, StellaType> context) {
  this->context = context;
}
void StellaFunction::proxyExpressionTypeToken(StellaDataType typeToken) {
  if (!this->type.isCompleted()) {
    return this->type.parse(typeToken);
  }
  this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaFunction::proxyType(StellaType type) {
  if (!this->type.isCompleted()) {
    return this->type.proxyType(type);
  }
  this->expression->proxyType(type);
}
void StellaFunction::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
    this->expression->addContext(this->paramIdent, this->type.getParamType());
    return;
  }
  this->expression->proxyExpression(expression);
}
void StellaFunction::proxyIdent(Stella::StellaIdent ident) {
  std::cout << "FUN ident proxying: " << ident << std::endl;
  if (this->ident.size() == 0) {
    std::cout << 1 << std::endl;
    this->ident = ident;
    return;
  }
  if (this->paramIdent.size() == 0) {
    std::cout << 2 << std::endl;
    this->paramIdent = ident;
    return;
  }
  if (!this->type.isCompleted()) {
    std::cout << this->type.toString() << std::endl;
    std::cout << 3 << std::endl;
    this->type.parseIdent(ident);
    return;
  }
  std::cout << 4 << std::endl;
  this->expression->proxyIdent(ident);
}
bool StellaFunction::isTypingCorrect() {
  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->expression->getStellaType().castsTo(
                 this->type.getReturnType())) {
    std::cout << "Type error: function return and function expression types "
                 "are mismatched"
              << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin function \"" << this->ident << "\"" << std::endl;
  }
  return isCorrect;
}