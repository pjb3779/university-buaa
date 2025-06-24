#include <stdio.h>
#include <string.h>

typedef struct {
    char name[21];
    char called_funcs[100][21];
    int called_count; 
} Function;

Function functions[100]; // 함수 목록
int func_count = 0; // 함수 개수
char stack[200][21]; // 호출 스택
int stack_top = -1; // 스택의 최상위 인덱스

int get_func_index(char *name) {
    for (int i = 0; i < func_count; i++) {
        if (strcmp(functions[i].name, name) == 0) {
            return i; 
        }
    }
    
    strcpy(functions[func_count].name, name);
    functions[func_count].called_count = 0;
    return func_count++; // 새로 추가된 함수의 인덱스 반환
}

void process_input() {
    char operation[3], func_name[21];
    
    while (1) {
        scanf("%s", operation);
        
        if (strcmp(operation, "8") == 0) {
            scanf("%s", func_name);
            get_func_index(func_name);  // 현재 함수 인덱스
            
            if (stack_top >= 0) {
                int caller_func_index = get_func_index(stack[stack_top]);  // 호출한 함수의 인덱스
                // 현재 함수가 호출한 함수 목록에 없으면 추가
                int is_called = 0;
                for (int i = 0; i < functions[caller_func_index].called_count; i++) {
                    if (strcmp(functions[caller_func_index].called_funcs[i], func_name) == 0) {
                        is_called = 1;
                        break;
                    }
                }
                if (!is_called) {
                    strcpy(functions[caller_func_index].called_funcs[functions[caller_func_index].called_count], func_name);
                    functions[caller_func_index].called_count++;
                }
            }
            
            stack_top++;
            strcpy(stack[stack_top], func_name);
        } else if (strcmp(operation, "0") == 0) { 
            if (stack_top == -1) {
                break;
            }
            stack_top--;
        }
    }
}

void print_calls() {
    for (int i = 0; i < func_count; i++) {
        if (functions[i].called_count > 0) {  // 호출된 함수가 있으면 출력
            printf("%s:", functions[i].name);
            for (int j = 0; j < functions[i].called_count; j++) {
                if (j > 0) {
                    printf(",");
                }
                printf("%s", functions[i].called_funcs[j]);
            }
            printf("\n");
        }
    }
}

int main() {
    process_input();  // 사용자 입력 처리
    print_calls();    // 함수 호출 관계 출력
    return 0;
}

