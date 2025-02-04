#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define Maxn 200010

int max(int a, int b){
	return (a > b) ? a : b;
}

void run(){
	int n;
	scanf("%d", &n);
	int w[Maxn]={0,};
	int dp[Maxn] = {0,};
	int dp[Maxn][2]={{0,}};
	
	for(int i = 1;i <= n;i++){
		scanf("%d", &w[i]);	
	}
	
	for(int i = 1;i <= n;i++){
		dp[i][0] = max(dp[i-1][0], dp[i-1][1]);
		dp[i][1] = w[i] + (i == 1 ? 0 : max(dp[i-2][0], dp[i-2][1]));	
	}
	printf("%d\n", max(dp[n][0],dp[n][1]));
}

int main(){
	int t;
	scanf("%d", &t);
	while(t--){
		run();
	}	
	return 0;
}
