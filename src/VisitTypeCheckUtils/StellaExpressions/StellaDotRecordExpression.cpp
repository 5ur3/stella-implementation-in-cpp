#include "../StellaExpression.h"
#include <iostream>

StellaDotRecordExpression::StellaDotRecordExpression(std::string field) {
  this->field = field;
  this->type = STELLA_EXPRESSION_TYPE_DOT_RECORD;
}

StellaType StellaDotRecordExpression::getStellaType() {
  for (int i = 0; i < expression->getStellaType().names.size(); i++) {
    if (expression->getStellaType().names[i] == this->field) {
      return expression->getStellaType().children[i];
    }
  }
  return StellaType(STELLA_DATA_TYPE_INVALID);
}

bool StellaDotRecordExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool keyExists = false;
  for (int i = 0; i < expression->getStellaType().names.size(); i++) {
    if (expression->getStellaType().names[i] == this->field) {
      keyExists = true;
    }
  }

  bool isCorrect = true;
  if (!this->expression->isTypingCorrect()) {
    isCorrect = false;
  } else if (!expression->getStellaType().isRecord()) {
    std::cout << "Type error: accessing a component of not a record"
              << std::endl;
    isCorrect = false;
  } else if (!keyExists) {
    std::cout << "Type error: field \"" << this->field
              << "\" does not exist on record \""
              << this->expression->getStellaType().toString() << "\""
              << std::endl;
    isCorrect = false;
  }

  if (!isCorrect) {
    std::cout << "\tin dot record" << std::endl;
  }

  return isCorrect;
}

void StellaDotRecordExpression::proxyIdent(Stella::StellaIdent ident) {
  return this->expression->proxyIdent(ident);
}

void StellaDotRecordExpression::proxyExpressionTypeToken(
    StellaDataType typeToken) {
  return this->expression->proxyExpressionTypeToken(typeToken);
}
void StellaDotRecordExpression::proxyType(StellaType type) {
  this->expression->proxyType(type);
}

void StellaDotRecordExpression::proxyExpression(StellaExpression *expression) {
  if (this->expression == NULL) {
    this->expression = expression;
    this->expression->setContext(this->context);
  } else {
    this->expression->proxyExpression(expression);
  }
}

bool StellaDotRecordExpression::isParsed() {
  return this->expression != NULL && this->expression->isParsed();
}