#include "includes/expressionTree.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ----------------------> Declarations <----------------------

static double TreeCalculateFunction(double value, 
                                    TreeNodeFunction function);

static double TreeCalculateOperation(double left,
                                     double right,
                                     TreeNodeOperation operation);

static double _TreeCalculate(const Node* node);

// ----------------------> Definitions <----------------------

double TreeCalculate(const Tree* tree) {
  return _TreeCalculate(tree->root);
}

///////////////////////////////////////////////////////////////

static double _TreeCalculate(const Node* node) {  
  switch (node->type) {

    case NODE_CONST: {
        return node->content.value;
      break;
    }

    case NODE_OPERATION: {
      double left  = _TreeCalculate(node->left);
      double right = _TreeCalculate(node->right);

      return TreeCalculateOperation(left, 
                                    right,
                                    node->content.operation);
      break;
    }

    case NODE_FUNCTION: {
      double result = _TreeCalculate(node->right);
      return TreeCalculateFunction(result, 
                                   node->content.function);
      break;
    }

    case NODE_VARIABLE: {
      // TO DO
      double value;
      printf("Please Enter the value for %c: ", node->content.variable);
      scanf("%lg", &value);
      return value;
      break;
    }

    case NODE_ERROR:
    default: {
      return NAN;
      break;
    }

  }
}

///////////////////////////////////////////////////////////////

static double TreeCalculateFunction(double value, 
                                    TreeNodeFunction function
) {
  switch (function) {

    case FUNC_COS: {
      return cos(value);
      break;
    }

    case FUNC_SIN: {
      return sin(value);
      break;
    }

    case FUNC_TG: {
      return tan(value);
      break;
    }

    case FUNC_CTG: {
      return 1.0 / tan(value);
      break;
    }

    case FUNC_ARCSIN: {
      return asin(value);
      break;
    }

    case FUNC_ARCCOS: {
      return acos(value);
      break;
    }

    case FUNC_ARCTG: {
      return atan(value);
      break;
    }

    case FUNC_ARCCTG: {
      return M_PI / 2 - atan(value);
      break;
    }

    case FUNC_SH: {
      return sinh(value);
      break;
    }

    case FUNC_CH: {
      return cosh(value);
      break;
    }

    case FUNC_TH: {
      return tanh(value);
      break;
    }

    case FUNC_SQRT: {
      printf("%lg\n", value);
      return sqrt(value);
      break;
    }

    case FUNC_LN: {
      return log(value);
      break;
    }

    default: {
      return NAN;
      break;
    }

  }
}

///////////////////////////////////////////////////////////////

static double TreeCalculateOperation(double left,
                                     double right,
                                     TreeNodeOperation operation
) {
  switch (operation) {

    case OP_ADD: {
      return left + right;
      break;
    }

    case OP_SUB: {
      return left - right;
      break;
    }

    case OP_MUL: {
      return left * right;
      break;
    }

    case OP_DIV: {
      return left / right;
      break;
    }

    case OP_EXP: {
      return pow(left, right);
      break;
    }

    default: {
      return NAN;
      break;
    }

  }
}
