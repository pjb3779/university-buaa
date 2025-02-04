#include <stdio.h>

int ackmann(int m, int n){
	if(m == 0){
		return n+1;
	}else if(m > 0 && n == 0){
		return ackmann(m-1, 1);
	}else{
		return ackmann(m-1,ackmann(m, n-1));
	}	
}

int main(){
	int t,m,n;
	scanf("%d", &t);
	while(t--){
		scanf("%d %d", &m, &n);
		printf("%d\n", ackmann(m,n));
	}
	return 0;
}
