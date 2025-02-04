#include <stdio.h>
#include <stdlib.h>
typedef long long ll;

typedef struct graph{
	int v;
	int **arr;
}graph;

void graphinit(graph* pGrp, int cnt){
	pGrp->v = cnt;
	
	pGrp->arr = (int**)calloc(cnt ,sizeof(int*));
	for(int i = 0; i < cnt; i++){
		pGrp -> arr[i] = (int*)calloc(cnt,sizeof(int));
	}
}

void display(graph* pGrp){
	int i,j;
	for(i = 0; i < pGrp->v; i++){
		for(j = 0; j < pGrp->v; j++){
			printf("%d", pGrp->arr[i][j]);
		}
	}
}

void connect(graph* pGrp,int src, int dst, int cost){
	pGrp->arr[src][dst] = cost;
	pGrp->arr[dst][src] = cost;
}

void memoryfree(graph *pGrp){
	int i;
	for(i = 0; i < pGrp->v; i++){
		free(pGrp->arr[i]);
	}
}
int main(){
	graph grp;
	int n,m,s,x,y;
	ll t,cost;
	scanf("%d %d %d %lld",&n ,&m, &s, &t);
	graphinit(grp, n);
	
	for(int i = 0; i < m; i++){
		scanf("%d %d %lld", &x, &y, &cost);
		connect(x,y,cost);
	}
	
	
	return 0;
}
