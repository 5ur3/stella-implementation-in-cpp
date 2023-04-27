#include "../StellaExpression.h"
#include <iostream>

StellaSequenceExpression::StellaSequenceExpression() {
  this->type = STELLA_EXPRESSION_TYPE_SEQUENCE;
}

StellaType StellaSequenceExpression::getStellaType() {
  return this->expression2->getStellaType();
}

bool StellaSequenceExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression1->isTypingCorrect() ||
      !this->expression2->isTypingCorrect()) {
    isCorrect = false;
  } else if (!this->expression1->getStellaType().castsTo(StellaType(STELLA_DATA_TYPE_UNIT))) {
    std::cout << "Type error: first expression type is \"" << this->expression1->getStellaType().toString() << "\". Expected Unit type" << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin sequence" << std::endl;
  }

  return isCorrect;
}

void StellaSequenceExpression::proxyIdent(Stella::StellaIdent ident) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyIdent(ident);
  }
  return this->expression2->proxyIdent(ident);
}

void StellaSequenceExpression::proxyExpressionTypeToken(StellaDataType typeToken) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyExpressionTypeToken(typeToken);
  }
  return this->expression2->proxyExpressionTypeToken(typeToken);
}
void StellaSequenceExpression::proxyType(StellaType type) {
  if (!this->expression1->isParsed()) {
    return this->expression1->proxyType(type);
  }
  return this->expression2->proxyType(type);
}
void StellaSequenceExpression::proxyExpression(StellaExpression *expression) {
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

bool StellaSequenceExpression::isParsed() {
  return this->expression1 != NULL && this->expression2 != NULL &&
         this->expression1->isParsed() && this->expression2->isParsed();
}
