#include "../StellaExpression.h"

StellaConstUnitExpression::StellaConstUnitExpression() {
  this->type = STELLA_EXPRESSION_TYPE_CONST_UNIT;
}
StellaType StellaConstUnitExpression::getStellaType() {
  return StellaType("unit");
}
bool StellaConstUnitExpression::isTypingCorrect() { return true; }
