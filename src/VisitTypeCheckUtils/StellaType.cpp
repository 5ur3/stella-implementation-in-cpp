#include "StellaType.h"
#include <deque>
#include <iostream>
#include <string>
#include <vector>

bool StellaType::isCompleted() {
  if (this->childrenCount != this->children.size()) {
    return false;
  }
  for (int i = 0; i < childrenCount; i++) {
    if (!this->children[i].isCompleted()) {
      return false;
    }
  }
  return true;
}

StellaType::StellaType(StellaDataType baseType) {
  this->type = baseType;

  switch (baseType) {
  case STELLA_DATA_TYPE_ANY:
    break;
  case STELLA_DATA_TYPE_BOOL:
    break;
  case STELLA_DATA_TYPE_NAT:
    break;
  case STELLA_DATA_TYPE_UNIT:
    break;
  case STELLA_DATA_TYPE_RECORD:
    break;
  case STELLA_DATA_TYPE_INVALID:
    break;
  case STELLA_DATA_TYPE_FUN:
    this->childrenCount = 2;
    break;
  case STELLA_DATA_TYPE_REF:
    this->childrenCount = 1;
    break;
  case STELLA_DATA_TYPE_SUM:
    this->childrenCount = 2;
    break;
  case STELLA_DATA_TYPE_TUPLE:
    this->childrenCount = 2;
    break;
  }
};
StellaType::StellaType() : StellaType(STELLA_DATA_TYPE_INVALID){};
StellaType::StellaType(StellaDataType baseType, StellaType arg)
    : StellaType(baseType) {
  this->children.push_back(arg);
};
StellaType::StellaType(StellaDataType baseType, StellaType arg1,
                       StellaType arg2)
    : StellaType(baseType) {
  this->children.push_back(arg1);
  this->children.push_back(arg2);
};
StellaType::StellaType(StellaDataType baseType, int childrenCount) {
  this->type = baseType;
  this->childrenCount = childrenCount;
};

void StellaType::parse(StellaDataType typeToken) {
  for (int i = 0; i < this->children.size(); i++) {
    if (!this->children[i].isCompleted()) {
      this->children[i].parse(typeToken);
      return;
    }
  }
  this->children.push_back(StellaType(typeToken));
}

void StellaType::proxyType(StellaType type) {
  for (int i = 0; i < this->children.size(); i++) {
    if (!this->children[i].isCompleted()) {
      this->children[i].proxyType(type);
      return;
    }
  }
  this->children.push_back(type);
}

void StellaType::parseIdent(std::string ident) {
  if (this->type == STELLA_DATA_TYPE_RECORD &&
      (this->children.size() == 0 ||
       this->children[this->children.size() - 1].isCompleted())) {
    this->names.push_back(ident);
    return;
  }

  this->children[this->children.size() - 1].parseIdent(ident);
}

bool StellaType::isRecord() {
  return this->type == STELLA_DATA_TYPE_RECORD ||
         this->type == STELLA_DATA_TYPE_ANY;
}

bool StellaType::isFunction() {
  return this->type == STELLA_DATA_TYPE_FUN ||
         this->type == STELLA_DATA_TYPE_ANY;
}

bool StellaType::isTuple() {
  return this->type == STELLA_DATA_TYPE_TUPLE ||
         this->type == STELLA_DATA_TYPE_ANY;
}

bool StellaType::isSumType() {
  return this->type == STELLA_DATA_TYPE_SUM ||
         this->type == STELLA_DATA_TYPE_ANY;
}

bool StellaType::isRefType() {
  return this->type == STELLA_DATA_TYPE_REF ||
         this->type == STELLA_DATA_TYPE_ANY;
}

StellaType StellaType::getSubType(int index) {
  if (this->type == STELLA_DATA_TYPE_ANY) {
    return StellaType(STELLA_DATA_TYPE_ANY);
  }

  if (index - 1 >= this->children.size() || !this->isCompleted()) {
    return StellaType(STELLA_DATA_TYPE_INVALID);
  }

  return this->children[index - 1];
}

StellaType StellaType::getParamType() { return this->getSubType(1); }

StellaType StellaType::getReturnType() { return this->getSubType(2); }

StellaType StellaType::getDerefType() { return this->getSubType(1); }

bool StellaType::castsTo(StellaType stellaType) {
  if (!this->isCompleted() || !stellaType.isCompleted()) {
    return false;
  }

  if (this->type == STELLA_DATA_TYPE_INVALID ||
      stellaType.type == STELLA_DATA_TYPE_INVALID) {
    return false;
  }

  if (this->type == STELLA_DATA_TYPE_ANY) {
    return true;
  }

  if (this->type == STELLA_DATA_TYPE_FUN &&
      stellaType.type == STELLA_DATA_TYPE_FUN) {
    if (this->childrenCount != stellaType.childrenCount) {
      return false;
    }

    for (int i = 1; i <= this->childrenCount; i++) {
      if (!stellaType.getSubType(i).castsTo(this->getSubType(i))) {
        return false;
      }
    }
    return true;
  }

  if (this->type == STELLA_DATA_TYPE_RECORD &&
      stellaType.type == STELLA_DATA_TYPE_RECORD) {
    for (int i = 0; i < stellaType.names.size(); i++) {
      for (int j = 0; j < this->names.size(); j++) {
        if (stellaType.names[i] == this->names[j]) {
          if (!this->children[j].castsTo(stellaType.children[i])) {
            return false;
          }
          break;
        }

        if (j == this->names.size() - 1) {
          return false;
        }
      }
    }

    return true;
  }

  if (this->childrenCount != stellaType.childrenCount) {
    return false;
  }

  if (this->type == stellaType.type) {
    for (int i = 1; i <= this->childrenCount; i++) {
      if (!this->getSubType(i).castsTo(stellaType.getSubType(i))) {
        return false;
      }
    }
    return true;
  }

  return false;
}

bool StellaType::biCasts(StellaType stellaType) {
  return this->castsTo(stellaType) || stellaType.castsTo(*this);
}

std::string StellaType::toString() {
  std::string typeString;
  switch (this->type) {
  case STELLA_DATA_TYPE_ANY:
    return "any";
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
  case STELLA_DATA_TYPE_REF:
    typeString = "ref";
    break;
  case STELLA_DATA_TYPE_RECORD:
    typeString = "record";
    break;
  case STELLA_DATA_TYPE_INVALID:
    typeString = "INVALID";
    break;
  }

  if (this->children.size() > 0) {
    typeString += "(";

    for (int i = 0; i < this->children.size(); i++) {
      if (this->type == STELLA_DATA_TYPE_RECORD) {
        typeString += this->names[i] + ": ";
      }
      typeString += this->children[i].toString() + ", ";
    }

    typeString += ")";
  }

  return typeString;
}

StellaType mergeTypes(StellaType type1, StellaType type2) {
  if (!type1.biCasts(type2)) {
    return StellaType(STELLA_DATA_TYPE_INVALID);
  }

  if (type1.type == STELLA_DATA_TYPE_ANY) {
    return type2;
  }
  if (type2.type == STELLA_DATA_TYPE_ANY) {
    return type1;
  }

  if (type1.childrenCount == 0) {
    return type1;
  }

  StellaType res = StellaType(type1.type);
  for (int i = 0; i < type1.childrenCount; i++) {
    res.children.push_back(
        mergeTypes(type1.getSubType(i + 1), type2.getSubType(i + 1)));
  }
  return res;
}
