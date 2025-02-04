#include <stdio.h>
#include <string.h>

void run(){
	char n[1001];
	int x,len;
	scanf("%s %d",n, &x);
	len = strlen(n);
	printf("%d\n", len);
	if(n[0] == '-'){
		printf("%c",n[0]);
		for(int i = 1; i < len ; i++){
			if(n[i] - '0' > x){
			printf("%c%d", n[i], x);
			}else{
				printf("%c",n[i]);
			}
		}
	}else{
		for(int i = 0; i <= len ; i++){
			if(n[i] - '0' > x){
			printf("%c%d", n[i], x);
			}else{
				printf("%c",n[i]);
			}
		}
	}

	printf("\n");
}

int main() {
	int t;
	scanf("%d", &t);
	while(t--){
		run();
	}
	return 0;
}
