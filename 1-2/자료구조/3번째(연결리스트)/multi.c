#include <stdio.h>
#include <stdlib.h>

typedef struct Xiang {
    int exp;//게수(앞에 붙은 수)
    int fang;//지수(제곱) 
    struct Xiang* next;
} Xiang;

Xiang* createXiang(int exp, int fang) {
    Xiang* newXiang = (Xiang*)malloc(sizeof(Xiang));
    newXiang->exp = exp;
    newXiang->fang = fang;
    newXiang->next = NULL;
    return newXiang;
}

// (지수 내림차순으로 정렬 + 같은 지수끼리 합치기)
void insertXiang(Xiang** shi, int exp, int fang) {
    if (exp == 0) return;

    Xiang *prev = NULL, *curr = *shi;

    // 정렬된 위치에서 알맞은 팡으로 이동 
    while (curr != NULL && curr->fang > fang) {
        prev = curr;
        curr = curr->next;
    }

	// 같은 지수가 이미 있다면 계수만 더함
    if (curr != NULL && curr->fang == fang) {
        curr->exp += exp;
        if (curr->exp == 0) { 
            if (prev) prev->next = curr->next;
            else *shi = curr->next;
            free(curr);
        }
    } else {
    	 // 새로운 항 삽입
        Xiang* newXiang = createXiang(exp, fang);
        if (prev == NULL) {// 맨 앞에 삽입 shi이 head인경우 
            newXiang->next = *shi;
            *shi = newXiang;
        } else {// 중간 끝에 삽입
            newXiang->next = curr;
            prev->next = newXiang;
        }
    }
}

void printShi(Xiang* shi) {
    while (shi != NULL) {
        printf("%d %d ", shi->exp, shi->fang);
        shi = shi->next;
    }
    printf("\n");
}

void freeShi(Xiang* shi) {
    while (shi) {
        Xiang* temp = shi;
        shi = shi->next;
        free(temp);
    }
}

Xiang* multiplyShi(Xiang* shi1, Xiang* shi2) {
    Xiang* result = NULL;

    for (Xiang* p1 = shi1; p1 != NULL; p1 = p1->next) {
        for (Xiang* p2 = shi2; p2 != NULL; p2 = p2->next) {
            int newExp = p1->exp * p2->exp;
            int newFang = p1->fang + p2->fang;
            insertXiang(&result, newExp, newFang);
        }
    }

    return result;
}

Xiang* readShi() {
    Xiang* shi = NULL;
    int exp, fang;
    while (scanf("%d %d", &exp, &fang) == 2) {
        insertXiang(&shi, exp, fang);
        if (getchar() == '\n') break;
    }
    return shi;
}

int main() {
    Xiang* shi1 = readShi();
    Xiang* shi2 = readShi();

    Xiang* result = multiplyShi(shi1, shi2);
    printShi(result);

    freeShi(shi1);
    freeShi(shi2);
    freeShi(result);

    return 0;
}

