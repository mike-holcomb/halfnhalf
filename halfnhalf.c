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
//#include "halfnhalf.h"

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

int *merge(int * const X, int * const Y, const int m,  const int n){
	int num = m + n; // Number of elements overall
	int *tmp = (int *)malloc(num * sizeof(int)); // Temp array
	int *tmpp = tmp + num;
	int *xp = X + m -1; // loop counter for left
	int *yp = Y + n -1; // loop counter for right
	int left = *xp; // Largest left value
	int right = *yp; // Largest right value
	while(tmpp > tmp) {
		tmpp--;
		if(left > right){
			*tmpp = left;
			xp--;
			left = (xp < X) ? INT_MIN : *xp;
		} else {
			*tmpp = right;
			yp--;
			right = (yp < Y) ? INT_MIN : *yp;
		}
	}
	bf_merge_calls++;
	return tmp;
}

void merge_sort(int * const X, const int n){
	if( n == 1) // Base Case
		return;
	
	const int m = n/2; // Find Mid Point

	merge_sort(X, m);
	merge_sort(X+m, n-m);
	int *t = merge(X, X+m, m, n-m);
	memcpy(X,t,n*sizeof(int));
	free(t);
}

int * thin_sort(int X, int * const Y, int n){
	int * t = (int *)malloc(n * sizeof(int));
	int i;
	for(i = 0; i < n; i++){
		*(t+i) = X + *(Y+i);
	}
	return t;
}

int point_sum(int * const X, int * const Y, int i, int j){
	return *(X+i) + *(Y+j);
}

int * hh_sort(int * const X, int * const Y, int m, int n) {
	if(m == 1) {
		return thin_sort(*X, Y, n);
	}
	if(n == 1) {
		return thin_sort(*Y, X, m);
	}
	int m2 = m/2;
	int n2 = n/2;

	int diff_horz = point_sum(X,Y,m2-1,n-1) - point_sum(X,Y,m2,0);
	int diff_vert = point_sum(X,Y,m-1,n2-1) -point_sum(X,Y,0,n2);
	// printf("h: %d, v: %d\t hp1: %d, hp2: %d\t vp1: %d, vp2: %d\n", 
	// 	diff_horz, diff_vert, 
	// 	point_sum(X,Y,m2-1,n-1), point_sum(X,Y,m2,0),
	// 	point_sum(X,Y,m-1,n2-1), point_sum(X,Y,0,n2));
	int * r1, * r2, * r;
	int l1,l2;
	r=r1=r2=(int *)0;

	if (diff_horz < diff_vert) {
		r1 = hh_sort(X, Y, m2, n);
		l1 = m2*n;
		r2 = hh_sort(X+m2, Y, m - m2,n);
		l2 = (m-m2)*n;
		if (diff_horz > 0) {
			r = merge(r1, r2, m2*n, (m-m2)*n);
			branches++;
		} else {
			r = malloc(m*n*sizeof(int));
			memcpy(r, r1, m2 * n *sizeof(int));
			memcpy(r+m2*n,r2,(m - m2)*n*sizeof(int));
			branches++;
			good_branches++;
		}
	}
	else {
		r1 = hh_sort(X, Y, m, n2);
		l1 = m * n2;
		r2 = hh_sort(X, Y+n2, m,n-n2);
		l2 = (n-n2)*n;

		if (diff_vert > 0) {
			r = merge(r1, r2, m*n2, m*(n-n2));
			branches++;
		} else {
			r = malloc(m*n*sizeof(int));
			memcpy(r, r1, m*n2 * sizeof(int));
			memcpy(r+m*n2,r2, m*(n-n2)*sizeof(int));
			branches++;
			good_branches++;
		}
		vert_branches++;
	}
	print_array(r1,l1);
	print_array(r2,l2);
	free(r1);
	free(r2);
	return r;
}
int * brute_force(int * const X, int * const Y, const int m, const int n){
	int * r = malloc(m * n *sizeof(int));
	int * rp = r;
	int * xp, * yp;
	for(xp = X; xp < X + m; xp++ ) {
		for(yp = Y; yp < Y + n; yp++){
			*rp = *xp + *yp;
			rp++;
		}
	}
	merge_sort(r, m *n);
	return r;
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
	int n = 2;     // Set problem size
	int i, j; // indices for iteration
	
	unsigned int MAX_INT = 20;//(1<< 15);          // Set range of values in X, Y
	
	// PRINT PREAMBLE
	puts("Testing Sum XY Range Sort for X + Y");
	puts("==========================================");
	
	for(j= 0; j <1; j++) {
		n = n << 1;

		printf("N = %d\t MAX_INT = %d\t", n, MAX_INT);
		int * X = (int *)malloc(n*sizeof(int));
		int * Y = (int *)malloc(n*sizeof(int));
		int * Z, *Zbf;
		
		// POPULATE X, Y

		// for( i = 0; i < n; i++) {
		// 	*(X+i) = rand() % MAX_INT;
		// 	*(Y+i) = rand() % MAX_INT;
		// }
		
		// // SORT X, Y
		// merge_sort(X, n);
		// merge_sort(Y, n);
		cosnard(X,Y,n);
		
//		Print arrays
		puts("n\tX\tY");
		for( i = 0; i < n; i++){
			printf("%d\t%d\t%d\n",i, X[i], Y[i]);
		}

		clock_t start1, end1, start2, end2;
		double cpu_time_used1, cpu_time_used2;
		start1 = clock();
		Z = hh_sort(X, Y, n, n);
		end1 = clock();
		cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
	printf("Half n Half: %0.3f\t ", cpu_time_used1);
		

		start2 = clock();
		bf_merge_calls=0;
		Zbf = brute_force(X, Y, n, n);
		end2 = clock();
		cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
		printf("Brute Force: %0.3f bf_branch: %d\t",cpu_time_used2, bf_merge_calls);

		printf("Branch factor: %0.2f\t branches: %d/%d/%d\n",(good_branches*100.0)/branches, vert_branches, good_branches, branches);

		compare(Z, Zbf, n*n);
		free(X);
		free(Y);
		free(Z);
		free(Zbf);
		branches= 0;
		good_branches = 0;
		vert_branches = 0;
	}
	
	return 0;
}

