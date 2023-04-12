#ifndef STELLA_TYPE_HEADER
#define STELLA_TYPE_HEADER

#include <deque>
#include <string>
#include <vector>

enum StellaDataType {
  STELLA_DATA_TYPE_UNIT = 0,
  STELLA_DATA_TYPE_BOOL = 1,
  STELLA_DATA_TYPE_NAT = 2,
  STELLA_DATA_TYPE_FUN = 3,
  STELLA_DATA_TYPE_TUPLE = 4,
  STELLA_DATA_TYPE_SUM = 5,

  STELLA_DATA_TYPE_ANY = 100
};

// Representation of a stella type. Supports "==" and "!=" operators
class StellaType {
private:
  bool isCompletedRecursive(std::deque<StellaDataType> &remainingTokens);

public:
  // The actual type information is stored in a vector.
  // type_vector of [fun, fun, nat, nat, bool] is equivalent to
  // stella type of "fn(fn(Nat) -> Nat) -> Bool"
  std::vector<StellaDataType> typeVector;

  // Types from stella source have to be built token-by-token.
  // Pass here StellaDataType, and this will add it
  // to the type vector
  void parse(StellaDataType typeToken);

  // Since types are often constructed token-by-token, this method is used to
  // check if the type is complete.
  // For example, it returns true for type [fun, fun, nat, nat, bool],
  // and false for type [fun, fun, nat, nat]
  bool isCompleted();

  // Default constructor, for token-by-token construction
  StellaType();
  // Sort of a constant type constructor. Used as StellaType(STELLA_DATA_TYPE_NAT) or
  // StellaType(STELLA_DATA_TYPE_NAT)
  StellaType(StellaDataType dataType);
  // Function constructor
  StellaType(StellaType baseType, StellaType arg1, StellaType arg2);

  bool isEqual(StellaType stellaType);
  std::string toString();

  // This method is used to check whether the type is atomic (nat, bool, ...) or composite (fun, tuple, ...)
  bool isComposite();

  // If the type is composite, you can get subtypes
  StellaType getSubType(int index);

  // This method is used to check whether the type is a function
  bool isFunction();

  // If the type is a function, you can get param and return types
  StellaType getParamType();
  StellaType getReturnType();

  // This method is used to check whether the type is a tuple (2 elements only)
  bool isTuple();

  // If the type is a tuple, you can get types of the components
  // (index can be 1 or 2)
  StellaType getTupleType(int index);

  // This method is used to check whether the type is a sum type
  bool isSumType();

  // If the type is a sum type, you can get types of the components
  // (index can be 1 or 2)
  StellaType getSumType(int index);
};

#endif