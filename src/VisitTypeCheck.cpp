#include "VisitTypeCheck.h"
#include "VisitTypeCheckUtils/StellaExpression.h"
#include "VisitTypeCheckUtils/StellaFunction.h"
#include "VisitTypeCheckUtils/StellaType.h"
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

// To typecheck, I construct typing trees.
// For example, when Visitor encounters a Function Declaration, it
// starts to construct a new typing tree with the StellaFunction at the root.
// When the tree is built, it is enough to call StellaFunction.isTypingCorrect()
// to check typing correctness of the function (and everything inside)
//
// It is advised to read the code and comments in that order:
// 1) Briefly read the state machine (this file)
// 2) See StellaType class (./VisitTypeCheckUtils/StellaType)
// 3) See StellaFunction class (./VisitTypeCheckUtils/StellaFunction)
// 4) See StellaExpression declaration (./VisitTypeCheckUtils/StellaExpression)
// 5) See some StellaExpression implementations
// (./VisitTypeCheckUtils/StellaExpressions/*)
//
// My implementation also prints informative type errors

// Global context contains function type signatures
std::map<Stella::StellaIdent, StellaType> globalContext;
StellaFunction *currentFunction = NULL;

// stellaFunctions map is used for debugging only
std::map<Stella::StellaIdent, StellaFunction *> stellaFunctions;

void verifyFunction(StellaFunction *function) {
  bool isCorrect = function->isTypingCorrect();

  if (!isCorrect) {
    exit(1);
  }
}

// Functions below are called directly from the template visitor
// These functions control the program state and build typing trees
void onFunction(Stella::StellaIdent ident, int genericsSize) {
  // std::cout << "FUN " << ident << std::endl;
  if (currentFunction != NULL) {
    verifyFunction(currentFunction);
    globalContext.insert({currentFunction->ident, currentFunction->type});
  }

  StellaFunction *function = new StellaFunction(globalContext, genericsSize);
  stellaFunctions.insert({ident, function});
  currentFunction = function;
}

void onFunction(Stella::StellaIdent ident) { onFunction(ident, 0); }

void onIdent(Stella::StellaIdent ident) {
  // std::cout << "IDENT " << ident << std::endl;
  currentFunction->proxyIdent(ident);
  // std::cout << "IDENT END" << std::endl;
}

void onType(StellaDataType type) {
  // std::cout << "TYPE " << type << std::endl;
  currentFunction->proxyExpressionTypeToken(type);
  // std::cout << "TYPE END" << std::endl;
}

void onExactType(StellaType type) {
  // std::cout << "EXACT TYPE " << type.toString() << std::endl;
  currentFunction->proxyType(type);
  // std::cout << "EXACT TYPE END" << std::endl;
}

void onExpression(StellaExpression *expression) {
  // std::cout << "EXPR " << expression->type << std::endl;
  currentFunction->proxyExpression(expression);
  // std::cout << "EXPR END" << std::endl;
}

void onConstInt() { onExpression(new StellaConstIntExpression()); }

void onConstBool() { onExpression(new StellaConstBoolExpression()); }

void onConstUnit() { onExpression(new StellaConstUnitExpression()); }

void onVar(Stella::StellaIdent ident) {
  onExpression(new StellaVarExpression(ident));
}

void onSucc() { onExpression(new StellaSuccExpression()); }

void onNatRec() { onExpression(new StellaNatRecExpression()); }

void onAbstraction() { onExpression(new StellaAbstractionExpression()); }

void onApplication() { onExpression(new StellaApplicationExpression()); }

void onCondition() { onExpression(new StellaConditionExpression()); }

void onTuple() { onExpression(new StellaTupleExpression()); }

void onDotTuple(int index) {
  onExpression(new StellaDotTupleExpression(index));
}

void onInl() { onExpression(new StellaInlInrExpression(1)); }

void onInr() { onExpression(new StellaInlInrExpression(2)); }

void onMatch(int size) { onExpression(new StellaMatchExpression(size)); }

void onMatchCase() { onExpression(new StellaMatchCaseExpression()); }

void onPatternInl() { onExpression(new StellaPatternInlInrExpression(1)); }

void onPatternInr() { onExpression(new StellaPatternInlInrExpression(2)); }

void onPatternVar(Stella::StellaIdent ident) {
  onExpression(new StellaPatternVarExpression(ident));
}

void onSequence() { onExpression(new StellaSequenceExpression()); }

void onRef() { onExpression(new StellaRefExpression()); }

void onDeref() { onExpression(new StellaDerefExpression()); }

void onAssignment() { onExpression(new StellaAssignmentExpression()); }

void onPanic() { onExpression(new StellaPanicExpression()); }

void onRecord(int size) { onExpression(new StellaRecordExpression(size)); }

void onDotRecord(std::string field) {
  onExpression(new StellaDotRecordExpression(field));
}

void onTypeAbstraction(int size) {
  onExpression(new StellaTypeAbstractionExpression(size));
}

void onTypeApplication(int size) {
  onExpression(new StellaTypeApplicationExpression(size));
}

// Functions below are used for debugging only. They print function and
// expression type signatures
void print_indent(int level) {
  for (int i = 0; i < level; i++) {
    std::cout << "\t";
  }
}

