#include <stdio.h>
#include <string.h>
#define NHASH 3001  // 해시 테이블 크기
#define MULT 37     // 해시 계산에 사용되는 곱셈 상수

// 연결 리스트 노드 정의
struct node {
    char word[21];      // 최대 길이 20의 단어 저장
    struct node* next;  // 다음 노드를 가리키는 포인터
};
typedef struct node* Nodeptr;
typedef struct node Node;

// 함수 선언
int find_first(char key[], char dictionary[][21], int n);        // 순차 탐색
int find_second(char key[], char dictionary[][21], int n);       // 이진 탐색
int find_third(char key[], char dictionary[][21], int table[]);  // 인덱스+이진 탐색
int find_fourth(char key[], char dictionary[][21], int n);       // 해시 탐색
unsigned int hash(char* str);                                    // 해시 함수
Nodeptr searchNode(Nodeptr head, char elem[]);                   // 해시에서 탐색
Nodeptr insertNode(Nodeptr head, char elem[]);                   // 해시 테이블에 삽입

int main() {
    int way;            // 탐색 방식 (1~4)
    char key[21];       // 찾을 단어
    char dictionary[3501][21];  // 사전 단어 배열
    FILE* in;
    in = fopen("dictionary3000.txt", "r");  // 파일 열기
    int i = 0, table[26], t = 0;
    char letter = 'a';

    // 단어를 파일에서 읽어오고 첫 글자별 시작 인덱스를 table[]에 저장
    while ((fscanf(in, "%s", dictionary[i]) != EOF)) {
        while (dictionary[i][0] >= letter) {
            letter++;
            table[t++] = i;
        }
        i++;
    }
    fclose(in);

    // 단어와 탐색 방식 입력
    scanf("%s%d", key, &way);

    // 탐색 방식에 따라 해당 함수 호출
    switch (way) {
    case 1:
        find_first(key, dictionary, i - 1);
        break;
    case 2:
        find_second(key, dictionary, i - 1);
        break;
    case 3:
        find_third(key, dictionary, table);
        break;
    case 4:
        find_fourth(key, dictionary, i);
        break;
    }
    return 0;
}

// 방식 1: 순차 탐색
int find_first(char key[], char dictionary[][21], int n) {
    int count = 0, flag = 0;

    while (count <= n) {
        if (strcmp(key, dictionary[count]) == 0) {  // 단어 일치
            flag = 1;
            printf("1 %d\n", count + 1);  // 성공, 비교 횟수 출력
            return 1;
        }
        if (strcmp(key, dictionary[count]) < 0)     // key보다 크면 탐색 종료
            break;
        count++;
    }

    // 못 찾았을 경우
    if (flag == 0) {
        printf("0 %d\n", count + 1);
    }
    return 0;
}

// 방식 2: 이진 탐색
int find_second(char key[], char dictionary[][21], int n) {
    int low = 0, high = n, mid;
    int count = 0, flag = 0;

    while (low <= high) {
        mid = (high + low) / 2;
        count++;
        if (strcmp(key, dictionary[mid]) == 0) {
            flag = 1;
            printf("1 %d\n", count);
            return 1;
        }
        if (strcmp(key, dictionary[mid]) > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (flag == 0) {
        printf("0 %d\n", count);
    }
    return 0;
}

// 방식 3: 인덱스를 이용한 이진 탐색
int find_third(char key[], char dictionary[][21], int table[]) {
    int a = key[0] - 'a';  // 첫 글자를 이용한 인덱스 계산
    int low = table[a], high = table[a + 1] - 1, mid;
    int count = 0, flag = 0;

    while (low <= high) {
        mid = (high + low) / 2;
        count++;
        if (strcmp(key, dictionary[mid]) == 0) {
            flag = 1;
            printf("1 %d\n", count);
            return 1;
        }
        if (strcmp(key, dictionary[mid]) > 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    if (flag == 0) {
        printf("0 %d\n", count);
    }
    return 0;
}

// 방식 4: 해시 탐색
int find_fourth(char key[], char dictionary[][21], int n) {
    Nodeptr table[NHASH];
    int i;

    // 해시 테이블 초기화
    for (i = 0; i < NHASH; i++)
        table[i] = NULL;

    // 단어를 해시 테이블에 삽입
    for (i = 0; i < n; i++) {
        int t = hash(dictionary[i]);
        table[t] = insertNode(table[t], dictionary[i]);
    }

    // 탐색 수행
    searchNode(table[hash(key)], key);
    return 0;
}

// 해시 함수 정의
unsigned int hash(char* str) {
    unsigned int h = 0;
    char* p;
    for (p = str; *p != '\0'; p++)
        h = MULT * h + *p;
    return h % NHASH;
}

// 연결 리스트에서 단어 탐색
Nodeptr searchNode(Nodeptr head, char elem[]) {
    Nodeptr p;
    int count = 1;

    for (p = head; p->next != NULL && strcmp(p->word, elem) < 0; p = p->next)
        count++;

    if (strcmp(p->word, elem) == 0)
        printf("1 %d", count);  // 찾았을 때
    else
        printf("0 %d", count);  // 못 찾았을 때
    return 0;
}

// 정렬된 연결 리스트에 단어 삽입
Nodeptr insertNode(Nodeptr head, char elem[]) {
    Nodeptr p, q, r;
    r = (Nodeptr)malloc(sizeof(Node));  // 새 노드 생성
    strcpy(r->word, elem);
    r->next = NULL;

    // 정렬된 위치 찾기
    for (p = head; p != NULL && strcmp(p->word, elem) < 0; q = p, p = p->next)
        ;

    if (p == head) {
        r->next = head;
        return r;  // 새 노드를 헤드로
    } else {
        q->next = r;
        r->next = p;
    }
    return head;
}

