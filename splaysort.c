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
	char m = (n->count > 1) ? '*' : ' ';
	printf(" %ld %c\n", n->value, m);
	if(n->left != 0) {
		splaynode_print(n->left, level+1);
	}
	
	if(n->right != 0) {
		splaynode_print(n->right, level+1);
	}
}

void splaynode_splay(splaynode_t * const node, splaynode_t * const root) {
	if(node == root)
		return;
	// ZIG
	if(node->parent == root) {
		if(root->left == node) {
			puts("Zig Left");
			root->left = node->right;
			if(root->left != 0) root->left->parent = root;
			root->parent = node;
			node->right = root;
			node->parent = 0;
			return;
		} else {
			puts("Zig Right");
			root->right = node->left;
			if(root->right != 0) root->right->parent = root;
			root->parent = node;
			node->left = root;
			node->parent = 0;
			return;
		}
	} else {
		splaynode_t *y = node->parent;
		splaynode_t *x = y->parent;
		splaynode_t *g = x->parent;
		int isRoot = (x == root) ? 1 : 0;
		if(!isRoot) {
			node->parent = g;
			if(g->left == x) {
				g->left = node;
			} else{
				g->right = node;
			}
		}
		if(x->left == y) {
			if(y->left == node) { // ZIG-ZIG
				puts("Zig Zig Left");
				x->left = y->right;
				if(x->left != 0) x->left->parent = x;
				y->right = x;
				x->parent = y;

				y->left = node->right;
				if(y->left != 0) y->left->parent = y;
				node->right = y;
				y->parent = node;
			} else {              // ZIG-ZAG
				puts("Zig Zag Left");
				x->left = node->right;
				if(x->left != 0) x->left->parent = x;
				node->right = x;
				x->parent = node;

				y->right = node->left;
				if(y->right != 0) y->right->parent = x;
				y->parent = node;
				node->left = y;
			}
		} else {      
			if(y->left == node) { // ZIG-ZAG
				puts("Zig Zag Right");
				x->right = node->left;
				if(x->right != 0) x->right->parent = x;
				node->left = x;
				x->parent = node;

				y->left = node->right;
				if(y->left != 0) y->left->parent = y;
				node->right = y;
				y->parent = node;
			} else {              // ZIG-ZIG
				puts("Zig Zig Right");
				x->right = y->left;
				if(x->right != 0) x->right->parent = x;
				y->left = x;
				x->parent = y;

				y->right = node->left;
				if(y->right != 0) y->right->parent = x;
				node->left = y;
				y->parent = node;
			}
		}
		if(isRoot)
			return;
		splaynode_splay(node, root);
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
		splaynode_splay(ret, t->root);
		t->root = ret;
	} else {
		t->count++;
	}

}

void splaytree_print(splaytree_t * t) {
	puts("Printing tree");
	splaynode_print(t->root, 0);
	puts("");
}
