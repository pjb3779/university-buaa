#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char name[21];
    char phone[12];
} Entry;

Entry phonebook[101];
int count = 0;

int compare(const void *a, const void *b) {
    return strcmp(((Entry*)a)->name, ((Entry*)b)->name);
}

void add_entry(char *name, char *phone) {
	//이름이랑 전번 같으면 패스 
    for (int i = 0; i < count; i++) {
        if (strcmp(phonebook[i].name, name) == 0 && strcmp(phonebook[i].phone, phone) == 0) {
            return;
        }
    }
    
    // 이름이 같은 경우 번호 개수 확인하여 _1, _2 추가
    int index = 0;
    char modified_name[21];
    strcpy(modified_name, name);
    
    for (int i = 0; i < count; i++) {
        if (strcmp(phonebook[i].name, modified_name) == 0) {
            index++;
            snprintf(modified_name, 21, "%s_%d", name, index);
        }
    }
    
    // 새 항목 추가
    strcpy(phonebook[count].name, modified_name);
    strcpy(phonebook[count].phone, phone);
    count++;
}

int main() {
    int n;
    scanf("%d", &n);
    
    for (int i = 0; i < n; i++) {
        char name[21];
        char phone[12];
        scanf("%s %s", name, phone);
        add_entry(name, phone);
    }
    
    qsort(phonebook, count, sizeof(Entry), compare);
    
    for (int i = 0; i < count; i++) {
        printf("%s %s\n", phonebook[i].name, phonebook[i].phone);
    }
    
    return 0;
}

//strcpy(dest, src) 복사
//strcmp(str1, str2) 문자열 비교 대소문자도 구분 
/*
int compare(const void *a, const void *b) { 정수 오름차순 정렬 
    return (*(int*)a - *(int*)b);
}

// 비교 함수 (문자열 오름차순 정렬)
int compare(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}
qsort(names, n, sizeof(char *), compare);문자열은 포인터 배열이므로 char **

// 비교 함수 (나이 오름차순 정렬)
int compare(const void *a, const void *b) {
    return ((Person*)a)->age - ((Person*)b)->age;
}
*/
