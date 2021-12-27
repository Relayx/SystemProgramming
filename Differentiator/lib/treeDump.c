#include "includes/expressionTree.h"

#include <stdio.h>
#include <stdlib.h>

// ----------------------> Declarations <----------------------

typedef struct NodeInfo_ {
  const char* shape;
  const char* value;
  int         color;
  int         index;
} NodeInfo;

static const char* GRAPH_SETTINGS = " \
  digraph Tree{ \n\
  node [colorscheme=set312] \n\
  edge [colorscheme=paired12] \n\
  rankdir=UD \n\
";

static const char* NODE_DECRIPTION = " \
  node%d [ \
  shape=\"%s\", \
  style=\"rounded, bold, filled\", \
  fillcolor=%d, \
  fontcolor=\"white\", \
  label=\"%s\", \
  width=0.9, \
  height=0.9, \
  fontname=\"Impact\" \
  ]";

static const char* NODE_SHAPES[] = {
  [NODE_ERROR]    = "none",
  [NODE_CONST]    = "circle",
  [NODE_VARIABLE] = "doublecircle",
  [NODE_FUNCTION] = "note",
  [OP_ADD]        = "square",
  [OP_SUB]        = "triangle",
  [OP_MUL]        = "hexagon",
  [OP_DIV]        = "diamond",
  [OP_EXP]        = "house"
};

static const char* NODE_VALUES[] = {
  [NODE_ERROR]  = "!",
  [OP_ADD]      = "+",
  [OP_SUB]      = "-",
  [OP_MUL]      = "×",
  [OP_DIV]      = "/",
  [OP_EXP]      = "^",
  [FUNC_SIN]    = "sin",
  [FUNC_COS]    = "cos",
  [FUNC_TG]     = "tg",
  [FUNC_CTG]    = "ctg",
  [FUNC_LN]     = "ln",
  [FUNC_ARCSIN] = "arcsin",
  [FUNC_ARCCOS] = "arccos",
  [FUNC_ARCCTG] = "arctg",
  [FUNC_ARCCTG] = "arcctg",
  [FUNC_SQRT]   = "sqrt",
  [FUNC_SH]     = "sh",
  [FUNC_CH]     = "ch",
  [FUNC_TH]     = "th"
};

static NodeInfo GetNodeInfo(const Node* node);

static void PrintNode(const Node* node, 
                      const FILE* fout, 
                      const NodeInfo* info);

static size_t _TreeDump(const Node* node, 
                        const FILE* fout, 
                        size_t index);

// ----------------------> Definitions <----------------------

void TreeDump(const Tree* tree) {
  FILE* fout;
  if ((fout = fopen("expression.txt", "w")) == NULL) {
    return;
  }

  fprintf(fout, GRAPH_SETTINGS);

  _TreeDump(tree->root, fout, 1);

  fprintf(fout, "}\n");
  fclose(fout);

  system("R:\\Programming\\C\\SystemProgramming\\Differentiator\\Graphviz\\bin\\dot.exe expression.txt -Tpng -o test.png");
  system("start .\\test.png");

  return;
}

///////////////////////////////////////////////////////////////

static size_t _TreeDump(const Node* node, 
                        const FILE* fout, 
                        size_t index
) {
  NodeInfo info = GetNodeInfo(node);
  info.index = index;

  PrintNode(node, fout, &info);

  if (node->left != NULL) {
    size_t next_index = _TreeDump(node->left, fout, 2 * index);
    fprintf(fout, "node%d -> node%d\n", index, next_index);
  }
  if (node->right != NULL) {
    size_t next_index = _TreeDump(node->right, fout, 2 * index + 1);
    fprintf(fout, "node%d -> node%d\n", index, next_index);
  }

  return index;
}

///////////////////////////////////////////////////////////////

static NodeInfo GetNodeInfo(const Node* node) {
  NodeInfo info = {0};

  switch (node->type) {

    case NODE_OPERATION: {
      info.color = node->content.operation;
      info.shape = NODE_SHAPES[node->content.operation];
      info.value = NODE_VALUES[node->content.operation];
      break;
    }

    case NODE_FUNCTION: {
      info.color = node->type;
      info.shape = NODE_SHAPES[node->type];
      info.value = NODE_VALUES[node->content.function];
      break;
    }

    case NODE_CONST:
    case NODE_VARIABLE: {
        info.color = node->type;
        info.shape = NODE_SHAPES[node->type];
      break;
    }

    default: {
      info.color = node->type;
      info.shape = NODE_SHAPES[node->type];
      info.value = NODE_VALUES[node->type];
      break;
    }

  }

  return info;
}

///////////////////////////////////////////////////////////////

static void PrintNode(const Node* node, 
                      const FILE* fout, 
                      const NodeInfo* info
) {
  switch (node->type) {

    case NODE_CONST: {
      size_t needed = snprintf(NULL, 0, "%lg", node->content.value);
      char double_str[needed + 1];
      sprintf(double_str, "%lg", node->content.value);
      fprintf(fout, NODE_DECRIPTION, info->index, info->shape, 
              info->color, double_str);
      break;
    }

    case NODE_VARIABLE: {
      fprintf(fout, NODE_DECRIPTION, info->index, info->shape, 
              info->color, &node->content.variable);
      break;
    }

    default: {
      fprintf(fout, NODE_DECRIPTION, info->index, info->shape, 
              info->color, info->value);
      break;
    }

  }
}
