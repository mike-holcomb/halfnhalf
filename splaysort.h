#ifndef _SPLAYSORT_H_
#define _SPLAYSORT_H_

typedef struct splaynode_t {
    long                 value;    // Value of current node
    long                 count;    // Frequency of values
    struct splaynode_t * parent;   // Pointer to parent
    struct splaynode_t * left;     // Pointer to left child
    struct splaynode_t * right;    // Pointer to right child
    int                  init;     // Boolean, 1 = initialized, 2 = empty
} splaynode_t;

typedef struct splaytree_t {
	splaynode_t * nodes;    // Pointer to space for all nodes
	splaynode_t * root;     // Pointer to root node address
	long          size;     // Number of initialized nodes
	long          count;    // Total frequency of all values
	long          capacity; // Maximum number of nodes
} splaytree_t;

splaytree_t * splaytree_init( long capacity );

void splaytree_destroy( splaytree_t * t );

void splaytree_insert( splaytree_t * t, long val );

void splaytree_print( splaytree_t * t );

long * splaytree_fill_array( splaytree_t * t );

#endif /* _SPLAYSORT_H_ */