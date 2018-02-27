//
//  halfnhalf.c
//  halfnhalf
//
//  Created by Michael Holcomb on 1/30/18.
//  Copyright © 2018 Michael Holcomb. All rights reserved.
//

#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "string.h"
#include "limits.h"
#include "math.h"

#define POINT_SUM(X, Y, i, j)  *(X+i) + *(Y+j)
//#include "halfnhalf.h"

void insert_sort(int * const x, int n){
	// INSERTION SORT BC EASY TO CODE - PRETEND ITS A O(N LOG N) SORT
	int t;
	int *xi, *xj;
	for(xi = x+1; xi < (x + n); xi++){
		xj = xi;
		t  = *xj; 
		while (xj > x && (*(xj-1) > t)) {
			*(xj) = *(xj-1);
			xj--;
		}
		*(xj) = t;
	}
}

void print_array(int * const arr, const int n){
	int * arrp;
	for(arrp = arr; arrp < arr+n; arrp++)
		printf("%d ", *arrp);
	puts("");
}

int branches = 0;
int good_branches = 0;
int vert_branches = 0;
int bf_merge_calls = 0;
int thin_cells = 0;
int k = 0;

void cosnard(int *X, int *Y, const int n) {
	int *yp = Y;
	*yp = (rand() % n) + n;
	for(yp = Y+1; yp < Y+n; yp++) 
		*yp = (rand() % n) + n + *(yp-1);
	int i;
	*X = 0;//*(Y+n-1);
	for(i=1; i<n; i++){
		*(X+i) = *(X+i-1) + *(Y+n-1-i);
		if(i==1)
			*(X+i) +=  *(Y+n-1);
	}
}

void ideal(int *X, int *Y, const int n) {
	int *yp = Y;
	*yp = (rand() % 3) + 3;
	for(yp = Y+1; yp < Y+n; yp++) 
		*yp = (rand() % 3) + 3 + *(yp-1);
	int i;
	*X = *(Y+n-1) + 1;
	for(i=1; i < n; i++) {
		*(X+i) = *(X+i-1) + *(Y+n-1)+1;
	}
}

void merge(int * const X, int * const Y, const int m,  const int n){
	if(*Y < *(X+m-1)){
		int num = m + n; // Number of elements overall
		int *tmp = (int *)malloc(m * sizeof(int)); // Temp array
		memcpy(tmp, X, m *sizeof(int));
		int *tmpp = tmp;
		int *xp = X; // loop counter for left
		int *yp = Y; // loop counter for right
		int left = *tmpp; // Largest left value
		int right = *yp; // Largest right value
		while(xp < X + num) {
			if(left < right){
				*xp = left;
				tmpp++;
				left = (tmpp >= tmp + m) ? INT_MAX : *tmpp;
			} else {
				*xp = right;
				yp++;
				right = (yp >= Y + n) ? INT_MAX : *yp;
			}
			xp++;
		}
		free(tmp);
	} else {
		memcpy(X+m,Y,n);
	}
	// print_array(X, m);
	// print_array(Y, n);
	bf_merge_calls++;
}

void merge_sort(int * const X, const int n){
	if( n < 2) // Base Case
		return;
	
	const int m = n/2; // Find Mid Point

	merge_sort(X, m);
	merge_sort(X+m, n-m);
	merge(X, X+m, m, n-m);
}
void k_merge_sort(int * const X, const int n){
	if( n < k) {// Base Case{
		insert_sort(X,n);
		return;
	}
	
	const int m = n/2; // Find Mid Point

	k_merge_sort(X, m);
	k_merge_sort(X+m, n-m);
	merge(X, X+m, m, n-m);
}

void  thin_sort(int X, int * const Y, int * const Z, int n){
	int * t = Z;
	int i;
	for(i = 0; i < n; i++){
		*(t+i) = X + *(Y+i);
	}
	thin_cells++;
}

void gen_z(int * const X, int * const Y, int * const Z, const int m, const int n){
	int * rp = Z;
	int * xp, * yp;
	for(xp = X; xp < X + m; xp++ ) {
		for(yp = Y; yp < Y + n; yp++){
			*rp = *xp + *yp;
			rp++;
		}
	}
}

