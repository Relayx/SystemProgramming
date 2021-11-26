#ifndef DPOAOPDJIOAWD
#define DPOAOPDJIOAWD

typedef enum {
    NODE_ERROR = 0, 
    NODE_CONST,     
    NODE_VAR,       
    NODE_OP_PLUS,   
    NODE_OP_MUL,    
    NODE_OP_SUB,    
    NODE_OP_DIV,    
    NODE_TYPES_COUNT
} TreeNodeType;

typedef struct _NodeInfo {
    const char* const graphNodeType;
} NodeInfo;

struct _Node;
typedef struct _Node Node;

typedef struct _Node
{
    TreeNodeType type;
    int value;
    Node* left;
    Node* right;
    char name[16];
} Node;

void TreeDump(Node* root);
int TreeCalculate(Node* root);
Node* TreeParse(char* string, Node* parent);

#endif