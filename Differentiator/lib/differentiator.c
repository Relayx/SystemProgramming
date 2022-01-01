#include "includes/expressionTree.h"

#include <stdlib.h>

// ----------------------> Declarations <----------------------

static Node* Differentiate(const Node* node);

static Node* DifferentiateConst(const Node* node);

static Node* DifferentiateVariable(const Node* node);

static Node* DifferentiateOperation(const Node* node);

static Node* DifferentiateFunction(const Node* node);

static NodeValue OperationWrapper(TreeNodeOperation operation);

static NodeValue ConstWrapper(double number);

static NodeValue FunctionWrapper(TreeNodeFunction function);

// ----------------------> Aliases <----------------------

#define DL Differentiate(node->left)

#define DR Differentiate(node->right)

#define CL CopySubTree(node->left)

#define CR CopySubTree(node->right)

#define ADD(left, right) \
  CreateNode(NODE_OPERATION, OperationWrapper(OP_ADD), left, right)

#define SUB(left, right) \
  CreateNode(NODE_OPERATION, OperationWrapper(OP_SUB), left, right)

#define MUL(left, right) \
  CreateNode(NODE_OPERATION, OperationWrapper(OP_MUL), left, right)

#define DIV(left, right) \
  CreateNode(NODE_OPERATION, OperationWrapper(OP_DIV), left, right)

#define EXP(left, right) \
  CreateNode(NODE_OPERATION, OperationWrapper(OP_EXP), left, right)

#define FUNC(func, left, right) \
  CreateNode(NODE_FUNCTION, FunctionWrapper(func), left, right)

#define CONST(val) \
  CreateNode(NODE_CONST, ConstWrapper(val), NULL, NULL)

// ----------------------> Definitions <----------------------

Tree TreeDifferentiate(const Tree* tree) {
  Tree result = {0};
  result.root = Differentiate(tree->root);
  return result;
}

static Node* Differentiate(const Node* node) {
  switch (node->type) {

    case NODE_CONST: {
      return DifferentiateConst(node);
      break;
    }

    case NODE_OPERATION: {
      return DifferentiateOperation(node);
      break;
    }

    case NODE_FUNCTION: {
      return DifferentiateFunction(node);
      break;
    }

    case NODE_VARIABLE: {
      return DifferentiateVariable(node);
      break;
    }

    case NODE_ERROR:
    default: {

      break;
    }

  }
}

static Node* DifferentiateConst(const Node* node) {
  return CreateNode(NODE_CONST, ConstWrapper(0.), NULL, NULL);
}

static Node* DifferentiateVariable(const Node* node) {
  return CreateNode(NODE_CONST, ConstWrapper(1.), NULL, NULL);
}

static Node* DifferentiateOperation(const Node* node) {
  switch (node->content.operation) {

    case OP_ADD: {
      return ADD(DL, DR);
      break;
    }

    case OP_SUB: {
      return SUB(DL, DR);
      break;
    }

    case OP_MUL: {
      return ADD(MUL(DL, CR), MUL(CL, DR));
      break;
    }

    case OP_DIV: {
      DIV(SUB(MUL(DL, CR), MUL(CL, DR)), MUL(CR, CR));
      break;
    }

    default: {

      break;
    }

  }
}

static Node* DifferentiateFunction(const Node* node) {
  switch (node->content.function) {

    case FUNC_SIN: {
      return
      MUL(
        FUNC(FUNC_COS, NULL, CR), 
        DR
      );
      break;
    }

    case FUNC_COS: {
      return
      MUL(
        MUL(
          FUNC(FUNC_SIN, NULL, CR), 
          CONST(-1.)
        ), 
        DR
      );
      break;
    }

    case FUNC_TG: {
      return
      MUL(
        DIV(
          CONST(1.),
            EXP(
              FUNC(FUNC_COS, NULL, CR),
              CONST(2.)
            )
        ),
        DR
      );
      break;
    }

    case FUNC_CTG: {
      return
      MUL(
        MUL(
          DIV(
            CONST(1.),
            EXP(
              FUNC(FUNC_SIN, NULL, CR),
              CONST(2.)
            )
          ),
          CONST(-1.)
        ),
        DR
      );
      break;
    }

    case FUNC_ARCSIN: {
      return
      MUL(
        DIV(
          CONST(1.),
          FUNC(FUNC_SQRT, NULL,
            SUB(
              CONST(1.),
              EXP(
                CR,
                CONST(2.)
              )
            )
          )
        ),
        DR
      );
      break;
    }

    case FUNC_ARCCOS: {
      return
      MUL(
        MUL(
          DIV(
            CONST(1.),
            FUNC(FUNC_SQRT, NULL,
              SUB(
                CONST(1.),
                EXP(
                  CR,
                  CONST(2.)
                )
              )
            )
          ),
          CONST(-1.)
        ),
        DR
      );
      break;
    }

    case FUNC_ARCTG: {
      return
      MUL(
        DIV(
          CONST(1.),
          ADD(
            CONST(1.),
            EXP(
              CR,
              CONST(2.)
            )
          )
        ),
        DR
      );
      break;
    }

    case FUNC_ARCCTG: {
      return
      MUL(
        MUL(
          DIV(
            CONST(1.),
            ADD(
              CONST(1.),
              EXP(
                CR,
                CONST(2.)
              )
            )
          ),
          CONST(-1.)
        ),
        DR
      );
      break;
    }

    default: {

      break;
    }

  }
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

// ----------------------> Clear <----------------------

#undef DL
#undef DR
#undef CL
#undef CR
#undef ADD
#undef SUB
#undef MUL
#undef DIV
#undef EXP
#undef FUNC
#undef CONST
