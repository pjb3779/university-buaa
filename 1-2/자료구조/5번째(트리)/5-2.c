#include <stdio.h>
#include <string.h>

int stack[101];        
int top;            

void init_stack(void){
	top = -1;
}

int push(int t){
	if (top >= 100) 
	{
		return -1;
	}
	stack[++top] = t;    
	return t;
}

int pop(void){
	if (top < 0)
	
	{
		return -1;
	}
	return stack[top--]; 
}

int is_operator(int s){
	return (s == '+' || s == '-' || s == '*' || s == '/');
}

int precedence(int op){
	if (op == '(')
		return 0;   
	if (op == '+' || op == '-')
		return 1;  
	if (op == '*' || op == '/')
		return 2;   
	else
		return 3; 
}

void postfix(char* pn, char* in){
	char c;
	init_stack();       
	while (*in){
		if (*in == '('){
			push(*in);
			in++;
		}
		else if (*in == ')'){
			while (stack[top] != '('){
				*pn++ = pop();      
				*pn++ = ' ';         
			}
			pop();        
			in++;       
		}
		else if (is_operator(*in)){
			while ((top >= 0) && precedence(stack[top]) >= precedence(*in)){
				*pn++ = pop();   
				*pn++ = ' ';
			}
			push(*in);    
			in++; 
		}
		else if (*in >= '0' && *in <= '9'){
			do
			{
				*pn++ = *in++;
			} while (*in >= '0' && *in <= '9');  
			*pn++ = ' ';               
		}
		else{
			in++;                 
		}
	}
	while (top >= 0){
		*pn++ = pop();   
		*pn++ = ' ';
	}
	--pn;    
	*pn = NULL;   
}

int calc(char* p){
	int i; 
	init_stack(); 
	while (*p){
		if (*p >= '0' && *p <= '9'){
			i = 0;   
			do{        
				i = i * 10 + *p - '0';          
				p++;
			} while (*p >= '0' && *p <= '9');  
			push(i); 
		}
		else if (*p == '+'){
			push(pop() + pop());
			p++;
		}
		else if (*p == '*'){
			push(pop() * pop());
			p++;
		}
		else if (*p == '-'){
			i = pop();      
			push(pop() - i);
			p++;
		}
		else if (*p == '/'){
			i = pop();
			push(pop() / i);  
			p++;
		}
		else{
			p++;
		}
	}
	return pop();     
}

int main(){
	int ans, i;   
	char in[256];  
	char pf[256];
	int cnt = 0;
	char c = 0;

	gets(in);   
	postfix(pf, in);   
	
	int len = strlen(pf);
	
	for (i = len; i >= 0; i--){
		if (is_operator(pf[i])){
			cnt++;
		}
		if (cnt == 3){
			c = pf[i];
			break;
		}
	}
	if (pf[len - 3] >= '0' && pf[len - 3] <= '9'){
		printf("%c", pf[len - 1]);
		printf(" %c ", pf[len - 5]);
		printf("%c", pf[len - 3]);
	}
	else if (is_operator(pf[len - 3])){
		printf("%c", pf[len - 1]);
		printf(" %c ", c);
		printf("%c", pf[len - 3]);
	}
	
	printf("\n");
	ans = calc(pf);  
	printf("%d\n", ans); 

	return 0;
}


