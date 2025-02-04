#include <stdio.h>
#include <stdlib.h>

typedef struct cai{
	int v;
	int w;
}Cai;

double compare(double *a, double *b){
	return (*(double*)a > *(double*)b) - (*(double*)a < *(double*)b);
}

int main(){
	int n,k;
	scanf("%d %d", &n, &k);
	double ans = 0;
	Cai* cai = (Cai*)malloc((n+1)*sizeof(Cai));
	  
	for(int i = 0; i < n;i++){
		scanf("%d %d", &cai[i].v, &cai[i].w);
	}
	
	double* greed = (double*)malloc((n+1)*sizeof(double));
	
	for(int i = 0;i < n;i++){
		greed[i] = (double)cai[i].v / (double)cai[i].w;
	}
	
	qsort(greed, n,sizeof(double),compare);
	
	for(int i = 0;i < n;i++){
		printf("%0.3f\n",greed[i]);
	}
	free(cai);
    free(greed);
    
	return 0;
}
