#include "includes/expressionTree.h"
#include "includes/service.h"

#include <malloc.h>
#include <math.h>

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

TreeNodeMathConst SpecialConstTransform(double number) {
  if (CheckEpsilon(number, M_PI)) {
    return CONST_PI;
  }
  if (CheckEpsilon(number, M_E)) {
    return CONST_E;
  }
  return CONST_NOT_SPECIAL;
}

static int CheckEpsilon(double number, double local) {
  if (fabs(local - number) < EPSILON) {
    return 1;
  }
  return 0;
}
