#include "../StellaExpression.h"
#include <iostream>

StellaInlInrExpression::StellaInlInrExpression(int side) {
  this->side = side;
  this->type = STELLA_EXPRESSION_TYPE_INL_INR;
}
StellaType StellaInlInrExpression::getStellaType() {
  if (side == 1) {
    return StellaType(STELLA_DATA_TYPE_SUM, this->expression->getStellaType(),
                      StellaType(STELLA_DATA_TYPE_ANY));
  }
  return StellaType(STELLA_DATA_TYPE_SUM, StellaType(STELLA_DATA_TYPE_ANY),
                    this->expression->getStellaType());
}
bool StellaInlInrExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  }

  if (!isCorrect) {
    if (side == 1) {
      std::cout << "\tin inl" << std::endl;
    } else {
      std::cout << "\tin inr" << std::endl;
    }
  }

  return isCorrect;
}
void StellaInlInrExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaInlInrExpression::proxyType(StellaType type) {
  this->expression->proxyType(type);
}
void StellaInlInrExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}
void StellaInlInrExpression::proxyIdent(Stella::StellaIdent ident) {
  this->expression->proxyIdent(ident);
}
bool StellaInlInrExpression::isParsed() {
  return this->expression != NULL && this->expression->isParsed();
}
