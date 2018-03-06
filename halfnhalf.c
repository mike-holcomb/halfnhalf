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

long branches = 0;
long good_branches = 0;
long vert_branches = 0;
long merge_count = 0;
double avg_merge_size = 0.0;
long thin_cells = 0;
int k = 0;
unsigned int MAX_INT = (1<< 15); 

void insert_sort(int * const x, long n){
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

void print_arrays(int * const arr, int * const arr2, const int m, const int n){
	int * arrp;
	for(arrp = arr; arrp < arr+m; arrp++)
		printf("%d ", *arrp);
	printf(" || ");
	for(arrp = arr2; arrp < arr2+n; arrp++)
		printf("%d ", *arrp);
	puts("");
}

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
	// if(*Y < *(X+m-1)){
	// printf("%d, %d , %x, %x    ",m,n, X, Y);
	// print_arrays(X,Y,m,n);
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
	// } else {
	// 	memcpy(X+m,Y,n);
	// }

	merge_count++;
	avg_merge_size += (m+n-avg_merge_size)/(merge_count);
}

void merge_sort(int * const X, const long n){
	if( n < 2) // Base Case
		return;
	
	const long m = n/2; // Find Mid Point
	// printf("%ld %ld  %x    %x\n",m,n-m, X, X+m);

	merge_sort(X, m);
	merge_sort(X+m, n-m);
	merge(X, X+m, m, n-m);
}
void k_merge_sort(int * const X, const long n){
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
		gen_z(X, Y, Z, 1,n);
		thin_cells++;
		return;
	} 

	if(n == 1) {
		gen_z(Y,X,Z,1,m);
		thin_cells++;
		return;
	}

	int m2 = m >> 1;
	int n2 = n >> 1;

	int diff_horz = (POINT_SUM(X,Y,m2-1,n-1)) - (POINT_SUM(X,Y,m2,0));
	int diff_vert = (POINT_SUM(X,Y,m-1,n2-1)) - (POINT_SUM(X,Y,0,n2));
	int l1,l2;

	if (diff_horz < diff_vert) {
		hh_sort(X, Y, Z, m2, n);
		l1 = m2*n;
		hh_sort(X+m2, Y, Z+l1, m - m2,n);
		l2 = (m-m2)*n;
		if (diff_horz > 0) {
			merge(Z, Z+l1, l1, l2);
		}
		branches++;
	}
	else {
		hh_sort(X, Y, Z, m, n2);
		l1 = m * n2;
		hh_sort(X, Y+n2, Z+l1, m,n-n2);
		l2 = (n-n2)*m;

		if (diff_vert > 0) {
			merge(Z, Z+l1, l1, l2);
		}
		branches++;
		vert_branches++;
	}

	int m2 = m >> 1;
	int n2 = n >> 1;

	int diff_horz = (POINT_SUM(X,Y,m2-1,n-1)) - (POINT_SUM(X,Y,m2,0));
	int diff_vert = (POINT_SUM(X,Y,m-1,n2-1)) - (POINT_SUM(X,Y,0,n2));
	int l1,l2;

	if (diff_horz < diff_vert) {
		hh_sort(X, Y, Z, m2, n);
		l1 = m2*n;
		hh_sort(X+m2, Y, Z+l1, m - m2,n);
		l2 = (m-m2)*n;
		if (diff_horz > 0) {
			merge(Z, Z+l1, l1, l2);
		}
		branches++;
	}
	else {
		hh_sort(X, Y, Z, m, n2);
		l1 = m * n2;
		hh_sort(X, Y+n2, Z+l1, m,n-n2);
		l2 = (n-n2)*m;

		if (diff_vert > 0) {
			merge(Z, Z+l1, l1, l2);
		}
		branches++;
		vert_branches++;
	}
}

