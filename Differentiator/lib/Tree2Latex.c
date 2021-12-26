#include "includes/expressionTree.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// ----------------------> Declarations <----------------------

static const char* TEX_MAIN = " \
  \\documentclass{article} \n\
  \\begin{document} \n\
    $%s$ \n\
  \\end{document} \n\
";

static const char* TEX_TEMPLATES[] = {
  [NODE_ERROR] = "!ERROR!",
  [OP_ADD] = "%s + %s",
  [OP_SUB] = "%s - (%s)",
  [OP_MUL] = "(%s) \\cdot (%s)",
  [OP_DIV] = "\\frac{%s}{%s}",
  [OP_EXP] = "{%s^{%s}}",
  [FUNC_SIN] = "sin(%s)",
  [FUNC_COS] = "cos(%s)",
  [FUNC_TG] = "tg(%s)",
  [FUNC_CTG] = "ctg(%s)",
  [FUNC_LN] = "ln(%s)",
  [FUNC_ARCSIN] = "arcsin(%s)", 
  [FUNC_ARCCOS] = "arccos(%s)",
  [FUNC_ARCTG] = "arctg(%s)",
  [FUNC_ARCCTG] = "arcctg(%s)",
  [FUNC_SH] = "sh(%s)",
  [FUNC_CH] = "ch(%s)",
  [FUNC_TH] = "th(%s)",
  [FUNC_LOG] = "log_{%s}{%s}",
  [FUNC_SQRT] = "\\sqrt{%s}"
};

static char* _Tree2Latex(const Node* node);

static char* FormatString(const char* format, ...);

// ----------------------> Definitions <----------------------

void Tree2Latex(const Tree* tree) {
  FILE* fout;
  if ((fout = fopen("lat.tex", "w")) == NULL) {
    return;
  }

  char* str = _Tree2Latex(tree->root);
  fprintf(fout, TEX_MAIN, str);

  fclose(fout);
  free(str);

  system("pdflatex lat.tex");

  return;
}

///////////////////////////////////////////////////////////////

static char* _Tree2Latex(const Node* node) {
  switch (node->type) {
    case NODE_CONST: {
      char* res = FormatString("%lg", node->content.value);
      return res;
      break;
    }

    case NODE_ERROR: {

      break;
    }

    case NODE_OPERATION: {
      char* left = _Tree2Latex(node->left);
      char* right = _Tree2Latex(node->right);
      char* res = FormatString(TEX_TEMPLATES[node->content.operation],
                               left,
                               right);
      free(left);
      free(right);
      return res;
      break;
    }

    case NODE_FUNCTION: {
      char* value = _Tree2Latex(node->right);
      char* res = FormatString(TEX_TEMPLATES[node->content.function],
                         value);
      free(value);
      return res;
      break;
    }

    case NODE_VARIABLE: {
      char* res = FormatString("%c", node->content.variable);
      return res;
      break;
    }

    default: {

      break;
    }
  }
}

///////////////////////////////////////////////////////////////

static char* FormatString(const char* format, ...) {
  va_list args;
  va_start(args, format);
  size_t needed = vsnprintf(NULL, 0, format, args);
  char *buffer = malloc(needed + 1);
  vsprintf(buffer, format, args);
  va_end(args);
  return buffer;
}
