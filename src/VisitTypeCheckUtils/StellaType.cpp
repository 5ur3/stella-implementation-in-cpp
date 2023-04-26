#include "StellaType.h"
#include <deque>
#include <iostream>
#include <string>
#include <vector>

// Forward-declaration of one of state machine controlling methods from
// VisitTypeCheck.cpp
void onTypeParsingEnd();

// Documentation of the class methods is in StellaType.h
bool StellaType::isCompletedRecursive(
    std::deque<StellaDataType> &remainingTokens) {
  if (remainingTokens.empty()) {
    return false;
  }

  StellaDataType token = remainingTokens.front();
  remainingTokens.pop_front();

  switch (token) {
  case STELLA_DATA_TYPE_UNIT:
    return true;
  case STELLA_DATA_TYPE_BOOL:
    return true;
  case STELLA_DATA_TYPE_NAT:
    return true;
  case STELLA_DATA_TYPE_ANY:
    return true;
  case STELLA_DATA_TYPE_FUN:
    return isCompletedRecursive(remainingTokens) &&
           isCompletedRecursive(remainingTokens);
  case STELLA_DATA_TYPE_TUPLE:
    return isCompletedRecursive(remainingTokens) &&
           isCompletedRecursive(remainingTokens);
  case STELLA_DATA_TYPE_SUM:
    return isCompletedRecursive(remainingTokens) &&
           isCompletedRecursive(remainingTokens);
  }
}

bool StellaType::isCompleted() {
  std::deque<StellaDataType> tokens;
  tokens.insert(tokens.begin(), this->typeVector.begin(),
                this->typeVector.end());
  return isCompletedRecursive(tokens);
}

StellaType::StellaType(){};
StellaType::StellaType(StellaDataType token) {
  this->typeVector.push_back(token);
};
StellaType::StellaType(StellaType baseType, StellaType arg1, StellaType arg2) {
  this->typeVector.insert(this->typeVector.end(), baseType.typeVector.begin(),
                          baseType.typeVector.end());
  this->typeVector.insert(this->typeVector.end(), arg1.typeVector.begin(),
                          arg1.typeVector.end());
  this->typeVector.insert(this->typeVector.end(), arg2.typeVector.begin(),
                          arg2.typeVector.end());
};

void StellaType::parse(StellaDataType typeToken) {
  this->typeVector.push_back(typeToken);

  if (isCompleted()) {
    onTypeParsingEnd();
  }
}

bool StellaType::isFunction() {
  return this->isCompleted() && this->typeVector[0] == STELLA_DATA_TYPE_FUN;
}

bool StellaType::isTuple() {
  return this->isCompleted() && this->typeVector[0] == STELLA_DATA_TYPE_TUPLE;
}

bool StellaType::isSumType() {
  return this->isCompleted() && this->typeVector[0] == STELLA_DATA_TYPE_SUM;
}

bool StellaType::isComposite() {
  return this->isFunction() || this->isTuple() || this->isSumType();
}

StellaType StellaType::getSubType(int index) {
  int counter = 1;
  StellaType type;
  for (int i = 0; i < index; i++) {
    type = StellaType();
    for (; !type.isCompleted(); counter++) {
      type.parse(this->typeVector[counter]);
    }
  }
  return type;
}

StellaType StellaType::getParamType() {
  if (this->isFunction()) {
    return this->getSubType(1);
  }
  return StellaType();
}

StellaType StellaType::getReturnType() {
  if (this->isFunction()) {
    return this->getSubType(2);
  }
  return StellaType();
}

StellaType StellaType::getTupleType(int index) {
  if (this->isTuple()) {
    return this->getSubType(index);
  }
  return StellaType();
}

StellaType StellaType::getSumType(int index) {
  if (this->isSumType()) {
    return this->getSubType(index);
  }
  return StellaType();
}

bool StellaType::isEqual(StellaType stellaType) {
  if (!this->isCompleted() || !stellaType.isCompleted()) {
    return false;
  }

  if (this->typeVector[0] == STELLA_DATA_TYPE_ANY ||
      stellaType.typeVector[0] == STELLA_DATA_TYPE_ANY) {
    return true;
  }

  if (this->isComposite() != stellaType.isComposite()) {
    return false;
  }

  if (!this->isComposite() && this->typeVector[0] == stellaType.typeVector[0]) {
    return true;
  }

  if (this->typeVector[0] == stellaType.typeVector[0]) {
    return this->getSubType(1).isEqual(stellaType.getSubType(1)) &&
           this->getSubType(2).isEqual(stellaType.getSubType(2));
  }

  return false;
}

std::string StellaType::toString() {
  if (!this->isCompleted()) {
    return "";
  }

  std::string typeString;
  switch (this->typeVector[0]) {
  case STELLA_DATA_TYPE_ANY:
    typeString = "any";
    break;
  case STELLA_DATA_TYPE_FUN:
    typeString = "fun";
    break;
  case STELLA_DATA_TYPE_SUM:
    typeString = "sum";
    break;
  case STELLA_DATA_TYPE_TUPLE:
    typeString = "tuple";
    break;
  case STELLA_DATA_TYPE_BOOL:
    typeString = "bool";
    break;
  case STELLA_DATA_TYPE_NAT:
    typeString = "nat";
    break;
  case STELLA_DATA_TYPE_UNIT:
    typeString = "unit";
    break;
  }

  if (!this->isComposite()) {
    return typeString;
  }

  return typeString + "(" + this->getSubType(1).toString() + ", " +
         this->getSubType(2).toString() + ")";
}

StellaType mergeTypes(StellaType type1, StellaType type2) {
  if (!type1.isEqual(type2)) {
    return StellaType();
  }

  if (type1.typeVector[0] == STELLA_DATA_TYPE_ANY) {
    return type2;
  }
  if (type2.typeVector[0] == STELLA_DATA_TYPE_ANY) {
    return type1;
  }

  if (!type1.isComposite()) {
    return type1;
  }

  return StellaType(StellaType(type1.typeVector[0]),
                    mergeTypes(type1.getSubType(1), type2.getSubType(1)),
                    mergeTypes(type1.getSubType(2), type2.getSubType(2)));
}
