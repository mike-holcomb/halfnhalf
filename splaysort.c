#include "splaysort.h"
#include <stdlib.h>
#include <stdio.h>
//// PRIVATE -- NOT IN API

int splaynode_init(splaynode_t * n, splaynode_t * p, long val) {
	if(n->init == 1) {
		return 1;
	}

	n->init   = 1;
	n->value  = val;  // Value of current node
    n->count  = 1;    // Frequency of values
    n->parent = p;   // Pointer to parent
    n->left   = 0;     // Pointer to left child
    n->right  = 0;    // Pointer to right child

	return 0;
}

int splaynode_increment(splaynode_t *n) {
	return ++(n->count);
}

splaynode_t * const splaynode_insert(splaynode_t * const node, splaynode_t * const next, const long val) {
	if(node->value == val) {
		splaynode_increment(node);
		return node;
	} else if (node->value >= val) {
		if(node->left == 0) {
			splaynode_init(next, node, val);
			return node->left = next;
		}
		return splaynode_insert(node->left, next, val);
	} else {
		if(node->right == 0) {
			splaynode_init(next, node, val);
			return node->right = next;
		}
		return splaynode_insert(node->right, next, val);
	}
}

void splaynode_print(splaynode_t *n, int level ) {
	if(n->init == 0)
		return;
	int i =0;

	for(;i<level;i++){
		printf("-");
	}
	printf(" %ld\n", n->value);
	if(n->left != 0) {
		splaynode_print(n->left, level+1);
	}
	
	if(n->right != 0) {
		splaynode_print(n->right, level+1);
	}
}

//// PUBLIC IN API

splaytree_t * splaytree_init(long capacity) {
	splaytree_t * r = malloc(sizeof(splaytree_t));
	r->capacity = capacity;
	r->size     = 0;
	r->root     = 0;
	r->count    = 0;
	r->nodes    = malloc(capacity * sizeof(splaynode_t));

	return r;
}

void splaytree_destroy(splaytree_t * t){
	free(t->nodes);
	free(t);
}

void splaytree_insert(splaytree_t * t, long val){
	if(t->size == 0) {
		splaynode_init(t->nodes, 0, val);
		t->root = t->nodes;
		t->size++;
		t->count++;
		return;
	}
	splaynode_t * next = t->nodes + t->size;
	splaynode_t * ret  = splaynode_insert(t->root, next, val);
	if(ret == next){
		t->size++;
		t->count++;
	} else {
		t->count++;
	}
	//  splaytree_splay(t, ret);
}

void splaytree_print(splaytree_t * t) {
	puts("Printing tree");
	splaynode_print(t->root, 0);
	puts("");
}

