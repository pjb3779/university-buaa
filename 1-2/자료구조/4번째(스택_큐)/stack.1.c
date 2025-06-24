#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int data[100];
    int top;
} Stack;

void initStack(Stack *stack) {
    stack->top = -1;
}

int push(Stack *stack, int value) {
    if (stack->top >= 99) {
        return 0; 
    }
    stack->data[++(stack->top)] = value;
    return 1; 
}

int pop(Stack *stack, int *data) {
    if (stack->top < 0) {
        return 0; 
    }
    *data = stack->data[(stack->top)--];
    return 1; 
}

void processCommands() {
    Stack stack;
    initStack(&stack);

    int op;
    while (scanf("%d", &op) != EOF) {
        if (op == -1) {
            break;
        } else if (op == 1) { 
            int value;
            scanf("%d", &value);
            if (!push(&stack, value)) {
                printf("error ");
            }
        } else if (op == 0) {
        	
            int data;
            if (pop(&stack, &data)) {
                printf("%d ", data);
            } else {
                printf("error ");
            }
        }
    }
}

int main() {
    processCommands();
    return 0;
}

