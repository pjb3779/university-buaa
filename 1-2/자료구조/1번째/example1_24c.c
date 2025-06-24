#include <stdio.h>

int calculate(int,char,int);
int getAns();

int main(){
	printf("%d",getAns());
	return 0;
}

int calculate(int v1,char op,int v2){
	switch(op){
		case '+':
			return v1+v2;
		case '-':
			return v1-v2;
		case '*':
			return v1*v2;
		case '/':
			return v1/v2;
	}
}

int getAns(){
	char c;
	int v1 = 0,v2 = 1, buf = 0;
	char op1 = '+',op2 = '*';
	while( op1 != '='){
		c=getchar();
		if(c==' ')continue;
		if(isdigit(c)){
			buf = 10 * buf + c - '0';
			continue;
		}
		if(c=='*'||c=='/'){
			v2 = calculate(v2, op2,buf);
			op2 = c;
		}
		if(c=='+'||c=='-'||c=='='){
			buf = calculate(v2,op2,buf);//전 op 연산 = 일떄 마무리후 답출력 
			v1 = calculate(v1,op1,buf);
			op1 = c;
			v2 = 1;
			op2 = '*';
		}
		buf = 0;
	}
	return v1;
}



