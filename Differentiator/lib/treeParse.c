#include "includes/expressionTree.h"

#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static NodeValue OperationWrapper(TreeNodeOperation operation);

static NodeValue ConstWrapper(double number);

static NodeValue FunctionWrapper(TreeNodeFunction function);

static void SyntaxError();
static void Require(const char** str, char c);

static Node* GetGrammar(const char* expression);
static Node* GetExpression(const char** str);
static Node* GetTerm(const char** str);
static Node* GetExponent(const char** str);
static Node* GetPriority(const char** str);
static Node* GetNumber(const char** str);

Tree TreeParse(const char* fileName) {
  FILE* fin;
  if ((fin = fopen(fileName, "r")) == NULL) {
    // TODO
  }

  struct stat info;
  if (stat(fileName, &info) != 0) {
    // TODO
  }

  char* buffer = (char*) calloc(info.st_size + 1, sizeof(char));
  if (buffer == NULL) {
    //TODO
  }

  size_t size = fread(buffer, sizeof(char), info.st_size, fin);
  if (ferror(fin)) {
    free(buffer);
    // TODO
  }

  buffer[size] = '$';

  fclose(fin);

  Tree new_tree = {0};
  new_tree.root = GetGrammar(buffer);

  return new_tree;
}

static void SyntaxError() {
  assert(!"syntax error");
}

static void Require(const char** str, char c) {
  if (**str == c) ++(*str);
  else SyntaxError();
}

static Node* GetGrammar(const char* expression) {
  Node* root = GetExpression(&expression);
  Require(&expression, '$');
  return root;
}

static Node* GetExpression(const char** str) {
  Node* cur = GetTerm(str);
  while(**str == '+' || **str == '-') {
    char op = **str;
    ++(*str);
    Node* right = GetTerm(str);

    Node* new_node = NULL;
    if (op == '+') {
      new_node = CreateNode(NODE_OPERATION, OperationWrapper(OP_ADD),
                            cur, right);
    } else {
      new_node = CreateNode(NODE_OPERATION, OperationWrapper(OP_SUB),
                            cur, right);
    }

    cur = new_node;
  }

  return cur;
}

static Node* GetTerm(const char** str) {
  Node* cur = GetExponent(str);
  while(**str == '*' || **str == '/') {
    char op = **str;
    ++(*str);
    Node* right = GetExponent(str);

    Node* new_node = NULL;
    if (op == '*') {
      new_node = CreateNode(NODE_OPERATION, OperationWrapper(OP_MUL),
                            cur, right);
    } else {
      new_node = CreateNode(NODE_OPERATION, OperationWrapper(OP_DIV),
                            cur, right);
    }

    cur = new_node;
  }

  return cur;
}

static Node* GetExponent(const char** str) {
  Node* cur = GetPriority(str);
  if (**str == '^') {
    ++(*str);
    Node* right = GetExponent(str);

    cur = CreateNode(NODE_OPERATION, OperationWrapper(OP_EXP),
                            cur, right);
  }

  return cur;
}

static Node* GetPriority(const char** str) {
  if (**str == '(') {
    ++(*str);
    Node* cur = GetExpression(str);
    Require(str, ')');
    return cur;
  }
  return GetNumber(str);
}

static Node* GetNumber(const char** str) {
  double val = 0;
  while('0' <= **str && **str <= '9') {
    val = val * 10 + (**str - '0');
    ++(*str);
  }

  return CreateNode(NODE_CONST, ConstWrapper(val), NULL, NULL);
}

static NodeValue OperationWrapper(TreeNodeOperation operation) {
  NodeValue value;
  value.operation = operation;
  return value;
}

static NodeValue ConstWrapper(double number) {
  NodeValue value;
  value.value = number;
  return value;
}

static NodeValue FunctionWrapper(TreeNodeFunction function) {
  NodeValue value;
  value.function = function;
  return value;
}