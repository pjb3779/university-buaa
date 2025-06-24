#include <stdio.h>
#include <string.h>

typedef struct {
    int op;
    int pos;
    char str[512];
} Edit;

char text[512];  // 최종 텍스트
Edit history[512];
int his_top = 0;

void insert(int pos, const char* str) {
    int len = strlen(text);
    int str_len = strlen(str);
    
    if (len + str_len >= 512) {
        printf("삽입 배열 초과\n");
        return;
    }

    //문자 뒤로 밀기
    for (int i = len; i >= pos; i--) {
        text[i + str_len] = text[i];
    }

    for (int i = 0; i < str_len; i++) {
        text[pos + i] = str[i];
    }

    text[len + str_len] = '\0';
}

void remove_his(int pos, int n) {
    int len = strlen(text);
    
    if (pos < 0 || pos >= len) {
        printf("삭제 배열초과\n");
        return;
    }

    if (pos + n > len) {
        n = len - pos;
    }

    for (int i = pos; i + n < len; i++) {
        text[i] = text[i + n];
    }

    text[len - n] = '\0';
}

void undo() {
    if (his_top == 0) {
        //printf("스택 = 0\n");//문제 요구에 스택이 비엇을때는 아무것도 출력x 
        return;
    }

    Edit last_op = history[his_top - 1];
    if (last_op.op == 1) {
        remove_his(last_op.pos, strlen(last_op.str));
    } else if (last_op.op == 2) {
        insert(last_op.pos, last_op.str);
    }
    
    his_top--;
}



void add_history(int op, int pos, const char* str) {
    history[his_top].op = op;
    history[his_top].pos = pos;
    strcpy(history[his_top].str, str);
    his_top++;
}

int main() {
    fgets(text, 512, stdin);
    text[strcspn(text, "\n")] = '\0';  // 개행 문자 제거

    int n;
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        int op, pos;
        char str[512];
        scanf("%d %d %s", &op, &pos, str);

        add_history(op, pos, str);
    }
    getchar(); // 공백 문자 처리 습관화하깅 

    while (1) {
        int op, pos;
        char str[512];
        scanf("%d", &op);

        if (op == -1) {
            break;
        } else if (op == 1) {
            scanf("%d %s", &pos, str);
            insert(pos, str);
            add_history(1, pos, str);
        } else if (op == 2) {
            scanf("%d %d", &pos, &n);
            char temp[512];
            strncpy(temp, text + pos, n);//text에서 pos부터 n개를 복사 
            temp[n] = '\0';
            remove_his(pos, n);
            add_history(2, pos, temp);
        } else if (op == 3) {
            undo();
        }
    }

    printf("%s\n", text);

    return 0;
}

