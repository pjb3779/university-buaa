#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_RECORDS 100
#define MAX_STUDENTS 100
#define ID_LEN 9
#define NAME_LEN 21

typedef struct {
    char id[ID_LEN];       // 8자리 학번 + 널종료
    char name[NAME_LEN];   // 최대 20자 + 널종료
    int total_time;        // 누적 사용 시간
} Student;

// 정렬용 비교 함수
int cmp(const void *a, const void *b) {
    const Student *A = a;
    const Student *B = b;
    // 사용 시간 내림차순
    if (A->total_time != B->total_time)
        return B->total_time - A->total_time;
    // 사용 시간이 같으면 학번 오름차순
    return strcmp(A->id, B->id);
}

int main() {
    int n;
    scanf("%d", &n);

    Student students[MAX_STUDENTS];
    int cnt = 0;

    for (int i = 0; i < n; i++) {
        char id[ID_LEN], name[NAME_LEN];
        int t;
        scanf("%s %s %d", id, name, &t);

        // 이미 등록된 학생인지 확인
        int found = 0;
        for (int j = 0; j < cnt; j++) {
            if (strcmp(students[j].id, id) == 0) {
                students[j].total_time += t;
                found = 1;
                break;
            }
        }
        // 새 학생이면 배열에 추가
        if (!found) {
            strcpy(students[cnt].id, id);
            strcpy(students[cnt].name, name);
            students[cnt].total_time = t;
            cnt++;
        }
    }

    // 정렬
    qsort(students, cnt, sizeof(Student), cmp);

    // 출력
    for (int i = 0; i < cnt; i++) {
        printf("%s %s %d\n",
               students[i].id,
               students[i].name,
               students[i].total_time);
    }

    return 0;
}