void bass_ackwards(int * const X, int * const Y, const int m, const int n ){
	// ASSUMES X < Y ON AVERAGE
	int i = m-1;
	int x_ins, y_ins;
	while(*(X+i) > *Y && i >= 0){
		i--;
	}
	x_ins = i;
	i = 0;
	while(*(Y+i) < *(X+m-1) && i < n) {
		i++;
	}
	y_ins = i;
	insert_sort(Y-x_ins,x_ins+y_ins+1);
}

void hh_sort2(int * const X, int * const Y, int * const Z, const int m, const int n) {
	if(m == 1) {
		gen_z(X, Y, Z, 1,n);
		//thin_sort(*X, Y, Z, n);
		thin_cells++;
		return;
	} 

	if(n == 1) {
		gen_z(Y,X,Z,1,m);
		//thin_sort(*Y, X, Z, m);
		thin_cells++;
		return;
	}

	// if(m ==2){
	// 	gen_z(X,Y,Z,m,n);
	// 	merge(Z,Z+n,n,n);
	// 	thin_cells++;
	// 	return;
	// }
	// if( n == 2){
	// 	gen_z(Y,X,Z,n,m);
	// 	merge(Z,Z+m,m,m);
	// 	thin_cells++;
	// 	return;
	// }
	// if( m * n < k){
	// 	gen_z(Y,X,Z,n,m);
	// 	insert_sort(Z,n*m);
	// 	thin_cells++;
	// 	return;
	// }

	int i;
	int diff;
	int min_diffx = INT_MAX;
	int min_posx=1;
	for(i = 1; i < m; i++) {
		diff = (POINT_SUM(X,Y,i-1,n-1)) - (POINT_SUM(X,Y,i,0));
		if ( diff <=0 ) {
			//printf("Horz  Split %d %d\n",i,n );
			hh_sort2(X, Y, Z, i, n);
			hh_sort2(X+i, Y, Z+i*n, m-i,n);
		    // printf("X %d  ",i);
		    // print_array(Z,m*n);
			return;
		}
		if ( diff < min_diffx) {
			min_diffx = diff;
			min_posx = i;
		}
	}

	int min_diffy = INT_MAX;
	int min_posy=1;
	for(i = 1; i < n; i++) {
		diff = (POINT_SUM(X,Y,m-1,i-1)) - (POINT_SUM(X,Y,0,i));
		if ( diff <=0 ) {
			//printf("Vert  Split %d %d\n",m,i );
			hh_sort2(X, Y, Z, m, i);
			hh_sort2(X, Y+i, Z+i*m, m,n-i);
			// printf("Y %d  ",i);
			// print_array(Z,m*n);
			return;
		}
		if ( diff < min_diffy) {
			min_diffy = diff;
			min_posy = i;
		}
	}

	if(min_diffx < min_diffy){
		//printf("Sub Horz  Split %d %d\n",min_posx,n );
		hh_sort2(X, Y, Z, min_posx, n);
		hh_sort2(X+min_posx, Y, Z+min_posx*n, m-min_posx,n);
		merge(Z, Z+(min_posx)*n, (min_posx)*n, (m-min_posx)*n);
	} else {
		//printf("Sub Vert  Split %d %d\n",m,min_posy );
		hh_sort2(X, Y, Z, m, min_posy);
		hh_sort2(X, Y+min_posy, Z+m*min_posy, m, n-min_posy);
		merge(Z, Z+(min_posy)*m, (min_posy)*m, (n-min_posy)*m);
	}
}

void brute_force(int * const X, int * const Y, int * const Z, const int m, const int n){
	merge_sort(Z, m*n);
}

int compare(int * const Z, int * const Zbf, const int n){
	int i;
	for(i = 0; i < n; i++){
		if(*(Z+i) != *(Zbf+i))
			printf("%d: %d not equal %d\n", i, *(Z+i), *(Zbf+i));
	}
	return 0;
}

