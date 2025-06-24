#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    double stack[1000];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

void push(Stack *s, double value) {
    if (s->top < 999) {
        s->stack[++(s->top)] = value;
    } else {
        printf("스택 초과\n");
    }
}

double pop(Stack *s) {
    if (!isEmpty(s)) {
        return s->stack[(s->top)--];
    } else {
        printf("스택 언더 초과\n");
        return -1;
    }
}

double peek(Stack *s) {
    if (!isEmpty(s)) {
        return s->stack[s->top];
    }
    return -1;
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

//전위 표기법 계산
double evaluatePrefix(char *prefix) {
    Stack s;
    initStack(&s);

    // 전위식을 토큰으로 분리
    char *tokens[500];
    int count = 0;
    char exprCopy[1000];
    strcpy(exprCopy, prefix);  // strtok 파괴 방지

    char *tok = strtok(exprCopy, " ");
    while (tok != NULL) {
        tokens[count++] = tok;
        tok = strtok(NULL, " ");
    }

    // 오른쪽에서 왼쪽으로 처리
    for (int i = count - 1; i >= 0; i--) {
        if (isdigit(tokens[i][0]) || (tokens[i][0] == '-' && isdigit(tokens[i][1]))) {
            push(&s, atof(tokens[i]));
        } else {
            double a = pop(&s);
            double b = pop(&s);
            double result;
            switch (tokens[i][0]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = a / b; break;
                default: result = 0; break;
            }
            push(&s, result);
        }
    }

    return pop(&s);
}
 

// 후위 표기법 계산
double evaluatePostfix(char *postfix) {
    Stack s;
    initStack(&s);
    char token[20];
    int i = 0, j = 0;

    while (postfix[i] != '\0') {
        if (isdigit(postfix[i])) {// 음수 추가 || (postfix[i] == '-' && isdigit(postfix[i + 1])) 
            // 숫자 추출
            j = 0;
            while (isdigit(postfix[i]) || postfix[i] == '.') {// 소수도처리 
                token[j++] = postfix[i++];
            }
            token[j] = '\0';
            push(&s, atof(token));
        } else if (postfix[i] == ' ') {
            i++;
        } else {
            // 연산자 처리
            double b = pop(&s);
            double a = pop(&s);
            double result;
            switch (postfix[i]) {
                case '+': result = a + b; break;
                case '-': result = a - b; break;
                case '*': result = a * b; break;
                case '/': result = a / b; break;
                default: result = 0; break;
            }
            push(&s, result);
            i++;
        }
    }

    return pop(&s);
}

// 중위 표기법을 후위 표기법으로 변환
void infixToPostfix(char *infix, char *postfix) {
    Stack opStack;
    initStack(&opStack);//주소를 보내야함 함수에서 문자열사용과 같은 느낌 
    int i = 0;
    char token[20];

    while (infix[i] != '=' && infix[i] != '\0') {
        if (isdigit(infix[i])) {// 음수처리 || (infix[i] == '-' && isdigit(infix[i + 1])) 
            // 숫자 추출
            int k = 0;
            while (isdigit(infix[i]) || infix[i] == '.') {//숫자처리 소수포함 
                token[k++] = infix[i++];
            }
            token[k] = '\0';
            strcat(postfix, token);//숫자 복사 
            strcat(postfix, " ");
        } else if (infix[i] == ' ') {// 공백처리 
            i++;
        } else if (infix[i] == '(') {// 괄호는 연산자 스택에 추가 
            push(&opStack, infix[i]);
            i++;
        } else if (infix[i] == ')') {
            while (!isEmpty(&opStack) && peek(&opStack) != '(') {
                strncat(postfix, (char[]) {pop(&opStack)}, 1);
                strcat(postfix, " ");
            }
            pop(&opStack); // '(' 제거
            i++;
        } else {
            // 연산자 처리 
            while (!isEmpty(&opStack) && precedence(peek(&opStack)) >= precedence(infix[i])) {// 연산자 순위가 높으면 추가 
                strncat(postfix, (char[]) {pop(&opStack)}, 1);
                strcat(postfix, " ");
            }
            push(&opStack, infix[i]);//현재의 연산자 스택에 추가 
            i++;
        }
    }

    // 남아있는 연산자 처리
    while (!isEmpty(&opStack)) {
        strncat(postfix, (char[]) {pop(&opStack)}, 1);
        strcat(postfix, " ");
    }
}

// 중위식을 전위식으로 변환
void infixToPrefix(char *infix, char *prefix) {
    Stack opStack;
    initStack(&opStack);

    char output[1000] = "";
    char token[20];
    int i = strlen(infix) - 1;  // 오른쪽에서 왼쪽으로 처리
    int j = 0;

    while (i >= 0) {
        if (isspace(infix[i])) {
            i--;
            continue;
        }
        
        if (infix[i] == '=') { // ? = 기호 무시
	        i--;
	        continue;
	    }
        

        if (isdigit(infix[i])) {
            int k = 0;
            // 숫자 추출 (역방향)
            while (i >= 0 && (isdigit(infix[i]) || infix[i] == '.')) {
                token[k++] = infix[i--];
            }
            token[k] = '\0';
            // 숫자 반전
            for (int m = k - 1; m >= 0; m--) {
                output[j++] = token[m];
            }
            output[j++] = ' ';
        }
        else if (infix[i] == ')') {
            push(&opStack, infix[i]);
            i--;
        }
        else if (infix[i] == '(') {
            while (!isEmpty(&opStack) && peek(&opStack) != ')') {
                output[j++] = pop(&opStack);
                output[j++] = ' ';
            }
            pop(&opStack); // ')' 제거
            i--;
        }
        else {  // 연산자
            while (!isEmpty(&opStack) &&
                   precedence(infix[i]) < precedence(peek(&opStack))) {
                output[j++] = pop(&opStack);
                output[j++] = ' ';
            }
            push(&opStack, infix[i]);
            i--;
        }
    }

    while (!isEmpty(&opStack)) {
        output[j++] = pop(&opStack);
        output[j++] = ' ';
    }
    output[j] = '\0';

    // 결과를 단어 단위로 역순
    char *tokens[500];
    int count = 0;
    char *tok = strtok(output, " ");
    while (tok != NULL) {
        tokens[count++] = tok;
        tok = strtok(NULL, " ");
    }

    prefix[0] = '\0';
    for (int k = count - 1; k >= 0; k--) {
        strcat(prefix, tokens[k]);
        strcat(prefix, " ");
    }
}




int main() {
    char infix[1000], postfix[1000] = "", prefix[1000] = "";

    fgets(infix, sizeof(infix), stdin);// 공백 처리를 위해 fgets 사용 

	infixToPrefix(infix, prefix);
    printf("전위 표기식: %s\n", prefix);
    
    // 후위 표기법으로 변환
    infixToPostfix(infix, postfix);
    printf("후위 표기법: %s\n", postfix);

    double result = evaluatePostfix(postfix);
    double res = evaluatePrefix(prefix);
    
    printf("%.2lf\n", res);
	printf("%.2lf\n", result);

    return 0;
}

