#include <stdio.h>
#include <stdlib.h>
#include "lib/includes/expressionTree.h"

void Optimize(Tree* tree) {
  size_t count = TreeOptimize(tree);
  while (count) {
    count = TreeOptimize(tree);
  }
}

int main() {

  Tree tree = TreeParse("input.txt");
  Tree new_tree = TreeDifferentiate(&tree);
  Optimize(&new_tree);
  TreeDump(&new_tree);
  // printf("%lg\n", TreeCalculate(&tree));
  Tree2Latex(&new_tree);
  system("lat.pdf");

  return 0;
}