void hh_sort(int * const X, int * const Y, int * const Z, const int m, const int n) {
	if(m == 1) {
		thin_sort(*X, Y, Z, n);
		return;
	} 

	if(n == 1) {
		thin_sort(*Y, X, Z, m);
		return;
	}

	if(m*n < k) {
		//printf("%d * %d = %d < %d\n",m,n,m*n,k);
		gen_z(X,Y,Z,m,n);
		insert_sort(Z,m*n);
		thin_cells++;
		return;
	}

	int m2 = m >> 1;
	int n2 = n >> 1;

	int diff_horz = (POINT_SUM(X,Y,m2-1,n-1)) - (POINT_SUM(X,Y,m2,0));
	int diff_vert = (POINT_SUM(X,Y,m-1,n2-1)) - (POINT_SUM(X,Y,0,n2));
	// printf("h: %d, v: %d\t hp1: %d, hp2: %d\t vp1: %d, vp2: %d\n", 
	// 	diff_horz, diff_vert, 
	// 	POINT_SUM(X,Y,m2-1,n-1), POINT_SUM(X,Y,m2,0),
	// 	POINT_SUM(X,Y,m-1,n2-1), POINT_SUM(X,Y,0,n2));
	int l1,l2;

	if (diff_horz < diff_vert) {
		hh_sort(X, Y, Z, m2, n);
		l1 = m2*n;
		hh_sort(X+m2, Y, Z+l1, m - m2,n);
		l2 = (m-m2)*n;
		if (diff_horz > 0) {
			merge(Z, Z+l1, l1, l2);
			//printf("horz: ");
		}
		// } else {
		// 	branches++;
		// 	good_branches++;
		// }
		branches++;
	}
	else {
		hh_sort(X, Y, Z, m, n2);
		l1 = m * n2;
		hh_sort(X, Y+n2, Z+l1, m,n-n2);
		l2 = (n-n2)*m;

		if (diff_vert > 0) {
			merge(Z, Z+l1, l1, l2);
			//printf("vert: ");
		}// else {
		// 	branches++;
		// 	good_branches++;
		// }
		branches++;
		vert_branches++;
	}
	// print_array(Z,l1);
	// print_array(Z+l1,l2);
	//print_array(Z,l1+l2);
}



void brute_force(int * const X, int * const Y, int * const Z, const int m, const int n){
	k_merge_sort(Z, m *n);
}
int compare(int * const Z, int * const Zbf, const int n){
	int i;
	for(i = 0; i < n; i++){
		if(*(Z+i) != *(Zbf+i))
			printf("%d: %d not equal %d\n", i, *(Z+i), *(Zbf+i));
	}
	return 0;
}

int main(int argc, char *argv[]) {
	srand(time(NULL)); // SEED RANDOM
	int n = 500;     // Set problem size
	int i, j; // indices for iteration
	
	unsigned int MAX_INT = (1<< 15);          // Set range of values in X, Y
	
	// PRINT PREAMBLE
	puts("Testing Sum XY Range Sort for X + Y");
	puts("==========================================");
	
	for(j= 0; j < 5; j++) {
		n = n*2;
		k = 2;//log2(n)*log2(n);

		printf("N = %d\t MAX_INT = %d\t", n, MAX_INT);
		int * X = (int *)malloc(n*sizeof(int));
		int * Y = (int *)malloc(n*sizeof(int));
		int * Z = (int *)malloc(n*n*sizeof(int));
		int  *Zbf = (int *)malloc(n*n*sizeof(int));
		
		// POPULATE X, Y

		for( i = 0; i < n; i++) {
			*(X+i) = rand() % MAX_INT + rand()% MAX_INT;
			*(Y+i) = rand() % MAX_INT + rand()% MAX_INT;
		}
	
		// SORT X, Y
		 merge_sort(X, n);
		merge_sort(Y, n);
		// cosnard(X,Y,n);
		//ideal(Y,X,n);
		//Print arrays
		// puts("n\tX\tY");
		// for( i = 0; i < n; i++){
		// 	printf("%d\t%d\t%d\n",i, X[i], Y[i]);
		// }

		clock_t start1, end1, start2, end2;
		double cpu_time_used1, cpu_time_used2;
		start1 = clock();
		hh_sort(X, Y, Z, n, n);
		end1 = clock();
		cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
	printf("Half n Half: %0.3f\t ", cpu_time_used1);
		
		gen_z(X, Y, Zbf, n, n);
		start2 = clock();
		bf_merge_calls=0;
		brute_force(X, Y, Zbf, n, n);
		end2 = clock();
		cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
		printf("Brute Force: %0.3f bf_branch: %d\t",cpu_time_used2, bf_merge_calls);

		//printf("Branch factor: %0.2f\t branches: %d/%d/%d\n",(good_branches*100.0)/branches, vert_branches, good_branches, branches);
		printf("thin_cells: %d (avg: %0.1f)\n",thin_cells,(n*n)/(double)thin_cells);

		compare(Z, Zbf, n*n);
		// print_array(Z,n*n);
		// print_array(Zbf,n*n);
		free(X);
		free(Y);
		free(Z);
		free(Zbf);
		branches= 0;
		good_branches = 0;
		vert_branches = 0;
		thin_cells=0;
	}
	
	return 0;
}

