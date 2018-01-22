/*
 * tree.c
 *
 *  Created on: Jan 7, 2018
 *      Author: Evan Lalopoulos
 */

#include "tree.h"

Node* create_node(Token token);

Tree* tree_init(Token token) {
	Tree* t;

	t = (Tree*) malloc(sizeof(Tree));

	if (t == NULL) {
		ON_ERROR("Creation of tree failed. Memory allocation failed");
	}

	t->root = create_node(token);
	t->root->parent = NULL;

	return t;
}

Node* tree_insert_child(Node* parent, Token token) {
	Node* newNode;

	if (parent == NULL) {
		ON_ERROR("Null pointer to parent.");
	}

	newNode = create_node(token);
	newNode->parent = parent;

	parent->child[parent->n] = newNode;
	parent->n++;

	/*debug*/
	tree_print(parent);
	printf("\n\n");

	return newNode;
}

Node* create_node(Token token) {
	Node* node;

	node = (Node*) malloc(sizeof(Node));
	if (node == NULL) {
		ON_ERROR("Cannot create node. Memory allocation failed.");
	}

	node->token = token;
	node->parent = NULL;
	node->n = 0;

	return node;
}

void tree_print(Node* root) {
	int i = 0;
	int j=0;

	if (root == NULL) {
		return;
	}

	printf("(%s", root->token.str);

	for(i=0; i<root->n; i++)  {
		tree_print(root->child[i]);
	}
	printf(")");
}
