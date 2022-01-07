#ifndef LIB_RELAYX_SERVICE_HEADER
#define LIB_RELAYX_SERVICE_HEADER

#include "expressionTree.h"

TreeNodeMathConst SpecialConstTransform(double number);

char* FormatString(const char* format, ...);

NodeValue VariableWrapper(char* variable);

NodeValue OperationWrapper(TreeNodeOperation operation);

NodeValue ConstWrapper(double number);

NodeValue FunctionWrapper(TreeNodeFunction function);

#endif
