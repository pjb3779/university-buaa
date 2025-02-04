#include <stdio.h>
#include <stdlib.h> 

void run(){
	int n,m;
	scanf("%d %d", &n, &m);
	
	int* w = (int*)malloc(n*sizeof(int));
	int* v = (int*)malloc(n*sizeof(int));
	
	int *dp = (int*)malloc((m+1)*sizeof(int));
	
	for (int i = 0; i <= m; i++) {
        dp[i] = 0;
    }
	
	for(int i = 0;i < n;i++){
		scanf("%d %d",&w[i], &v[i]);
	}
	
	for(int i = 0; i < n;i++){
		for(int j = m; j >= v[i]; j--){
			dp[j] = dp[j] > (dp[j - v[i]] + w[i]) ? dp[j] : (dp[j - v[i]] + w[i]);	
		}
	}
	printf("%d\n", dp[m]);
	free(w);
	free(v);
	free(dp);
}

int main(){
	int t;
	scanf("%d", &t);
	while(t--){
		run();
	}
}
