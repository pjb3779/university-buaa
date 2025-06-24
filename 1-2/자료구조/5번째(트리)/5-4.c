#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 트리 노드 구조체 정의
typedef struct node {
    char word[20];        // 단어 저장
    int num;              // 단어 빈도 수
    struct node* lchild;  // 왼쪽 자식
    struct node* rchild;  // 오른쪽 자식
} Tree;

// 전역 변수 선언
Tree* temp, * tempp, * root = NULL, * add = NULL;

// 함수 원형 선언
Tree* New(char w[]);          // 새 단어 삽입 함수
Tree* PTFT(Tree* root);       // 중위 순회 출력 함수

int main() {
    int i;
    char ch;
    FILE* fp;

    // 텍스트 파일 열기
    fp = fopen("article.txt", "r+");

    ch = fgetc(fp);  // 첫 글자 읽기
    while (ch != EOF) {
        char s[20] = { 0 };  // 하나의 단어 저장용

        // 현재 문자가 알파벳이면 단어 시작
        if ((ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A')) {
            if (ch <= 'Z' && ch >= 'A')  // 대문자 → 소문자
                ch = ch + 32;

            s[0] = ch;

            // 최대 20글자까지 단어 추출
            for (i = 1; i < 20; i++) {
                ch = fgetc(fp);

                // 대문자 처리
                if (ch <= 'Z' && ch >= 'A') {
                    ch = ch + 32;
                }

                // 알파벳이 아니면 단어 끝
                if ((ch >= 'z' || (ch < 'a' && ch > 'Z') || ch < 'A')) {
                    root = New(s);       // 단어 삽입
                    ungetc(ch, fp);      // 현재 문자 다시 되돌리기
                    break;
                }

                s[i] = ch;  // 단어 이어붙이기
            }
        }

        ch = fgetc(fp);  // 다음 문자 읽기
    }

    // 루트, 루트->right, 루트->right->right 출력
    while (tempp != NULL) {
        printf("%s ", tempp->word);
        tempp = tempp->rchild;
    }
    printf("\n");

    // 중위 순회 (사전순 출력)
    PTFT(root);

    fclose(fp);
    return 0;
}

// 단어 삽입 함수
Tree* New(char w[]) {
    int i;

    // 새 노드 동적 할당
    temp = (Tree*)malloc(sizeof(Tree));
    temp->lchild = temp->rchild = NULL;

    // 단어 배열 초기화
    for (i = 0; i < 20; i++) {
        temp->word[i] = 0;
    }

    strcpy(temp->word, w);  // 단어 복사

    // 트리가 비어있는 경우 → 루트 설정
    if (root == NULL) {
        temp->num = 1;
        root = tempp = temp;
    } else {
        temp->num = 1;

        // 삽입 위치 탐색
        for (i = 0; strlen(tempp->word) != 0; i++) {
            // 새 단어가 현재 노드보다 크고 오른쪽 자식이 없을 경우 → 삽입
            if (strcmp(temp->word, tempp->word) > 0 && tempp->rchild == NULL) {
                tempp = tempp->rchild = temp;
                break;
            }

            // 크고 오른쪽 자식이 존재 → 계속 탐색
            if (strcmp(temp->word, tempp->word) > 0 && tempp->rchild != NULL)
                tempp = tempp->rchild;

            // 같은 단어 → 빈도수 증가
            if (strcmp(temp->word, tempp->word) == 0) {
                tempp->num++;
                break;
            }

            // 작고 왼쪽 자식이 존재 → 계속 탐색
            if (strcmp(temp->word, tempp->word) < 0 && tempp->lchild != NULL)
                tempp = tempp->lchild;

            // 작고 왼쪽 자식이 없을 경우 → 삽입
            if (strcmp(temp->word, tempp->word) < 0 && tempp->lchild == NULL) {
                tempp = tempp->lchild = temp;
                break;
            }
        }

        // 다시 탐색 시작점 초기화
        tempp = root;
    }

    tempp = root;
    return root;
}

// 트리 중위 순회 함수 (사전순으로 단어 출력)
Tree* PTFT(Tree* root) {
    if (root == NULL) {
        return NULL;
    }

    PTFT(root->lchild);                          // 왼쪽 서브트리
    printf("%s %d\n", root->word, root->num);    // 현재 노드 출력
    PTFT(root->rchild);                          // 오른쪽 서브트리

    return NULL;
}

