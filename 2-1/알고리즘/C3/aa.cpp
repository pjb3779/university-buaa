#include <stdio.h>
#include <stdlib.h>

long long maxprice(int n, long long p[], long long dp[],int cut[]){
		for(int i = 1; i <= n; i++){
			long long q = -1;
			for(int j = 1; j <= i; j++){
				if(q < p[j] + dp[i - j]){
					q = p[j] + dp[i-j];
					cut[i] = j;
				}
			}
			dp[i] = q;
		}
		return dp[n];
}

void run()
{
	int n;
	scanf("%d", &n);
	long long *p = (long long*)malloc((n+1)*sizeof(long long));
	long long *dp = (long long*)malloc((n+1)*sizeof(long long));
	int *cut = (int*)malloc((n+1)*sizeof(int));
	
	for(int i = 1; i <= n;i++){
		scanf("%lld", &p[i]);
	}
	for(int i = 0; i <= n; i++){
		dp[i] = 0;
	}
	printf("%lld\n", maxprice(n,p,dp,cut));
	
	int *len = (int*)malloc((n+1)*sizeof(int));
	int k = 0;
	
	while(n > 0){
		len[k] = cut[n];
		n -= cut[n];
		k++;
	}
	printf("%d\n", k);
	
	for(int i = 0;i < k; i++){
		printf("%d ",len[i]);
	}
	
	free(p);
    free(dp);
    free(cut);
    free(len);
}
int main(){
	run();
	return 0;
}
