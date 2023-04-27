#include "../StellaExpression.h"
#include <iostream>

StellaTupleExpression::StellaTupleExpression() {
  this->type = STELLA_EXPRESSION_TYPE_TUPLE;
}

StellaType StellaTupleExpression::getStellaType() {
  return StellaType(STELLA_DATA_TYPE_TUPLE, this->expression1->getStellaType(),
                    this->expression2->getStellaType());
}

bool StellaTupleExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression1->isTypingCorrect() ||
      !this->expression2->isTypingCorrect()) {
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin tuple" << std::endl;
  }

  return isCorrect;
}

void StellaTupleExpression::proxyIdent(Stella::StellaIdent ident) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyIdent(ident);
  }
  return this->expression2->proxyIdent(ident);
}

void StellaTupleExpression::proxyExpressionTypeToken(StellaDataType typeToken) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyExpressionTypeToken(typeToken);
  }
  return this->expression2->proxyExpressionTypeToken(typeToken);
}

void StellaTupleExpression::proxyType(StellaType type) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyType(type);
  }
  return this->expression2->proxyType(type);
}

void StellaTupleExpression::proxyExpression(StellaExpression *expression) {
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

bool StellaTupleExpression::isParsed() {
  return this->expression1 != NULL && this->expression2 != NULL &&
         this->expression1->isParsed() && this->expression2->isParsed();
}
