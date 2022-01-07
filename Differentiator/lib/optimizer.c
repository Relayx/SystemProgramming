#include "includes/expressionTree.h"

#include <stdlib.h>

// ----------------------> Declarations <----------------------

static void _TreeOptimize(Node* node, size_t* count);

static size_t ZeroConvolution(Node* node);

static size_t UnitConvolution(Node* node);

static size_t CalculateConvolution(Node* node);

// ----------------------> Aliases <----------------------

#define MAKELEAF(NODE)  \
  DeleteSubTree(NODE->left); \
  DeleteSubTree(NODE->right); \
  NODE->left = NULL; \
  NODE->right = NULL;

#define COPY(TO, FROM) \
  *TO = *FROM

#define ISOP(OP, NODE) \
  (NODE->type == NODE_OPERATION && NODE->content.operation == OP)

#define IS0(NODE) \
  (NODE->type == NODE_CONST && NODE->content.value == 0.)

#define IS1(NODE) \
  (NODE->type == NODE_CONST && NODE->content.value == 1.)

// ----------------------> Definitions <----------------------

size_t TreeOptimize(Tree* tree) {
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
  if (ISOP(OP_MUL, node)) {
    if (IS0(node->left) || IS0(node->right)) {
      node->type = NODE_CONST;
      node->content.value = 0.;

      MAKELEAF(node);

      return 1;
    }
  }

  if (ISOP(OP_DIV, node)) {
    if (IS0(node->right)) {
      node->type = NODE_ERROR;

      MAKELEAF(node);

      return 1;
    } else if (IS0(node->left)) {
      node->type = NODE_CONST;
      node->content.value = 0.;

      MAKELEAF(node);

      return 1;
    }
  }

  if (ISOP(OP_ADD, node)) {
    if (IS0(node->right)) {
      DeleteSubTree(node->right);
      COPY(node, node->left);

      return 1;
    } else if (IS0(node->left)) {
      DeleteSubTree(node->left);
      COPY(node, node->right);
      
      return 1;
    }
  }

  if (ISOP(OP_SUB, node)) {
    if (IS0(node->right)) {
      DeleteSubTree(node->right);
      COPY(node, node->left);

      return 1;
    }
  }

  if (ISOP(OP_EXP, node)) {
    if (IS0(node->left)) {
      DeleteSubTree(node->right);
      COPY(node, node->left);

      return 1;
    } else if (IS0(node->right)) {
      node->type = NODE_CONST;
      node->content.value = 1.;

      MAKELEAF(node);

      return 1;
    }
  }

  return 0;
}

static size_t UnitConvolution(Node* node) {
  if (ISOP(OP_MUL, node)) {
    if (IS1(node->left)) {
      DeleteSubTree(node->left);
      COPY(node, node->right);
      
      return 1;
    } else if (IS1(node->right)) {
      DeleteSubTree(node->right);
      COPY(node, node->left);

      return 1;
    }
  }

  if (ISOP(OP_DIV, node)) {
    if (IS1(node->right)) {
      DeleteSubTree(node->right);
      COPY(node, node->left);

      return 1;
    }
  }

  if (ISOP(OP_EXP, node)) {
    if (IS1(node->left)) {
      MAKELEAF(node);
      node->type = NODE_CONST;
      node->content.value = 1.;

      return 1;
    } else if (IS1(node->right)) {
      DeleteSubTree(node->right);
      COPY(node, node->left);

      return 1;
    }
  }
  return 0;
}

static size_t CalculateConvolution(Node* node) {
  if ((node->type        == NODE_OPERATION   &&
       node->left->type  == NODE_CONST       &&
       node->right->type == NODE_CONST)

      ||

      (node->type        == NODE_FUNCTION    &&
       node->right->type == NODE_CONST)
  ) {
    Tree temp = {0};
    temp.root = node;
    double value = TreeCalculate(&temp);
    node->type = NODE_CONST;
    node->content.value = value;

    MAKELEAF(node);

    return 1;
  }
  return 0;
}

// ----------------------> Clear <----------------------

#undef MAKELEAF
#undef COPY
#undef ISOP
#undef IS0
#undef IS1
