#include <expressionTree.h>

double AddViaLib(double a, double b);

int main()
{

    for (int i = 0; i < 1000; i++)
        for (int j = 0; j < 1000; j++)
        {
            double a = i;
            double b = j;

            if (AddViaLib(a, b) != a + b) return 1;
        }

    return 0;

}

double AddViaLib(double a, double b)
{
    Tree tree;

    NodeValue node_left_val;
    NodeValue node_right_val;

    node_left_val.value = a;
    node_right_val.value = b;

    Node left;
    Node right;

    left.left = (void*)0;
    left.right = (void*)0;
    right.left = (void*)0;
    right.right = (void*)0;

    left.content = node_left_val;
    left.type = NODE_CONST;

    right.content = node_right_val;
    right.type = NODE_CONST;

    NodeValue rootValue;
    rootValue.operation = OP_ADD;

    Node root;

    root.type = NODE_OPERATION;
    root.content = rootValue;

    root.left = &left;
    root.right = &right;

    tree.root = &root;

    TreeDump(&tree);

    return TreeCalculate(&tree);
}