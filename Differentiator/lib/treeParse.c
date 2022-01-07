#include "includes/expressionTree.h"
#include "includes/service.h"

#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

// ----------------------> Declarations <----------------------

typedef struct _WordToken {
  const char* token;
  size_t      id;
} WordToken;

static const WordToken FUNCTIONS[] = {
  {"cos",    FUNC_COS},
  {"sin",    FUNC_SIN},
  {"tg",     FUNC_TG},
  {"ctg",    FUNC_CTG},
  {"arctg",  FUNC_ARCTG},
  {"arcctg", FUNC_ARCCTG},
  {"arcsin", FUNC_ARCSIN},
  {"arccos", FUNC_ARCCOS},
  {"sh",     FUNC_SH},
  {"ch",     FUNC_CH},
  {"th",     FUNC_TH},
  {"sqrt",   FUNC_SQRT},
  {"ln",     FUNC_LN},
};

static const WordToken CONSTS[] = {
  {"pi", CONST_PI},
  {"e", CONST_E}
};

static const double CONSTS_DEF[] = {
  [CONST_PI] = M_PI,
  [CONST_E] = M_E
};

static void SyntaxError();
static void Require(const char** str, char c);

static Node* GetGrammar(const char* expression);
static Node* GetExpression(const char** str);
static Node* GetTerm(const char** str);
static Node* GetExponent(const char** str);
static Node* GetPriority(const char** str);
static Node* GenFunction(const char** str);
static Node* GetMathConst(const char** str);
static Node* GetVariable(const char** str);
static Node* GetNumber(const char** str);

// ----------------------> Definitions <----------------------

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
    Node* value = GetExpression(str);
    Require(str, ')');
    return value;
  }
  return GenFunction(str);
}

static Node* GenFunction(const char** str) {
  size_t size = sizeof(FUNCTIONS) / sizeof(FUNCTIONS[0]);

  for (size_t i = 0; i < size; ++i) {
    size_t token_size = strlen(FUNCTIONS[i].token);
    if (strncmp(*str, FUNCTIONS[i].token, token_size) == 0) {
      *str += token_size;

      Require(str, '(');
      Node* value = GetExpression(str);
      Require(str, ')');

      return CreateNode(NODE_FUNCTION, 
                        FunctionWrapper(FUNCTIONS[i].id),
                        NULL, 
                        value);
    }
  }

  return GetMathConst(str);
}

static Node* GetMathConst(const char** str) {
  size_t size = sizeof(CONSTS) / sizeof(CONSTS[0]);

  for (size_t i = 0; i < size; ++i) {
    size_t token_size = strlen(CONSTS[i].token);
    if (strncmp(*str, CONSTS[i].token, token_size) == 0) {
      *str += token_size;

      return CreateNode(NODE_CONST, 
                        ConstWrapper(CONSTS_DEF[CONSTS[i].id]),
                        NULL, 
                        NULL);
    }
  }

  return GetVariable(str);
}

static Node* GetVariable(const char** str) {
  const char* start = *str;
  size_t count = 0;
  while ('a' <= **str && **str <= 'z' ||
         'A' <= **str && **str <= 'Z'
  ) {
    ++count;
    ++(*str);
  }

  if (count > 0) {
    char* var_str = (char*) calloc(count + 1, sizeof(char));
    strncpy(var_str, start, count);
    return CreateNode(NODE_VARIABLE,
                      VariableWrapper(var_str),
                      NULL, NULL);
  }

  return GetNumber(str);
}

static Node* GetNumber(const char** str) {
  double sign = 1;
  if (**str == '-') {
    sign = -1;
    ++(*str);
  }

  const char* last = *str;
  
  double val = 0;
  while('0' <= **str && **str <= '9') {
    val = val * 10 + (**str - '0');
    ++(*str);
  }

  if (**str == '.') {
    ++(*str);

    double frac = 0;
    double digit = 1;

    while('0' <= **str && **str <= '9') {
    frac = frac * 10 + (**str - '0');
    digit *= 10;
    ++(*str);
    }

    val += frac / digit;
  }

  if (last == *str) SyntaxError();

  return CreateNode(NODE_CONST, ConstWrapper(val * sign), NULL, NULL);
}