void print_expression(StellaExpression *expression, int level) {
  print_indent(level);
  switch (expression->type) {
  case STELLA_EXPRESSION_TYPE_CONST_INT: {
    std::cout << "CONST INT";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    break;
  }
  case STELLA_EXPRESSION_TYPE_CONST_BOOL: {
    std::cout << "CONST BOOL";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    break;
  }
  case STELLA_EXPRESSION_TYPE_ABSTRACTION: {
    StellaAbstractionExpression *expr =
        (StellaAbstractionExpression *)expression;
    std::cout << "ABSTRACTION(" << expr->paramIdent << ": "
              << expr->stellaType.children[0].toString()
              << "): " << expr->expression->getStellaType().toString()
              << std::endl;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_APPLICATION: {
    std::cout << "APPLICATION";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaApplicationExpression *expr =
        (StellaApplicationExpression *)expression;
    print_expression(expr->expression1, level + 1);
    print_expression(expr->expression2, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_CONDITION: {
    std::cout << "CONDITION";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaConditionExpression *expr = (StellaConditionExpression *)expression;
    print_expression(expr->condition, level + 1);
    print_expression(expr->expression1, level + 1);
    print_expression(expr->expression2, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_VAR: {
    StellaVarExpression *expr = (StellaVarExpression *)expression;
    std::cout << "VAR " << expr->ident;
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    break;
  }
  case STELLA_EXPRESSION_TYPE_NAT_REC: {
    std::cout << "NAT::REC";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaNatRecExpression *expr = (StellaNatRecExpression *)expression;
    print_expression(expr->n, level + 1);
    print_expression(expr->z, level + 1);
    print_expression(expr->s, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_SUCC: {
    std::cout << "SUCC";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaSuccExpression *expr = (StellaSuccExpression *)expression;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_CONST_UNIT: {
    std::cout << "UNIT";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    break;
  }
  case STELLA_EXPRESSION_TYPE_DOT_TUPLE: {
    StellaDotTupleExpression *expr = (StellaDotTupleExpression *)expression;
    std::cout << "DOT TUPLE (." << expr->index << ")";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_TUPLE: {
    std::cout << "TUPLE";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaTupleExpression *expr = (StellaTupleExpression *)expression;
    print_expression(expr->expression1, level + 1);
    print_expression(expr->expression2, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_INL_INR: {
    StellaInlInrExpression *expr = (StellaInlInrExpression *)expression;
    if (expr->side == 1) {
      std::cout << "INL";
    } else {
      std::cout << "INR";
    }
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_MATCH: {
    std::cout << "MATCH";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaMatchExpression *expr = (StellaMatchExpression *)expression;
    print_expression(expr->matchExpression, level + 1);
    for (int i = 0; i < expr->expressions.size(); i++) {
      print_expression(expr->expressions[i], level + 1);
    }
    break;
  }
  case STELLA_EXPRESSION_TYPE_MATCH_CASE: {
    std::cout << "MATCH CASE";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaMatchCaseExpression *expr = (StellaMatchCaseExpression *)expression;
    print_expression(expr->match, level + 1);
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_PATTERN_INL_INR: {
    StellaPatternInlInrExpression *expr =
        (StellaPatternInlInrExpression *)expression;
    if (expr->side == 1) {
      std::cout << "PATTERN INL";
    } else {
      std::cout << "PATTERN INR";
    }
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_PATTERN_VAR: {
    StellaPatternVarExpression *expr = (StellaPatternVarExpression *)expression;
    std::cout << "PATTERN VAR " << expr->ident;
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    break;
  }
  case STELLA_EXPRESSION_TYPE_SEQUENCE: {
    std::cout << "SEQUENCE";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaSequenceExpression *expr = (StellaSequenceExpression *)expression;
    print_expression(expr->expression1, level + 1);
    print_expression(expr->expression2, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_REF: {
    std::cout << "REF";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaRefExpression *expr = (StellaRefExpression *)expression;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_DEREF: {
    std::cout << "DEREF";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaDerefExpression *expr = (StellaDerefExpression *)expression;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_ASSIGNMENT: {
    std::cout << "ASSIGNMENT";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    StellaAssignmentExpression *expr = (StellaAssignmentExpression *)expression;
    print_expression(expr->expression1, level + 1);
    print_expression(expr->expression2, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_PANIC: {
    std::cout << "PANIC";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    break;
  }
  case STELLA_EXPRESSION_TYPE_RECORD: {
    std::cout << "RECORD";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    break;
  }
  case STELLA_EXPRESSION_TYPE_DOT_RECORD: {
    StellaDotRecordExpression *expr = (StellaDotRecordExpression *)expression;
    std::cout << "DOT RECORD (." << expr->field << ")";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_TYPE_ABSTRACTION: {
    StellaTypeAbstractionExpression *expr =
        (StellaTypeAbstractionExpression *)expression;
    std::cout << "TYPE ABSTRACTION [";
    for (int i = 0; i < expr->typeVars.size(); i++) {
      std::cout << expr->typeVars[i] << ", ";
    }
    std::cout << "]";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    print_expression(expr->expression, level + 1);
    break;
  }
  case STELLA_EXPRESSION_TYPE_TYPE_APPLICATION: {
    StellaTypeApplicationExpression *expr =
        (StellaTypeApplicationExpression *)expression;
    std::cout << "TYPE APPLICATION [";
    for (int i = 0; i < expr->types.children.size(); i++) {
      std::cout << expr->types.children[i].toString() << ", ";
    }
    std::cout << "]";
    std::cout << " : (" << expression->getStellaType().toString() << ")"
              << std::endl;
    print_expression(expr->expression, level + 1);
    break;
  }
  }
}

void print_function(StellaFunction *function) {
  std::cout << function->ident << ": " << function->type.toString()
            << std::endl;

  print_expression(function->expression, 1);
}

// This is called when visitor finishes reading a program
void onEnd() {
  if (currentFunction != NULL) {
    verifyFunction(currentFunction);
  }

  // Try this if you want to see what typing tree looks like:
  // print_function(stellaFunctions["main"]);
}

namespace Stella {
void VisitTypeCheck::visitProgram(Program *t) {}               // abstract class
void VisitTypeCheck::visitLanguageDecl(LanguageDecl *t) {}     // abstract class
void VisitTypeCheck::visitExtension(Extension *t) {}           // abstract class
void VisitTypeCheck::visitDecl(Decl *t) {}                     // abstract class
void VisitTypeCheck::visitLocalDecl(LocalDecl *t) {}           // abstract class
void VisitTypeCheck::visitAnnotation(Annotation *t) {}         // abstract class
void VisitTypeCheck::visitParamDecl(ParamDecl *t) {}           // abstract class
void VisitTypeCheck::visitReturnType(ReturnType *t) {}         // abstract class
void VisitTypeCheck::visitThrowType(ThrowType *t) {}           // abstract class
void VisitTypeCheck::visitType(Type *t) {}                     // abstract class
void VisitTypeCheck::visitMatchCase(MatchCase *t) {}           // abstract class
void VisitTypeCheck::visitOptionalTyping(OptionalTyping *t) {} // abstract class
void VisitTypeCheck::visitPatternData(PatternData *t) {}       // abstract class
void VisitTypeCheck::visitExprData(ExprData *t) {}             // abstract class
void VisitTypeCheck::visitPattern(Pattern *t) {}               // abstract class
void VisitTypeCheck::visitLabelledPattern(LabelledPattern *t) {
} // abstract class
void VisitTypeCheck::visitBinding(Binding *t) {}               // abstract class
void VisitTypeCheck::visitExpr(Expr *t) {}                     // abstract class
void VisitTypeCheck::visitPatternBinding(PatternBinding *t) {} // abstract class
void VisitTypeCheck::visitVariantFieldType(VariantFieldType *t) {
} // abstract class
void VisitTypeCheck::visitRecordFieldType(RecordFieldType *t) {
} // abstract class
void VisitTypeCheck::visitTyping(Typing *t) {} // abstract class

void VisitTypeCheck::visitLanguageCore(LanguageCore *language_core) {
  /* Code For LanguageCore Goes Here */
}

void VisitTypeCheck::visitAnExtension(AnExtension *an_extension) {
  /* Code For AnExtension Goes Here */

  if (an_extension->listextensionname_)
    an_extension->listextensionname_->accept(this);
}

void VisitTypeCheck::visitDeclFun(DeclFun *decl_fun) {
  onFunction(decl_fun->stellaident_);

  if (decl_fun->listannotation_)
    decl_fun->listannotation_->accept(this);
  visitStellaIdent(decl_fun->stellaident_);
  if (decl_fun->listparamdecl_)
    decl_fun->listparamdecl_->accept(this);
  if (decl_fun->returntype_)
    decl_fun->returntype_->accept(this);
  if (decl_fun->throwtype_)
    decl_fun->throwtype_->accept(this);
  if (decl_fun->listdecl_)
    decl_fun->listdecl_->accept(this);
  if (decl_fun->expr_)
    decl_fun->expr_->accept(this);
}

void VisitTypeCheck::visitDeclTypeAlias(DeclTypeAlias *decl_type_alias) {
  /* Code For DeclTypeAlias Goes Here */

  visitStellaIdent(decl_type_alias->stellaident_);
  if (decl_type_alias->type_)
    decl_type_alias->type_->accept(this);
}

void VisitTypeCheck::visitDeclExceptionType(
    DeclExceptionType *decl_exception_type) {
  /* Code For DeclExceptionType Goes Here */

  if (decl_exception_type->type_)
    decl_exception_type->type_->accept(this);
}

void VisitTypeCheck::visitDeclExceptionVariant(
    DeclExceptionVariant *decl_exception_variant) {
  /* Code For DeclExceptionVariant Goes Here */

  visitStellaIdent(decl_exception_variant->stellaident_);
  if (decl_exception_variant->type_)
    decl_exception_variant->type_->accept(this);
}

void VisitTypeCheck::visitAssign(Assign *assign) {
  onAssignment();

  if (assign->expr_1)
    assign->expr_1->accept(this);
  if (assign->expr_2)
    assign->expr_2->accept(this);
}

void VisitTypeCheck::visitRef(Ref *ref) {
  onRef();

  if (ref->expr_)
    ref->expr_->accept(this);
}

void VisitTypeCheck::visitDeref(Deref *deref) {
  onDeref();

  if (deref->expr_)
    deref->expr_->accept(this);
}

void VisitTypeCheck::visitPanic(Panic *panic) { onPanic(); }

void VisitTypeCheck::visitThrow(Throw *throw_) {
  /* Code For Throw Goes Here */

  if (throw_->expr_)
    throw_->expr_->accept(this);
}

void VisitTypeCheck::visitTryCatch(TryCatch *try_catch) {
  /* Code For TryCatch Goes Here */

  if (try_catch->expr_1)
    try_catch->expr_1->accept(this);
  if (try_catch->pattern_)
    try_catch->pattern_->accept(this);
  if (try_catch->expr_2)
    try_catch->expr_2->accept(this);
}

void VisitTypeCheck::visitTryWith(TryWith *try_with) {
  /* Code For TryWith Goes Here */

  if (try_with->expr_1)
    try_with->expr_1->accept(this);
  if (try_with->expr_2)
    try_with->expr_2->accept(this);
}

void VisitTypeCheck::visitALocalDecl(ALocalDecl *a_local_decl) {
  /* Code For ALocalDecl Goes Here */

  if (a_local_decl->decl_)
    a_local_decl->decl_->accept(this);
}

void VisitTypeCheck::visitInlineAnnotation(
    InlineAnnotation *inline_annotation) {
  /* Code For InlineAnnotation Goes Here */
}

void VisitTypeCheck::visitAParamDecl(AParamDecl *a_param_decl) {
  /* Code For AParamDecl Goes Here */

  visitStellaIdent(a_param_decl->stellaident_);
  if (a_param_decl->type_)
    a_param_decl->type_->accept(this);
}

void VisitTypeCheck::visitNoReturnType(NoReturnType *no_return_type) {
  /* Code For NoReturnType Goes Here */
}

void VisitTypeCheck::visitSomeReturnType(SomeReturnType *some_return_type) {
  /* Code For SomeReturnType Goes Here */

  if (some_return_type->type_)
    some_return_type->type_->accept(this);
}

void VisitTypeCheck::visitNoThrowType(NoThrowType *no_throw_type) {
  /* Code For NoThrowType Goes Here */
}

void VisitTypeCheck::visitSomeThrowType(SomeThrowType *some_throw_type) {
  /* Code For SomeThrowType Goes Here */

  if (some_throw_type->listtype_)
    some_throw_type->listtype_->accept(this);
}

void VisitTypeCheck::visitTypeFun(TypeFun *type_fun) {
  onType(STELLA_DATA_TYPE_FUN);

  if (type_fun->listtype_)
    type_fun->listtype_->accept(this);
  if (type_fun->type_)
    type_fun->type_->accept(this);
}

void VisitTypeCheck::visitTypeRec(TypeRec *type_rec) {
  /* Code For TypeRec Goes Here */

  visitStellaIdent(type_rec->stellaident_);
  if (type_rec->type_)
    type_rec->type_->accept(this);
}

void VisitTypeCheck::visitTypeSum(TypeSum *type_sum) {
  onType(STELLA_DATA_TYPE_SUM);

  if (type_sum->type_1)
    type_sum->type_1->accept(this);
  if (type_sum->type_2)
    type_sum->type_2->accept(this);
}

void VisitTypeCheck::visitTypeTuple(TypeTuple *type_tuple) {
  onType(STELLA_DATA_TYPE_TUPLE);

  if (type_tuple->listtype_)
    type_tuple->listtype_->accept(this);
}

void VisitTypeCheck::visitTypeRecord(TypeRecord *type_record) {
  onExactType(StellaType(STELLA_DATA_TYPE_RECORD,
                         type_record->listrecordfieldtype_->size()));

  if (type_record->listrecordfieldtype_)
    type_record->listrecordfieldtype_->accept(this);
}

void VisitTypeCheck::visitTypeVariant(TypeVariant *type_variant) {
  /* Code For TypeVariant Goes Here */

  if (type_variant->listvariantfieldtype_)
    type_variant->listvariantfieldtype_->accept(this);
}

void VisitTypeCheck::visitTypeList(TypeList *type_list) {
  /* Code For TypeList Goes Here */

  if (type_list->type_)
    type_list->type_->accept(this);
}

void VisitTypeCheck::visitTypeBool(TypeBool *type_bool) {
  onType(STELLA_DATA_TYPE_BOOL);
}

void VisitTypeCheck::visitTypeNat(TypeNat *type_nat) {
  onType(STELLA_DATA_TYPE_NAT);
}

void VisitTypeCheck::visitTypeUnit(TypeUnit *type_unit) {
  onType(STELLA_DATA_TYPE_UNIT);
}

void VisitTypeCheck::visitTypeTop(TypeTop *type_top) {
  /* Code For TypeTop Goes Here */
}

void VisitTypeCheck::visitTypeBottom(TypeBottom *type_bottom) {
  /* Code For TypeBottom Goes Here */
}

void VisitTypeCheck::visitTypeRef(TypeRef *type_ref) {
  onType(STELLA_DATA_TYPE_REF);

  if (type_ref->type_)
    type_ref->type_->accept(this);
}

void VisitTypeCheck::visitTypeVar(TypeVar *type_var) {
  onExactType(StellaType(type_var->stellaident_));
}

void VisitTypeCheck::visitAMatchCase(AMatchCase *a_match_case) {
  onMatchCase();

  if (a_match_case->pattern_)
    a_match_case->pattern_->accept(this);
  if (a_match_case->expr_)
    a_match_case->expr_->accept(this);
}

void VisitTypeCheck::visitNoTyping(NoTyping *no_typing) {
  /* Code For NoTyping Goes Here */
}

void VisitTypeCheck::visitSomeTyping(SomeTyping *some_typing) {
  /* Code For SomeTyping Goes Here */

  if (some_typing->type_)
    some_typing->type_->accept(this);
}

void VisitTypeCheck::visitNoPatternData(NoPatternData *no_pattern_data) {
  /* Code For NoPatternData Goes Here */
}

void VisitTypeCheck::visitSomePatternData(SomePatternData *some_pattern_data) {
  /* Code For SomePatternData Goes Here */

  if (some_pattern_data->pattern_)
    some_pattern_data->pattern_->accept(this);
}

void VisitTypeCheck::visitNoExprData(NoExprData *no_expr_data) {
  /* Code For NoExprData Goes Here */
}

void VisitTypeCheck::visitSomeExprData(SomeExprData *some_expr_data) {
  /* Code For SomeExprData Goes Here */

  if (some_expr_data->expr_)
    some_expr_data->expr_->accept(this);
}

void VisitTypeCheck::visitPatternVariant(PatternVariant *pattern_variant) {
  visitStellaIdent(pattern_variant->stellaident_);
  if (pattern_variant->patterndata_)
    pattern_variant->patterndata_->accept(this);
}

void VisitTypeCheck::visitPatternInl(PatternInl *pattern_inl) {
  onPatternInl();

  if (pattern_inl->pattern_)
    pattern_inl->pattern_->accept(this);
}

void VisitTypeCheck::visitPatternInr(PatternInr *pattern_inr) {
  onPatternInr();

  if (pattern_inr->pattern_)
    pattern_inr->pattern_->accept(this);
}

void VisitTypeCheck::visitPatternTuple(PatternTuple *pattern_tuple) {
  /* Code For PatternTuple Goes Here */

  if (pattern_tuple->listpattern_)
    pattern_tuple->listpattern_->accept(this);
}

void VisitTypeCheck::visitPatternRecord(PatternRecord *pattern_record) {
  /* Code For PatternRecord Goes Here */

  if (pattern_record->listlabelledpattern_)
    pattern_record->listlabelledpattern_->accept(this);
}

void VisitTypeCheck::visitPatternList(PatternList *pattern_list) {
  /* Code For PatternList Goes Here */

  if (pattern_list->listpattern_)
    pattern_list->listpattern_->accept(this);
}

void VisitTypeCheck::visitPatternCons(PatternCons *pattern_cons) {
  /* Code For PatternCons Goes Here */

  if (pattern_cons->pattern_1)
    pattern_cons->pattern_1->accept(this);
  if (pattern_cons->pattern_2)
    pattern_cons->pattern_2->accept(this);
}

void VisitTypeCheck::visitPatternFalse(PatternFalse *pattern_false) {
  /* Code For PatternFalse Goes Here */
}

void VisitTypeCheck::visitPatternTrue(PatternTrue *pattern_true) {
  /* Code For PatternTrue Goes Here */
}

void VisitTypeCheck::visitPatternUnit(PatternUnit *pattern_unit) {
  /* Code For PatternUnit Goes Here */
}

void VisitTypeCheck::visitPatternInt(PatternInt *pattern_int) {
  /* Code For PatternInt Goes Here */

  visitInteger(pattern_int->integer_);
}

void VisitTypeCheck::visitPatternSucc(PatternSucc *pattern_succ) {
  /* Code For PatternSucc Goes Here */

  if (pattern_succ->pattern_)
    pattern_succ->pattern_->accept(this);
}

void VisitTypeCheck::visitGreaterThan(GreaterThan *greater_than) {
  /* Code For GreaterThan Goes Here */

  if (greater_than->expr_1)
    greater_than->expr_1->accept(this);
  if (greater_than->expr_2)
    greater_than->expr_2->accept(this);
}

void VisitTypeCheck::visitGreaterThanOrEqual(
    GreaterThanOrEqual *greater_than_or_equal) {
  /* Code For GreaterThanOrEqual Goes Here */

  if (greater_than_or_equal->expr_1)
    greater_than_or_equal->expr_1->accept(this);
  if (greater_than_or_equal->expr_2)
    greater_than_or_equal->expr_2->accept(this);
}

void VisitTypeCheck::visitEqual(Equal *equal) {
  /* Code For Equal Goes Here */

  if (equal->expr_1)
    equal->expr_1->accept(this);
  if (equal->expr_2)
    equal->expr_2->accept(this);
}

void VisitTypeCheck::visitNotEqual(NotEqual *not_equal) {
  /* Code For NotEqual Goes Here */

  if (not_equal->expr_1)
    not_equal->expr_1->accept(this);
  if (not_equal->expr_2)
    not_equal->expr_2->accept(this);
}

void VisitTypeCheck::visitTypeAsc(TypeAsc *type_asc) {
  /* Code For TypeAsc Goes Here */

  if (type_asc->expr_)
    type_asc->expr_->accept(this);
  if (type_asc->type_)
    type_asc->type_->accept(this);
}

void VisitTypeCheck::visitTypeCast(TypeCast *type_cast) {
  /* Code For TypeCast Goes Here */

  if (type_cast->expr_)
    type_cast->expr_->accept(this);
  if (type_cast->type_)
    type_cast->type_->accept(this);
}

void VisitTypeCheck::visitVariant(Variant *variant) {
  /* Code For Variant Goes Here */

  visitStellaIdent(variant->stellaident_);
  if (variant->exprdata_)
    variant->exprdata_->accept(this);
}

void VisitTypeCheck::visitList(List *list) {
  /* Code For List Goes Here */

  if (list->listexpr_)
    list->listexpr_->accept(this);
}

void VisitTypeCheck::visitAdd(Add *add) {
  /* Code For Add Goes Here */

  if (add->expr_1)
    add->expr_1->accept(this);
  if (add->expr_2)
    add->expr_2->accept(this);
}

void VisitTypeCheck::visitSubtract(Subtract *subtract) {
  /* Code For Subtract Goes Here */

  if (subtract->expr_1)
    subtract->expr_1->accept(this);
  if (subtract->expr_2)
    subtract->expr_2->accept(this);
}

void VisitTypeCheck::visitLogicOr(LogicOr *logic_or) {
  /* Code For LogicOr Goes Here */

  if (logic_or->expr_1)
    logic_or->expr_1->accept(this);
  if (logic_or->expr_2)
    logic_or->expr_2->accept(this);
}

void VisitTypeCheck::visitMultiply(Multiply *multiply) {
  /* Code For Multiply Goes Here */

  if (multiply->expr_1)
    multiply->expr_1->accept(this);
  if (multiply->expr_2)
    multiply->expr_2->accept(this);
}

void VisitTypeCheck::visitDivide(Divide *divide) {
  /* Code For Divide Goes Here */

  if (divide->expr_1)
    divide->expr_1->accept(this);
  if (divide->expr_2)
    divide->expr_2->accept(this);
}

void VisitTypeCheck::visitLogicAnd(LogicAnd *logic_and) {
  /* Code For LogicAnd Goes Here */

  if (logic_and->expr_1)
    logic_and->expr_1->accept(this);
  if (logic_and->expr_2)
    logic_and->expr_2->accept(this);
}

void VisitTypeCheck::visitDotRecord(DotRecord *dot_record) {
  onDotRecord(dot_record->stellaident_);

  if (dot_record->expr_)
    dot_record->expr_->accept(this);
  // visitStellaIdent(dot_record->stellaident_);
}

void VisitTypeCheck::visitRecord(Record *record) {
  /* Code For Record Goes Here */
  onRecord(record->listbinding_->size());

  if (record->listbinding_)
    record->listbinding_->accept(this);
}

void VisitTypeCheck::visitConsList(ConsList *cons_list) {
  /* Code For ConsList Goes Here */

  if (cons_list->expr_1)
    cons_list->expr_1->accept(this);
  if (cons_list->expr_2)
    cons_list->expr_2->accept(this);
}

void VisitTypeCheck::visitHead(Head *head) {
  /* Code For Head Goes Here */

  if (head->expr_)
    head->expr_->accept(this);
}

void VisitTypeCheck::visitIsEmpty(IsEmpty *is_empty) {
  /* Code For IsEmpty Goes Here */

  if (is_empty->expr_)
    is_empty->expr_->accept(this);
}

void VisitTypeCheck::visitTail(Tail *tail) {
  /* Code For Tail Goes Here */

  if (tail->expr_)
    tail->expr_->accept(this);
}

void VisitTypeCheck::visitLogicNot(LogicNot *logic_not) {
  /* Code For LogicNot Goes Here */

  if (logic_not->expr_)
    logic_not->expr_->accept(this);
}

void VisitTypeCheck::visitPred(Pred *pred) {
  /* Code For Pred Goes Here */

  if (pred->expr_)
    pred->expr_->accept(this);
}

void VisitTypeCheck::visitIsZero(IsZero *is_zero) {
  /* Code For IsZero Goes Here */

  if (is_zero->expr_)
    is_zero->expr_->accept(this);
}

void VisitTypeCheck::visitFix(Fix *fix) {
  /* Code For Fix Goes Here */

  if (fix->expr_)
    fix->expr_->accept(this);
}

void VisitTypeCheck::visitFold(Fold *fold) {
  /* Code For Fold Goes Here */

  if (fold->type_)
    fold->type_->accept(this);
  if (fold->expr_)
    fold->expr_->accept(this);
}

void VisitTypeCheck::visitUnfold(Unfold *unfold) {
  /* Code For Unfold Goes Here */

  if (unfold->type_)
    unfold->type_->accept(this);
  if (unfold->expr_)
    unfold->expr_->accept(this);
}

void VisitTypeCheck::visitConstMemory(ConstMemory *const_memory) {
  /* Code For ConstMemory Goes Here */

  visitMemoryAddress(const_memory->memoryaddress_);
}

void VisitTypeCheck::visitAPatternBinding(APatternBinding *a_pattern_binding) {
  /* Code For APatternBinding Goes Here */

  if (a_pattern_binding->pattern_)
    a_pattern_binding->pattern_->accept(this);
  if (a_pattern_binding->expr_)
    a_pattern_binding->expr_->accept(this);
}

void VisitTypeCheck::visitAVariantFieldType(
    AVariantFieldType *a_variant_field_type) {
  /* Code For AVariantFieldType Goes Here */

  visitStellaIdent(a_variant_field_type->stellaident_);
  if (a_variant_field_type->optionaltyping_)
    a_variant_field_type->optionaltyping_->accept(this);
}

void VisitTypeCheck::visitARecordFieldType(
    ARecordFieldType *a_record_field_type) {
  /* Code For ARecordFieldType Goes Here */

  visitStellaIdent(a_record_field_type->stellaident_);
  if (a_record_field_type->type_)
    a_record_field_type->type_->accept(this);
}

void VisitTypeCheck::visitATyping(ATyping *a_typing) {
  /* Code For ATyping Goes Here */

  if (a_typing->expr_)
    a_typing->expr_->accept(this);
  if (a_typing->type_)
    a_typing->type_->accept(this);
}

void VisitTypeCheck::visitListStellaIdent(ListStellaIdent *list_stella_ident) {
  for (ListStellaIdent::iterator i = list_stella_ident->begin();
       i != list_stella_ident->end(); ++i) {
    visitStellaIdent(*i);
  }
}

void VisitTypeCheck::visitListExtensionName(
    ListExtensionName *list_extension_name) {
  for (ListExtensionName::iterator i = list_extension_name->begin();
       i != list_extension_name->end(); ++i) {
    visitExtensionName(*i);
  }
}

void VisitTypeCheck::visitListExtension(ListExtension *list_extension) {
  for (ListExtension::iterator i = list_extension->begin();
       i != list_extension->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListDecl(ListDecl *list_decl) {
  for (ListDecl::iterator i = list_decl->begin(); i != list_decl->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListLocalDecl(ListLocalDecl *list_local_decl) {
  for (ListLocalDecl::iterator i = list_local_decl->begin();
       i != list_local_decl->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListAnnotation(ListAnnotation *list_annotation) {
  for (ListAnnotation::iterator i = list_annotation->begin();
       i != list_annotation->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListParamDecl(ListParamDecl *list_param_decl) {
  for (ListParamDecl::iterator i = list_param_decl->begin();
       i != list_param_decl->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListType(ListType *list_type) {
  for (ListType::iterator i = list_type->begin(); i != list_type->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListMatchCase(ListMatchCase *list_match_case) {
  for (ListMatchCase::iterator i = list_match_case->begin();
       i != list_match_case->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListPattern(ListPattern *list_pattern) {
  for (ListPattern::iterator i = list_pattern->begin();
       i != list_pattern->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListLabelledPattern(
    ListLabelledPattern *list_labelled_pattern) {
  for (ListLabelledPattern::iterator i = list_labelled_pattern->begin();
       i != list_labelled_pattern->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListBinding(ListBinding *list_binding) {
  for (ListBinding::iterator i = list_binding->begin();
       i != list_binding->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListExpr(ListExpr *list_expr) {
  for (ListExpr::iterator i = list_expr->begin(); i != list_expr->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListPatternBinding(
    ListPatternBinding *list_pattern_binding) {
  for (ListPatternBinding::iterator i = list_pattern_binding->begin();
       i != list_pattern_binding->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListVariantFieldType(
    ListVariantFieldType *list_variant_field_type) {
  for (ListVariantFieldType::iterator i = list_variant_field_type->begin();
       i != list_variant_field_type->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitListRecordFieldType(
    ListRecordFieldType *list_record_field_type) {
  for (ListRecordFieldType::iterator i = list_record_field_type->begin();
       i != list_record_field_type->end(); ++i) {
    (*i)->accept(this);
  }
}

void VisitTypeCheck::visitInteger(Integer x) {
  /* Code for Integer Goes Here */
}

void VisitTypeCheck::visitChar(Char x) { /* Code for Char Goes Here */
}

void VisitTypeCheck::visitDouble(Double x) { /* Code for Double Goes Here */
}

void VisitTypeCheck::visitString(String x) { /* Code for String Goes Here */
}

void VisitTypeCheck::visitIdent(Ident x) { /* Code for Ident Goes Here */
}

void VisitTypeCheck::visitExtensionName(ExtensionName x) {
  /* Code for ExtensionName Goes Here */
}

void VisitTypeCheck::visitMemoryAddress(MemoryAddress x) {
  /* Code for MemoryAddress Goes Here */
}

void VisitTypeCheck::visitAProgram(AProgram *a_program) {
  /* Code For AProgram Goes Here */

  if (a_program->languagedecl_)
    a_program->languagedecl_->accept(this);
  if (a_program->listextension_)
    a_program->listextension_->accept(this);
  if (a_program->listdecl_)
    a_program->listdecl_->accept(this);

  onEnd();
}

void VisitTypeCheck::visitPatternVar(PatternVar *pattern_var) {
  onPatternVar(pattern_var->stellaident_);

  // visitStellaIdent(pattern_var->stellaident_);
}

void VisitTypeCheck::visitALabelledPattern(
    ALabelledPattern *a_labelled_pattern) {
  /* Code For ALabelledPattern Goes Here */

  visitStellaIdent(a_labelled_pattern->stellaident_);
  if (a_labelled_pattern->pattern_)
    a_labelled_pattern->pattern_->accept(this);
}

void VisitTypeCheck::visitABinding(ABinding *a_binding) {
  visitStellaIdent(a_binding->stellaident_);
  if (a_binding->expr_)
    a_binding->expr_->accept(this);
}

void VisitTypeCheck::visitSequence(Sequence *sequence) {
  onSequence();

  if (sequence->expr_1)
    sequence->expr_1->accept(this);
  if (sequence->expr_2)
    sequence->expr_2->accept(this);
}

void VisitTypeCheck::visitIf(If *if_) {
  onCondition();

  if (if_->expr_1)
    if_->expr_1->accept(this);
  if (if_->expr_2)
    if_->expr_2->accept(this);
  if (if_->expr_3)
    if_->expr_3->accept(this);
}

void VisitTypeCheck::visitLet(Let *let) {
  /* Code For Let Goes Here */

  if (let->listpatternbinding_)
    let->listpatternbinding_->accept(this);
  if (let->expr_)
    let->expr_->accept(this);
}

void VisitTypeCheck::visitLetRec(LetRec *let_rec) {
  /* Code For LetRec Goes Here */

  if (let_rec->listpatternbinding_)
    let_rec->listpatternbinding_->accept(this);
  if (let_rec->expr_)
    let_rec->expr_->accept(this);
}

void VisitTypeCheck::visitLessThan(LessThan *less_than) {
  /* Code For LessThan Goes Here */

  if (less_than->expr_1)
    less_than->expr_1->accept(this);
  if (less_than->expr_2)
    less_than->expr_2->accept(this);
}

void VisitTypeCheck::visitLessThanOrEqual(LessThanOrEqual *less_than_or_equal) {
  /* Code For LessThanOrEqual Goes Here */

  if (less_than_or_equal->expr_1)
    less_than_or_equal->expr_1->accept(this);
  if (less_than_or_equal->expr_2)
    less_than_or_equal->expr_2->accept(this);
}

void VisitTypeCheck::visitAbstraction(Abstraction *abstraction) {
  onAbstraction();

  if (abstraction->listparamdecl_)
    abstraction->listparamdecl_->accept(this);
  if (abstraction->expr_)
    abstraction->expr_->accept(this);
}

void VisitTypeCheck::visitMatch(Match *match) {
  onMatch(match->listmatchcase_->size());

  if (match->expr_)
    match->expr_->accept(this);
  if (match->listmatchcase_)
    match->listmatchcase_->accept(this);
}

void VisitTypeCheck::visitApplication(Application *application) {
  onApplication();

  if (application->expr_)
    application->expr_->accept(this);
  if (application->listexpr_)
    application->listexpr_->accept(this);
}

void VisitTypeCheck::visitDotTuple(DotTuple *dot_tuple) {
  onDotTuple(dot_tuple->integer_);

  if (dot_tuple->expr_)
    dot_tuple->expr_->accept(this);
  visitInteger(dot_tuple->integer_);
}

void VisitTypeCheck::visitTuple(Tuple *tuple) {
  onTuple();

  if (tuple->listexpr_)
    tuple->listexpr_->accept(this);
}

void VisitTypeCheck::visitInl(Inl *inl) {
  onInl();

  if (inl->expr_)
    inl->expr_->accept(this);
}

void VisitTypeCheck::visitInr(Inr *inr) {
  onInr();

  if (inr->expr_)
    inr->expr_->accept(this);
}

void VisitTypeCheck::visitSucc(Succ *succ) {
  onSucc();

  if (succ->expr_)
    succ->expr_->accept(this);
}

void VisitTypeCheck::visitNatRec(NatRec *nat_rec) {
  onNatRec();

  if (nat_rec->expr_1)
    nat_rec->expr_1->accept(this);
  if (nat_rec->expr_2)
    nat_rec->expr_2->accept(this);
  if (nat_rec->expr_3)
    nat_rec->expr_3->accept(this);
}

void VisitTypeCheck::visitConstTrue(ConstTrue *const_true) { onConstBool(); }

void VisitTypeCheck::visitConstFalse(ConstFalse *const_false) { onConstBool(); }

void VisitTypeCheck::visitConstUnit(ConstUnit *const_unit) { onConstUnit(); }

void VisitTypeCheck::visitConstInt(ConstInt *const_int) {
  onConstInt();

  visitInteger(const_int->integer_);
}

void VisitTypeCheck::visitVar(Var *var) {
  onVar(var->stellaident_);

  // visitStellaIdent(var->stellaident_);
}

void VisitTypeCheck::visitStellaIdent(StellaIdent x) {
  onIdent(x);
  /* Code for StellaIdent Goes Here */
}

void VisitTypeCheck::visitDeclFunGeneric(DeclFunGeneric *decl_fun_generic) {
  onFunction(decl_fun_generic->stellaident_,
             decl_fun_generic->liststellaident_->size());

  if (decl_fun_generic->listannotation_)
    decl_fun_generic->listannotation_->accept(this);
  visitStellaIdent(decl_fun_generic->stellaident_);
  if (decl_fun_generic->liststellaident_)
    decl_fun_generic->liststellaident_->accept(this);
  if (decl_fun_generic->listparamdecl_)
    decl_fun_generic->listparamdecl_->accept(this);
  if (decl_fun_generic->returntype_)
    decl_fun_generic->returntype_->accept(this);
  if (decl_fun_generic->throwtype_)
    decl_fun_generic->throwtype_->accept(this);
  if (decl_fun_generic->listdecl_)
    decl_fun_generic->listdecl_->accept(this);
  if (decl_fun_generic->expr_)
    decl_fun_generic->expr_->accept(this);
}

void VisitTypeCheck::visitTypeAbstraction(TypeAbstraction *type_abstraction) {
  /* Code For TypeAbstraction Goes Here */

  onTypeAbstraction(type_abstraction->liststellaident_->size());

  if (type_abstraction->liststellaident_)
    type_abstraction->liststellaident_->accept(this);
  if (type_abstraction->expr_)
    type_abstraction->expr_->accept(this);
}

void VisitTypeCheck::visitTypeApplication(TypeApplication *type_application) {
  /* Code For TypeApplication Goes Here */

  onTypeApplication(type_application->listtype_->size());

  if (type_application->listtype_)
    type_application->listtype_->accept(this);
  if (type_application->expr_)
    type_application->expr_->accept(this);
}

void VisitTypeCheck::visitTypeForAll(TypeForAll *type_for_all) {
  /* Code For TypeForAll Goes Here */

  onExactType(StellaType(STELLA_DATA_TYPE_FORALL,
                         type_for_all->liststellaident_->size()));
  if (type_for_all->liststellaident_)
    type_for_all->liststellaident_->accept(this);
  if (type_for_all->type_)
    type_for_all->type_->accept(this);
}

} // namespace Stella
