#include <stdio.h>
#include <string.h>

void run(char *n,int x){
	int len = strlen(n);
	int flag = 0;
	if(n[0] == '-'){
		printf("%c", n[0]);
		for(int i = 1; i < len; i++){
			if(n[i] - '0' >  x && flag == 0){
				flag = 1;
				printf("%d%c", x,n[i]);
			}
			else{
				printf("%c", n[i]);
			}
		}
		if(flag == 0)
		printf("%d",x);
		printf("\n");
	}else{
		for(int i = 0; i < len; i++){
			if(n[i] - '0' <  x && flag == 0){
				flag = 1;
				printf("%d%c", x,n[i]);
			}
			else{
				printf("%c", n[i]);
			}
		}
		if(flag == 0)
		printf("%d",x);
		printf("\n");
	}
}

int main(){
	int t,x;
	char n[1002];
	
	scanf("%d", &t);
	while(t--){
		scanf("%s %d", n, &x);
		run(n,x);
	}
	return 0;
}
