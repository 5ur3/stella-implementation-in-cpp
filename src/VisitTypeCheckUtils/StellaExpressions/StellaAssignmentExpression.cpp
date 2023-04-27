#include "../StellaExpression.h"
#include <iostream>

StellaAssignmentExpression::StellaAssignmentExpression() {
  this->type = STELLA_EXPRESSION_TYPE_ASSIGNMENT;
}

StellaType StellaAssignmentExpression::getStellaType() {
  return StellaType(STELLA_DATA_TYPE_UNIT);
}

bool StellaAssignmentExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression1->isTypingCorrect() ||
      !this->expression2->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->expression1->getStellaType().isRefType()) {
    std::cout << "Type error: assigning to \""
              << this->expression1->getStellaType().toString()
              << "\". Expected Ref type" << std::endl;
    isCorrect = false;
  } else if (!this->expression2->getStellaType().castsTo(
                 this->expression1->getStellaType().getDerefType())) {
    std::cout << "Type error: assigning to \""
              << this->expression1->getStellaType().toString() << "\" type \""
              << this->expression2->getStellaType().toString()
              << "\". Expected type \""
              << this->expression1->getStellaType().getDerefType().toString()
              << "\"" << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin assignment" << std::endl;
  }

  return isCorrect;
}

void StellaAssignmentExpression::proxyIdent(Stella::StellaIdent ident) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyIdent(ident);
  }
  return this->expression2->proxyIdent(ident);
}

void StellaAssignmentExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyExpressionTypeToken(typeToken);
  }
  return this->expression2->proxyExpressionTypeToken(typeToken);
}
void StellaAssignmentExpression::proxyType(
    StellaType type) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyType(type);
  }
  this->expression2->proxyType(type);
}

void StellaAssignmentExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression1 == NULL) {
    this->expression1 = expression;
    this->expression1->setContext(this->context);
  } else if (!this->expression1->isParsed()) {
    this->expression1->proxyExpression(expression);
  } else if (this->expression2 == NULL) {
    this->expression2 = expression;
    this->expression2->setContext(this->context);
  } else {
    this->expression2->proxyExpression(expression);
  }
}

bool StellaAssignmentExpression::isParsed() {
  return this->expression1 != NULL && this->expression2 != NULL &&
         this->expression1->isParsed() && this->expression2->isParsed();
}