void normal(int * const X,int * const  Y, const int n) {
		int i;
		for( i = 0; i < n; i++) {
			*(X+i) = rand() % MAX_INT + ((rand()% MAX_INT)<<14);
			*(Y+i) = rand() % MAX_INT + ((rand()% MAX_INT)<<14);
		}
		merge_sort(X, n);
		merge_sort(Y, n);
}

void constant_alphabeta(int * const X, int * const Y, const int n, int alpha, int beta) {
	int i;
	*X = alpha;
	*Y = beta;
	for( i = 1; i < n; i++) {
		*(X+i) = (rand() % (2*alpha)) + *(X+i-1);
		*(Y+i) = (rand() % (2*beta)) + *(Y+i-1);
	}
}

void double_normal(int * const X,int * const  Y, const int n){
	int i;
	for( i = 0; i < n; i++) {
		*(X+i) = rand() % MAX_INT + rand()% MAX_INT;
	}
	merge_sort(X, n);
	memcpy(Y,X,n*sizeof(int));
}

int main(int argc, char *argv[]) {
	srand(time(NULL)); // SEED RANDOM
	int n = 4000;     // Set problem size
	int i, j; // indices for iteration
	int a = 0;
	
         // Set range of values in X, Y
	
	// PRINT PREAMBLE
	puts("Testing Sum XY Range Sort for X + Y");
	puts("==========================================");
	
	for(j= 0; j < 19; j++) {
		//n = n*2;
		k = 2;
		a+=200;

		printf("N = %d ", n);
		int * X = (int *)malloc(n*sizeof(int));
		int * Y = (int *)malloc(n*sizeof(int));
		int * Z = (int *)malloc(n*n*sizeof(int));
		int  *Zbf = (int *)malloc(n*n*sizeof(int));
		
		// POPULATE X, Y
	
		// SORT X, Y

		// cosnard(X,Y,n);
		// ideal(X,Y,n);
		// normal(X,Y,n);
		// double_normal(X,Y,n);
		constant_alphabeta(X, Y, n, a, n-a);

		int alpha = *(X+n-1) - *X;
		int beta  = *(Y+n-1) - *Y;
		double avg_alpha = (double)alpha / (n-1);
		double avg_beta  = (double)beta / (n-1);
		double pigeonhole = avg_alpha * avg_beta;

		//Print arrays
		// puts("n\tX\tY");
		// for( i = 0; i < n; i++){
		// 	printf("%d\t%d\t%d\n",i, X[i], Y[i]);
		// }

		clock_t start1, end1, start2, end2;
		double cpu_time_used1, cpu_time_used2;
		merge_count=0;
		avg_merge_size=0;
		start1 = clock();
		hh_sort(X, Y, Z, n, n);
		end1 = clock();
		cpu_time_used1 = ((double) (end1 - start1)) / CLOCKS_PER_SEC;
	printf("HH: %0.3f merge: %ld (%0.1f)\t", cpu_time_used1, merge_count, avg_merge_size);
		
		merge_count=0;
		avg_merge_size=0;
		start2 = clock();
		gen_z(X, Y, Zbf, n, n);
		// puts("");
		// print_array(Zbf,n*n);
		brute_force(X, Y, Zbf, n, n);
		end2 = clock();
		cpu_time_used2 = ((double) (end2 - start2)) / CLOCKS_PER_SEC;
		printf("Brute Force: %0.3f merge: %ld (%0.1f)\t",cpu_time_used2, merge_count, avg_merge_size);

		//printf("Branch factor: %0.2f\t branches: %d/%d/%d\n",(good_branches*100.0)/branches, vert_branches, good_branches, branches);
		printf("thin_cells: %ld (avg: %0.1f), avg alpha %0.1f, beta %0.1f, pigeonhole: %0.1f\n",thin_cells,(n*n)/(double)thin_cells, avg_alpha, avg_beta, pigeonhole);

        compare(Z, Zbf, n*n);
		// print_array(Z,n*n);
		//print_array(Zbf,n*n);
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

