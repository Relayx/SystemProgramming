#include "includes/expressionTree.h"

#include <stdlib.h>

// ----------------------> Declarations <----------------------

static void _TreeOptimize(Node* node, size_t* count);

static size_t ZeroConvolution(Node* node);

static size_t UnitConvolution(Node* node);

static size_t CalculateConvolution(Node* node);

// ----------------------> Aliases <----------------------

#define IsOp(OP, NODE) \
  (NODE->type == NODE_OPERATION && NODE->content.operation == OP)

#define Is0(NODE) \
  (NODE->type == NODE_CONST && NODE->content.value == 0.)

#define Is1(NODE) \
  (NODE->type == NODE_CONST && NODE->content.value == 1.)

// ----------------------> Definitions <----------------------

int TreeOptimize(Tree* tree) {
  size_t count = 0;
  _TreeOptimize(tree->root, &count);
  return count;
}

static void _TreeOptimize(Node* node, size_t* count) {
  if (node == NULL) {
    return;
  }

  *(count) += ZeroConvolution(node);
  *(count) += UnitConvolution(node);
  *(count) += CalculateConvolution(node);

  _TreeOptimize(node->left, count);
  _TreeOptimize(node->right, count);
}

static size_t ZeroConvolution(Node* node) {
  if (IsOp(OP_MUL, node)) {
    if (Is0(node->left) || Is0(node->right)) {
      DeleteSubTree(node->left);
      DeleteSubTree(node->right);
      node->type = NODE_CONST;
      node->content.value = 0.;
      node->left = NULL;
      node->right = NULL;
      return 1;
    }
  }
  return 0;
}

static size_t UnitConvolution(Node* node) {
  return 0;
}

static size_t CalculateConvolution(Node* node) {
  if ((node->type        == NODE_OPERATION   &&
      node->left->type   == NODE_CONST       &&
      node->right->type  == NODE_CONST) 
      
      ||

      (node->type        == NODE_FUNCTION    &&
       node->right->type == NODE_CONST)
  ) {
    Tree temp;
    temp.root = node;
    double value = TreeCalculate(&temp);
    node->type = NODE_CONST;
    node->content.value = value;
    node->left = NULL;
    node->right = NULL;
    return 1;
  } 
  return 0;
}
