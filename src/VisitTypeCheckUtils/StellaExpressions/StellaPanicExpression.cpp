#include "../StellaExpression.h"

StellaPanicExpression::StellaPanicExpression() {
  this->type = STELLA_EXPRESSION_TYPE_PANIC;
}

StellaType StellaPanicExpression::getStellaType() {
  return StellaType(STELLA_DATA_TYPE_ANY);
}

bool StellaPanicExpression::isTypingCorrect() { return true; }
