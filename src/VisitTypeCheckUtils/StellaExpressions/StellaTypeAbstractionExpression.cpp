#include "../StellaExpression.h"
#include <iostream>

StellaTypeAbstractionExpression::StellaTypeAbstractionExpression(
    int typeVarsCount) {
  this->typeVars = std::vector<std::string>(typeVarsCount, "");
  this->type = STELLA_EXPRESSION_TYPE_TYPE_ABSTRACTION;
}

StellaType StellaTypeAbstractionExpression::getStellaType() {
  StellaType type(STELLA_DATA_TYPE_FORALL, this->typeVars.size());
  for (int i = 0; i < this->typeVars.size(); i++) {
    type.parseIdent(this->typeVars[i]);
  }
  type.proxyType(this->expression->getStellaType());

  return type;
}

bool StellaTypeAbstractionExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin type abstraction" << std::endl;
  }

  return isCorrect;
}

void StellaTypeAbstractionExpression::proxyIdent(Stella::StellaIdent ident) {
  for (int i = 0; i < this->typeVars.size(); i++) {
    if (this->typeVars[i] == "") {
      this->typeVars[i] = ident;
      return;
    }
  }
  
  return this->expression->proxyIdent(ident);
}

void StellaTypeAbstractionExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  return this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaTypeAbstractionExpression::proxyType(StellaType type) {
  this->expression->proxyType(type);
}

void StellaTypeAbstractionExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}

bool StellaTypeAbstractionExpression::isParsed() {
  for (int i = 0; i < this->typeVars.size(); i++) {
    if (this->typeVars[i] == "") {
      return false;
    }
  }
  return this->expression != NULL && this->expression->isParsed();
}
