#include "../StellaExpression.h"
#include <iostream>

StellaDotTupleExpression::StellaDotTupleExpression(int index) {
  this->index = index;
  this->type = STELLA_EXPRESSION_TYPE_DOT_TUPLE;
}

StellaType StellaDotTupleExpression::getStellaType() {
  return expression->getStellaType().getTupleType(this->index);
}

bool StellaDotTupleExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!expression->getStellaType().isTuple()) {
    std::cout << "Type error: accessing a component of not a tuple"
              << std::endl;
    isCorrect = false;
  } else if (index < 1 || index > 2) {
    std::cout << "Type error: tuple access index is out of range" << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin dot tuple" << std::endl;
  }

  return isCorrect;
}

void StellaDotTupleExpression::proxyIdent(Stella::StellaIdent ident) {
  return this->expression->proxyIdent(ident);
}

void StellaDotTupleExpression::proxyExpressionTypeToken(StellaDataType typeToken) {
  return this->expression->proxyExpressionTypeToken(typeToken);
}

void StellaDotTupleExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}

bool StellaDotTupleExpression::isParsed() {
  return this->expression != NULL && this->expression->isParsed();
}