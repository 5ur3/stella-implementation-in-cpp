cmake_minimum_required(VERSION 3.19.0)
project(stella-interpreter VERSION 0.1.0)

set (Stella_Syntax
  src/Stella/Absyn.H
  src/Stella/Absyn.C
  src/Stella/Parser.H
  src/Stella/Parser.C
  src/Stella/ParserError.H
  src/Stella/Printer.H
  src/Stella/Printer.C
  src/Stella/Buffer.H
  src/Stella/Buffer.C
  src/Stella/Bison.H
  src/Stella/Lexer.C
)

set (SRCS
  src/Interpret.cpp
  src/Eval.h
  src/Eval.cpp
  src/TypeCheck.h
  src/TypeCheck.cpp
  src/VisitTypeCheckUtils/StellaType.h
  src/VisitTypeCheckUtils/StellaType.cpp
  src/VisitTypeCheckUtils/StellaExpression.h
  src/VisitTypeCheckUtils/StellaExpressions/StellaConstIntExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaConstBoolExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaConstUnitExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaVarExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaAbstractionExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaApplicationExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaConditionExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaSuccExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaNatRecExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaTupleExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaDotTupleExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaInlInrExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaMatchExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaMatchCaseExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaPatternInlInrExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaPatternVarExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaSequenceExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaRefExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaDerefExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaAssignmentExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaPanicExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaRecordExpression.cpp
  src/VisitTypeCheckUtils/StellaExpressions/StellaDotRecordExpression.cpp
  src/VisitTypeCheckUtils/StellaFunction.h
  src/VisitTypeCheckUtils/StellaFunction.cpp
  src/VisitTypeCheck.h
  src/VisitTypeCheck.cpp
)

