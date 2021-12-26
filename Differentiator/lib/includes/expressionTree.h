#ifndef LIB_RELAYX_EXPRESSION_TREE_HEADER
#define LIB_RELAYX_EXPRESSION_TREE_HEADER

struct Node_;
typedef struct Node_ Node;

typedef enum {
  NODE_ERROR = 0,  // 0
  NODE_CONST,      // 1
  NODE_OPERATION,  // 2
  NODE_VARIABLE,   // 3
  NODE_FUNCTION,   // 4
  NODE_TYPES_COUNT // 5
} TreeNodeType;

typedef enum {
  OP_UNDEFINED = 0,
  OP_ADD = NODE_TYPES_COUNT + 1,
  OP_SUB,
  OP_MUL,
  OP_DIV,
  OP_TYPES_COUNT
} TreeNodeOperation;

typedef enum {
  FUNC_ERROR = 0,
  FUNC_SIN = OP_TYPES_COUNT + 1,
  FUNC_COS,
  FUNC_TG,
  FUNC_CTG,
  FUNC_LN
} TreeNodeFunction;

typedef union NodeValue_ {
  double value;
  char variable;
  TreeNodeOperation operation;
  TreeNodeFunction function;
} NodeValue;

struct Node_ {
  TreeNodeType type;
  NodeValue content;
  Node* left;
  Node* right;
};

typedef struct Tree_ {
  Node* root;
} Tree;



void TreeDump(const Tree* tree);

double TreeCalculate(const Tree* tree);

void Tree2Latex(const Tree* tree);

Node* CreateNode(TreeNodeType type, NodeValue value, 
                 Node* left, Node* right);

Node* CopySubTree(const Node* node);

Tree DifferentiateTree(const Tree* tree);

#endif