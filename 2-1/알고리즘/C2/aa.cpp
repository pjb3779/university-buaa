#include <stdio.h>
#include <stdlib.h>

int compare(const void *a,const void *b){
	return (*(int *)b - *(int *)a);
}
int main(){
	int t,n;
	int a[1000000001];
	scanf("%d",&t);
	while(t--){
		scanf("%d",&n);
		for(int i = 0;i < n;i++){
			scanf("%d",&a[i]);
		}
		qsort(a,n,sizeof(int),compare);
		printf("%d", a[0] + a[1]);
		 
	}
	return 0;
}
