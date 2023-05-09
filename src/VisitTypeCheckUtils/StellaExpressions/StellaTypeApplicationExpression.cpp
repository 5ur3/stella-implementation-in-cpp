#include "../StellaExpression.h"
#include <iostream>

StellaTypeApplicationExpression::StellaTypeApplicationExpression(
    int typesCount) {
  this->types = StellaType(STELLA_DATA_TYPE_INVALID);
  this->types.childrenCount = typesCount;
  this->type = STELLA_EXPRESSION_TYPE_TYPE_APPLICATION;
}

StellaType StellaTypeApplicationExpression::getStellaType() {
  return this->expression->getStellaType().getSubstitutionType(
      this->types.children);
}

bool StellaTypeApplicationExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->expression->getStellaType().isForallType()) {
    std::cout << "Type error: expected a forall type. Got "
              << this->expression->getStellaType().toString() << std::endl;
    isCorrect = false;
  } else if (this->types.children.size() !=
             this->expression->getStellaType().names.size()) {
    std::cout << "Type error: expected a forall type with "
              << this->types.children.size() << " args. Got a forall type with "
              << this->expression->getStellaType().names.size() << " args"
              << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin type application" << std::endl;
  }

  return isCorrect;
}

void StellaTypeApplicationExpression::proxyIdent(Stella::StellaIdent ident) {
  if (!this->types.isCompleted()) {
    return this->types.parseIdent(ident);
  }
  return this->expression->proxyIdent(ident);
}

void StellaTypeApplicationExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  if (!this->types.isCompleted()) {
    return this->types.parse(typeToken);
  }
  return this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaTypeApplicationExpression::proxyType(StellaType type) {
  if (!this->types.isCompleted()) {
    return this->types.proxyType(type);
  }
  this->expression->proxyType(type);
}

void StellaTypeApplicationExpression::proxyExpression(
    StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}

bool StellaTypeApplicationExpression::isParsed() {
  return this->types.isCompleted() && this->expression != NULL &&
         this->expression->isParsed();
}
