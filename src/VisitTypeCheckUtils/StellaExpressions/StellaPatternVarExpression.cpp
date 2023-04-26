#include "../StellaExpression.h"
#include <iostream>

StellaPatternVarExpression::StellaPatternVarExpression(
    Stella::StellaIdent ident) {
  this->type = STELLA_EXPRESSION_TYPE_PATTERN_VAR;
  this->ident = ident;
}
StellaType StellaPatternVarExpression::getStellaType() {
  return this->context[".matchcontext"];
}
bool StellaPatternVarExpression::isTypingCorrect() { return true; }
bool StellaPatternVarExpression::isPattern() { return true; }
