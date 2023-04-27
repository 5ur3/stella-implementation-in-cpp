#include "../StellaExpression.h"
#include <iostream>

StellaAbstractionExpression::StellaAbstractionExpression() {
  this->type = STELLA_EXPRESSION_TYPE_ABSTRACTION;
}
StellaType StellaAbstractionExpression::getStellaType() {
  return StellaType(STELLA_DATA_TYPE_FUN, this->stellaType.children[0],
                    this->expression->getStellaType());
}
bool StellaAbstractionExpression::isTypingCorrect() {
  bool isCorrect = this->expression->isTypingCorrect();

  if (!isCorrect) {
    std::cout << "\tin abstraction" << std::endl;
  }

  return isCorrect;
}
void StellaAbstractionExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  if (this->stellaType.children.size() == 0 ||
      !this->stellaType.children[0].isCompleted()) {
    this->stellaType.parse(typeToken);
  } else {
    this->expression->proxyExpressionTypeToken(typeToken);
  }
}
void StellaAbstractionExpression::proxyType(StellaType type) {
  if (this->stellaType.children.size() == 0 ||
      !this->stellaType.children[0].isCompleted()) {
    this->stellaType.proxyType(type);
  } else {
    this->expression->proxyType(type);
  }
}
void StellaAbstractionExpression::proxyExpression(
    StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
    this->expression->addContext(this->paramIdent,
                                 this->stellaType.children[0]);
  } else {
    this->expression->proxyExpression(expression);
  }
}
void StellaAbstractionExpression::proxyIdent(Stella::StellaIdent ident) {
  std::cout << "HERE" << std::endl;
  if (this->paramIdent == "") {
    this->paramIdent = ident;
  } else if (!this->stellaType.children[0].isCompleted()) {
    this->stellaType.parseIdent(ident);
  } else {
    this->expression->proxyIdent(ident);
  }
}
bool StellaAbstractionExpression::isParsed() {
  return this->paramIdent != "" && this->stellaType.children.size() > 0 &&
         this->stellaType.children[0].isCompleted() &&
         this->expression != NULL && this->expression->isParsed();
}
