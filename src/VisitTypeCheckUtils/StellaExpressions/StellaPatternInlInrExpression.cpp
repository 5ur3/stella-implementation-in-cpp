#include "../StellaExpression.h"
#include <iostream>

StellaPatternInlInrExpression::StellaPatternInlInrExpression(int side) {
  this->side = side;
  this->type = STELLA_EXPRESSION_TYPE_PATTERN_INL_INR;
}
StellaType StellaPatternInlInrExpression::getStellaType() {
  return this->expression->getStellaType();
}
bool StellaPatternInlInrExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->expression->isPattern()) {
    std::cout << "Type error: pattern expression is not a pattern" << std::endl;
    isCorrect = false;
  } else if (!this->context[".matchcontext"].isSumType()) {
    std::string name;
    if (this->side == 1) {
      name = "inl";
    } else {
      name = "inr";
    }
    std::cout << "Type error: peforming pattern " << name << " on a "
              << this->context[".matchcontext"].toString()
              << ". Expected sum type" << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    if (side == 1) {
      std::cout << "\tin pattern inl" << std::endl;
    } else {
      std::cout << "\tin pattern inr" << std::endl;
    }
  }

  return isCorrect;
}
void StellaPatternInlInrExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaPatternInlInrExpression::proxyExpression(
    StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
    this->expression->addContext(
        ".matchcontext", this->context[".matchcontext"].getSubType(this->side));
  } else {
    this->expression->proxyExpression(expression);
  }
}
void StellaPatternInlInrExpression::proxyIdent(Stella::StellaIdent ident) {
  this->expression->proxyIdent(ident);
}
bool StellaPatternInlInrExpression::isParsed() {
  return this->expression != NULL && this->expression->isParsed();
}
bool StellaPatternInlInrExpression::isPattern() { return true; }
