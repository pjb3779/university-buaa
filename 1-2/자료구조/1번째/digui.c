#include <stdio.h>
#include <stdbool.h>

bool visited[11];
int ans[11];
int n;

void run(int level){
	if(level == n){
		for(int i = 0; i < n; i++){
			printf("%d ", ans[i]);
		}
		printf("\n");
		return;
	}
	for(int i = 1; i <= n; i++){
		if(!visited[i]){
			visited[i] = true;
			ans[level] = i;
			run(level + 1);
			visited[i] = false;
		}
	}
}

int main(){
	scanf("%d", &n);
	run(0);
	return 0;
}

