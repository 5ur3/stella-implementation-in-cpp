#include "../StellaExpression.h"

StellaConstBoolExpression::StellaConstBoolExpression() {
  this->type = STELLA_EXPRESSION_TYPE_CONST_BOOL;
}

StellaType StellaConstBoolExpression::getStellaType() {
  return StellaType(STELLA_DATA_TYPE_BOOL);
}

bool StellaConstBoolExpression::isTypingCorrect() { return true; }
