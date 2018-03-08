#include <stdio.h>
#include <stdlib.h>
#include "splaysort.h"

int main(int argc, char *argv[]) {
	puts("============ splaysort TEST ==========");
	int vals[] = {6, 3, 5, 2, 2, 4, 9, 7};
	int n = 8;
	int i;

	splaytree_t * t = splaytree_init(n+2);
	for(i =0; i<n; i++){
			splaytree_insert(t, vals[i]);
	}
	printf("Size: %ld Count: %ld Capacity: %ld\n", t->size, t->count, t->capacity);
	
	//splaytree_print(t);

	long * arr = splaytree_fill_array(t);
	
	for(i=0; i < n; i++) {
		printf("%ld ",arr[i]);
	}
	puts("");

	splaytree_destroy(t);

	free(arr);

	return 0;
}