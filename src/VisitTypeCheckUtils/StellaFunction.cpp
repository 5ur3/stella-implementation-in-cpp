#include "StellaFunction.h"
#include "StellaType.h"
#include <iostream>

StellaFunction::StellaFunction(
    std::map<Stella::StellaIdent, StellaType> context, int genericsCount) {
  this->context = context;
  if (genericsCount) {
    this->type = StellaType(STELLA_DATA_TYPE_FORALL, genericsCount);
    this->type.children.push_back(StellaType(STELLA_DATA_TYPE_FUN));
  }
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
  if (this->ident.size() == 0) {
    this->ident = ident;
    return;
  }
  if (this->type.names.size() > 0 && this->type.names[this->type.names.size() - 1] == "") {
    return this->type.parseIdent(ident);
  }
  if (this->paramIdent.size() == 0) {
    this->paramIdent = ident;
    return;
  }
  if (!this->type.isCompleted()) {
    this->type.parseIdent(ident);
    return;
  }
  this->expression->proxyIdent(ident);
}
bool StellaFunction::isTypingCorrect() {
  bool isCorrect = true;
  if (!this->expression->isTypingCorrect() || !this->type.areVarsDefined()) {
    isCorrect = false;
  } else if (!this->expression->getStellaType().castsTo(
                 this->type.getReturnType())) {
    std::cout << "Type error: function return and function expression types "
                 "are mismatched. Expected to return "
              << this->type.getReturnType().toString() << ". Got "
              << this->expression->getStellaType().toString() << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin function \"" << this->ident << "\"" << std::endl;
  }
  return isCorrect;
}