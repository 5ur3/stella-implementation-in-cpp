#ifndef STELLA_TYPE_HEADER
#define STELLA_TYPE_HEADER

#include <deque>
#include <map>
#include <string>
#include <vector>

enum StellaDataType {
  STELLA_DATA_TYPE_UNIT = 0,
  STELLA_DATA_TYPE_BOOL = 1,
  STELLA_DATA_TYPE_NAT = 2,
  STELLA_DATA_TYPE_FUN = 3,
  STELLA_DATA_TYPE_TUPLE = 4,
  STELLA_DATA_TYPE_SUM = 5,
  STELLA_DATA_TYPE_REF = 6,
  STELLA_DATA_TYPE_RECORD = 7,

  STELLA_DATA_TYPE_ANY = 100,

  STELLA_DATA_TYPE_INVALID = -1
};

class StellaType {
public:
  StellaDataType type;

  int childrenCount = 0;
  std::vector<StellaType> children;
  std::vector<std::string> names;

  void parse(StellaDataType typeToken);
  void proxyType(StellaType type);
  void parseIdent(std::string ident);

  bool isCompleted();

  StellaType(StellaDataType dataType);
  StellaType();
  StellaType(StellaDataType baseType, StellaType arg);
  StellaType(StellaDataType baseType, StellaType arg1, StellaType arg2);
  StellaType(StellaDataType baseType, int childrenCount);

  bool castsTo(StellaType stellaType);
  bool biCasts(StellaType stellaType);
  std::string toString();

  bool isFunction();
  bool isRecord();
  bool isTuple();
  bool isSumType();
  bool isRefType();

  StellaType getSubType(int index);
  StellaType getParamType();
  StellaType getReturnType();
  StellaType getDerefType();
};

StellaType mergeTypes(StellaType type1, StellaType type2);

#endif