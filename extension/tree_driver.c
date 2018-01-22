#include "tree.h"

int main() {
  Tree* t;
  Token token[6];
  Node* node_with_childs;

  strcpy(token[0].str, "1");
  strcpy(token[1].str, "2");
  strcpy(token[2].str, "3");
  strcpy(token[3].str, "4");
  strcpy(token[4].str, "5");
  strcpy(token[5].str, "6");


  t = tree_init(token[0]);

  tree_insert_child(t->root, token[1]);
  tree_insert_child(t->root, token[2]);
  node_with_childs = tree_insert_child(t->root, token[3]);
  tree_insert_child(node_with_childs, token[4]);
  tree_insert_child(node_with_childs, token[5]);
  printf("number of children: %d\n", node_with_childs->n);

  tree_print(t->root);

  return 1;
}
