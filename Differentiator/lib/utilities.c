#include "includes/expressionTree.h"
#include "includes/service.h"

#include <malloc.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

// ----------------------> Declarations <----------------------

const static double EPSILON = 1e-2;

static int CheckEpsilon(double number, double local);

// ----------------------> Definitions <----------------------

Node* CreateNode(TreeNodeType type, 
                 NodeValue value, 
                 Node* left, 
                 Node* right
) {
  Node* new_node = (Node*) calloc(1, sizeof(Node));
  new_node->type = type;
  new_node->content = value;
  new_node->left = left;
  new_node->right = right;

  return new_node;
}

Node* CopySubTree(const Node* node) {
  if (node == NULL) return NULL;

  return CreateNode(node->type,
                    node->content,
                    CopySubTree(node->left),
                    CopySubTree(node->right));
}

void DeleteSubTree(Node* node) {
  if (node == NULL) {
    return;
  }

  DeleteSubTree(node->left);
  DeleteSubTree(node->right);

  free(node);
}

static int CheckEpsilon(double number, double local) {
  if (fabs(local - number) < EPSILON) {
    return 1;
  }
  return 0;
}

// ----------------------> Service <----------------------

TreeNodeMathConst SpecialConstTransform(double number) {
  if (CheckEpsilon(number, M_PI)) {
    return CONST_PI;
  }
  if (CheckEpsilon(number, M_E)) {
    return CONST_E;
  }
  return CONST_NOT_SPECIAL;
}

char* FormatString(const char* format, ...) {
  va_list args;
  va_start(args, format);
  size_t needed = vsnprintf(NULL, 0, format, args);
  char *buffer = (char*) calloc(needed + 1, sizeof(char));
  vsprintf(buffer, format, args);
  va_end(args);
  return buffer;
}

NodeValue VariableWrapper(char* variable) {
  NodeValue value;
  value.variable = variable;
  return value;
}

NodeValue OperationWrapper(TreeNodeOperation operation) {
  NodeValue value;
  value.operation = operation;
  return value;
}

NodeValue ConstWrapper(double number) {
  NodeValue value;
  value.value = number;
  return value;
}

NodeValue FunctionWrapper(TreeNodeFunction function) {
  NodeValue value;
  value.function = function;
  return value;
}
