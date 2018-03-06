#include <stdio.h>
#include "splaysort.h"

int main(int argc, char *argv[]) {
	puts("============ splaysort TEST ==========");
	int vals[] = {6, 3, 5, 2, 1, 4, 9, 7};
	int n = 8;
	int i;

	splaytree_t * t = splaytree_init(n);
	for(i =0; i<n; i++){
			splaytree_insert(t, vals[i]);
	}
	splaytree_print(t);

	return 0;
}