add_executable(stella-interpreter ${Stella_Syntax} ${SRCS})
set_property(TARGET stella-interpreter PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(CTest)

add_test(NAME applying-non-function-1 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/applying-non-function-1.stella)
add_test(NAME applying-non-function-2 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/applying-non-function-2.stella)
add_test(NAME applying-non-function-3 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/applying-non-function-3.stella)
add_test(NAME argument-type-mismatch-1 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/argument-type-mismatch-1.stella)
add_test(NAME argument-type-mismatch-2 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/argument-type-mismatch-2.stella)
add_test(NAME argument-type-mismatch-3 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/argument-type-mismatch-3.stella)
add_test(NAME bad-if-1 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-if-1.stella)
add_test(NAME bad-if-2 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-if-2.stella)
add_test(NAME bad-if-3 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-if-3.stella)
add_test(NAME bad-if-4 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-if-4.stella)
add_test(NAME bad-squares-1 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-squares-1.stella)
add_test(NAME bad-squares-2 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-squares-2.stella)
add_test(NAME bad-succ-1 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-succ-1.stella)
add_test(NAME bad-succ-2 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-succ-2.stella)
add_test(NAME bad-succ-3 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/bad-succ-3.stella)
add_test(NAME shadowed-variable-1 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/shadowed-variable-1.stella)
add_test(NAME undefined-variable-1 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/undefined-variable-1.stella)
add_test(NAME undefined-variable-2 COMMAND stella-interpreter typecheck ../tests/core/ill-typed/undefined-variable-2.stella)
add_test(NAME bad-panic-1 COMMAND stella-interpreter typecheck ../tests/exceptions/ill-typed/bad-panic-1.stella)
add_test(NAME bad-variants-1 COMMAND stella-interpreter typecheck ../tests/lists/ill-typed/bad-variants-1.stella)
add_test(NAME bad-pairs-1 COMMAND stella-interpreter typecheck ../tests/pairs/ill-typed/bad-pairs-1.stella)
add_test(NAME bad-records-1 COMMAND stella-interpreter typecheck ../tests/records/ill-typed/bad-records-1.stella)
add_test(NAME bad-refs-1 COMMAND stella-interpreter typecheck ../tests/references/ill-typed/bad-refs-1.stella)
add_test(NAME bad-refs-2 COMMAND stella-interpreter typecheck ../tests/references/ill-typed/bad-refs-2.stella)
add_test(NAME bad-refs-3 COMMAND stella-interpreter typecheck ../tests/references/ill-typed/bad-refs-3.stella)
add_test(NAME bad-subtyping-1 COMMAND stella-interpreter typecheck ../tests/subtyping/ill-typed/bad-subtyping-1.stella)
add_test(NAME bad-subtyping-2 COMMAND stella-interpreter typecheck ../tests/subtyping/ill-typed/bad-subtyping-2.stella)
add_test(NAME bad-subtyping-3 COMMAND stella-interpreter typecheck ../tests/subtyping/ill-typed/bad-subtyping-3.stella)
add_test(NAME bad-subtyping-4 COMMAND stella-interpreter typecheck ../tests/subtyping/ill-typed/bad-subtyping-4.stella)
add_test(NAME bad-subtyping-5 COMMAND stella-interpreter typecheck ../tests/subtyping/ill-typed/bad-subtyping-5.stella)
add_test(NAME bad-sum-types-1 COMMAND stella-interpreter typecheck ../tests/sum-types/ill-typed/bad-sum-types-1.stella)
set_tests_properties(
 applying-non-function-1
 applying-non-function-2
 applying-non-function-3
 argument-type-mismatch-1
 argument-type-mismatch-2
 argument-type-mismatch-3
 bad-if-1
 bad-if-2
 bad-if-3
 bad-if-4
 bad-squares-1
 bad-squares-2
 bad-succ-1
 bad-succ-2
 bad-succ-3
 shadowed-variable-1
 undefined-variable-1
 undefined-variable-2
 bad-panic-1
 bad-variants-1
 bad-pairs-1
 bad-records-1
 bad-refs-1
 bad-refs-2
 bad-refs-3
 bad-subtyping-1
 bad-subtyping-2
 bad-subtyping-3
 bad-subtyping-4
 bad-subtyping-5
 bad-sum-types-1
PROPERTIES WILL_FAIL TRUE)

add_test(NAME factorial COMMAND stella-interpreter typecheck ../tests/core/well-typed/factorial.stella)
add_test(NAME higher-order-1 COMMAND stella-interpreter typecheck ../tests/core/well-typed/higher-order-1.stella)
add_test(NAME increment_twice COMMAND stella-interpreter typecheck ../tests/core/well-typed/increment_twice.stella)
add_test(NAME logical-operators COMMAND stella-interpreter typecheck ../tests/core/well-typed/logical-operators.stella)
add_test(NAME squares COMMAND stella-interpreter typecheck ../tests/core/well-typed/squares.stella)
add_test(NAME panic-3 COMMAND stella-interpreter typecheck ../tests/exceptions/well-typed/panic-3.stella)
add_test(NAME pairs-1 COMMAND stella-interpreter typecheck ../tests/pairs/well-typed/pairs-1.stella)
add_test(NAME records-1 COMMAND stella-interpreter typecheck ../tests/records/well-typed/records-1.stella)
add_test(NAME records-2 COMMAND stella-interpreter typecheck ../tests/records/well-typed/records-2.stella)
add_test(NAME refs-1 COMMAND stella-interpreter typecheck ../tests/references/well-typed/refs-1.stella)
add_test(NAME refs-2 COMMAND stella-interpreter typecheck ../tests/references/well-typed/refs-2.stella)
add_test(NAME refs-3 COMMAND stella-interpreter typecheck ../tests/references/well-typed/refs-3.stella)
add_test(NAME subtyping-1 COMMAND stella-interpreter typecheck ../tests/subtyping/well-typed/subtyping-1.stella)
add_test(NAME subtyping-2 COMMAND stella-interpreter typecheck ../tests/subtyping/well-typed/subtyping-2.stella)
add_test(NAME subtyping-3 COMMAND stella-interpreter typecheck ../tests/subtyping/well-typed/subtyping-3.stella)
add_test(NAME subtyping-4 COMMAND stella-interpreter typecheck ../tests/subtyping/well-typed/subtyping-4.stella)
add_test(NAME subtyping-5 COMMAND stella-interpreter typecheck ../tests/subtyping/well-typed/subtyping-5.stella)
add_test(NAME subtyping-6 COMMAND stella-interpreter typecheck ../tests/subtyping/well-typed/subtyping-6.stella)
add_test(NAME sum-types-1 COMMAND stella-interpreter typecheck ../tests/sum-types/well-typed/sum-types-1.stella)
add_test(NAME sum-types-2 COMMAND stella-interpreter typecheck ../tests/sum-types/well-typed/sum-types-2.stella)
