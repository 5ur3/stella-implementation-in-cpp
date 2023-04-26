#include "../StellaExpression.h"
#include <iostream>

StellaMatchExpression::StellaMatchExpression(int size) {
  this->expressions = std::vector<StellaExpression *>(size, NULL);
  this->type = STELLA_EXPRESSION_TYPE_MATCH;
}

StellaType StellaMatchExpression::getStellaType() {
  StellaType type = this->expressions[0]->getStellaType();

  for (int i = 1; i < this->expressions.size(); i++) {
    type = mergeTypes(type, this->expressions[i]->getStellaType());
  }

  return type;
}

bool StellaMatchExpression::isTypingCorrect() {
  if (!this->isParsed()) {
    return false;
  }

  bool areInnerExpressionsCorrect = this->matchExpression->isTypingCorrect();
  for (int i = 0; i < this->expressions.size(); i++) {
    if (!this->expressions[i]->isTypingCorrect()) {
      areInnerExpressionsCorrect = false;
      break;
    }
  }

  bool doInnerExpressionTypesMatch = true;
  if (areInnerExpressionsCorrect) {
    StellaType innerExpressionsType = expressions[0]->getStellaType();
    for (int i = 1; i < this->expressions.size(); i++) {
      if (!innerExpressionsType.isEqual(
              this->expressions[i]->getStellaType())) {
        std::cout << "Type error: unable to infer type of match expression "
                     "(match case expression types are mismatched)"
                  << std::endl;
        std::cout << "\tNo supertype for " << innerExpressionsType.toString()
                  << " and " << this->expressions[i]->getStellaType().toString()
                  << std::endl;
        doInnerExpressionTypesMatch = false;
        break;
      }
      innerExpressionsType =
          mergeTypes(innerExpressionsType, expressions[i]->getStellaType());
    }
  }

  bool isCorrect = areInnerExpressionsCorrect && doInnerExpressionTypesMatch;

  if (!isCorrect) {
    std::cout << "\tin match" << std::endl;
  }

  return isCorrect;
}

void StellaMatchExpression::proxyIdent(Stella::StellaIdent ident) {
  if (!this->matchExpression->isParsed()) {
    return this->matchExpression->proxyIdent(ident);
  }
  for (int i = 0; i < this->expressions.size(); i++) {
    if (!this->expressions[i]->isParsed()) {
      return this->expressions[i]->proxyIdent(ident);
    }
  }
}

void StellaMatchExpression::proxyExpressionTypeToken(StellaDataType typeToken) {
  if (!this->matchExpression->isParsed()) {
    return this->matchExpression->proxyExpressionTypeToken(typeToken);
  }
  for (int i = 0; i < this->expressions.size(); i++) {
    if (!this->expressions[i]->isParsed()) {
      return this->expressions[i]->proxyExpressionTypeToken(typeToken);
    }
  }
}

void StellaMatchExpression::proxyExpression(StellaExpression *expression) {
  if (this->matchExpression == NULL) {
    this->matchExpression = expression;
    this->matchExpression->setContext(this->context);
  } else if (!this->matchExpression->isParsed()) {
    this->matchExpression->proxyExpression(expression);
  } else {
    for (int i = 0; i < this->expressions.size(); i++) {
      if (this->expressions[i] == NULL) {
        this->expressions[i] = expression;
        this->expressions[i]->setContext(this->context);
        this->expressions[i]->addContext(".matchcontext", matchExpression->getStellaType());
        return;
      }
      if (!this->expressions[i]->isParsed()) {
        return this->expressions[i]->proxyExpression(expression);
        return;
      }
    }
  }
}

bool StellaMatchExpression::isParsed() {
  if (this->matchExpression == NULL || !this->matchExpression->isParsed()) {
    return false;
  }
  for (int i = 0; i < this->expressions.size(); i++) {
    if (this->expressions[i] == NULL || !this->expressions[i]->isParsed()) {
      return false;
    }
  }
  return true;
}