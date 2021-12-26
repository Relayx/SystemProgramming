#include "includes/expressionTree.h"

#include <malloc.h>

// ----------------------> Declarations <----------------------

//

// ----------------------> Definitions <----------------------

Node* CreateNode(TreeNodeType type, NodeValue value, 
                 Node* left, Node* right) {

  Node* new_node = calloc(1, sizeof(Node));
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
