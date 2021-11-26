#include "includes/expressionTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

NodeInfo NODES_INFO[] = {
    [NODE_ERROR] = {"none"},
    [NODE_CONST] = {"circle"},
    [NODE_VAR] = {"doublecircle"},
    [NODE_OP_PLUS] = {"square"},
    [NODE_OP_MUL] = {"hexagon"},
    [NODE_OP_SUB] = {"triangle"},
    [NODE_OP_DIV] = {"diamond"}
};

void _TreeDump(Node* node, FILE* fout);

void TreeDump(Node* root) {
    srand(time(NULL));
    FILE* fout;
    if ((fout = fopen("input.txt", "w")) == NULL)
    {
        return;
    }

    fprintf(fout, "digraph Tree{\nnode [colorscheme=set312]\nedge [colorscheme=paired12]\nrankdir=UD\n");

    _TreeDump(root, fout);

    fprintf(fout, "}\n");
    fclose(fout);

    system("R:\\Programming\\C\\SystemProgramming\\Differentiator\\Graphviz\\bin\\dot.exe input.txt -Tpng -o test.png");
    system("start .\\test.png");

    return;
}

void _TreeDump(Node* node, FILE* fout) {
    for (int i = 0; i < 16; i++)
    {
        node->name[i] = rand() % 25 + 65;
    }
    node->name[15] = '\0';

    if (node->type == NODE_CONST) {
    fprintf(fout, "%s [shape=\"%s\", style=\"rounded, bold, filled\", fillcolor=%d, \
color=\"darkslategray\", fontcolor=\"white\", label=<<B>%d</B>>, width=0.9, height=0.9]\n", 
    node->name, NODES_INFO[node->type].graphNodeType, node->type, node->value);
    } else {
    fprintf(fout, "%s [shape=\"%s\", style=\"rounded, bold, filled\", fillcolor=%d, \
color=\"darkslategray\", fontcolor=\"white\", label=<<B>%c</B>>, width=0.9, height=0.9]\n", 
    node->name, NODES_INFO[node->type].graphNodeType, node->type, (char)node->value);
    }

    if (node->left != NULL) {
        _TreeDump(node->left, fout);
        fprintf(fout, "%s -> %s\n", node->name, node->left->name);
    }
    if (node->right != NULL) {
        _TreeDump(node->right, fout);
        fprintf(fout, "%s -> %s\n", node->name, node->right->name);
    }
}

int TreeCalculate(Node* root) 
{
    switch (root->type)
    {
        case NODE_CONST: 
        {
            return root->value;
            break;
        }

        case NODE_OP_PLUS: 
        {
            return TreeCalculate(root->left) + TreeCalculate(root->right);
            break;
        }

        case NODE_OP_MUL: 
        {
            return TreeCalculate(root->left) * TreeCalculate(root->right);
            break;
        }

        case NODE_OP_SUB: 
        {
            return TreeCalculate(root->left) - TreeCalculate(root->right);
            break;
        }

        case NODE_OP_DIV: 
        {
            return TreeCalculate(root->left) / TreeCalculate(root->right);
            break;
        }
    }
}

Node* TreeParse(char* string, Node* parent)
{
    if (*string == '\0') return NULL;
    if (*string == ')') {
        return parent;
    }
    if (*string == '(') {
        Node* new_node = malloc(sizeof(Node));
        parent->left = TreeParse(++string, new_node);
        parent->right = TreeParse(++string, new_node);
    }
    if (*string == '+') {
        parent->type = NODE_OP_PLUS;
        parent->value = '+';
        return TreeParse(++string, parent);
    }
    if (*string == '*') {
        parent->type = NODE_OP_MUL;
        parent->value = '*';
        return TreeParse(++string, parent);
    }
    if (*string >= '0' && *string <= '9') {
        parent->type = NODE_CONST;
        parent->value = (int)(*string - '0');
        return TreeParse(++string, parent);
    }
}