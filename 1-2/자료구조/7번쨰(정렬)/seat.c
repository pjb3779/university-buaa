#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// 좌석 정보 구조체 정의
struct seat {
    char Id[200];     // 학번
    char name[200];   // 이름
    int seatId;       // 좌석 번호
    int when;         // 입력 순서 (초기 학번 정렬 순서를 유지하기 위함)
};

struct seat s[200];   // 최대 200명의 학생 데이터 저장 가능
int i, j, StuNum, maxseatId, k, n;

// 정렬용 함수 선언
int cmp(const void* p1, const void* p2);   // 좌석번호 + 입력순 정렬용
int cmp2(const void* p1, const void* p2);  // 학번 정렬용

int main()
{
    i = 1, k = 1;
    FILE* fp = fopen("in.txt", "r");     // 입력 파일
    FILE* fp2 = fopen("out.txt", "w");   // 출력 파일

    scanf("%d", &n);  // 학생 수 입력

    // 입력 파일에서 학생 정보 읽기
    for (i = 1; i <= n; i++) {
        fscanf(fp, "%s %s %d", s[i].Id, s[i].name, &s[i].seatId);
        s[i].when = i;  // 원래 입력 순서를 기억
    }

    StuNum = i;  // 전체 학생 수 +1 (배열 인덱스 1부터 사용 중)

    // 1. 좌석번호 기준 정렬 (같은 좌석번호면 입력순서로)
    qsort(s, StuNum, sizeof(struct seat), cmp);
	//시작 주소, 배열 개수, 사이즈, 비교 함수 포인터 

    // 현재 좌석번호 중 가장 큰 값과 학생 수 중 작은 값을 maxseatId로 설정
    maxseatId = (StuNum - 1 < s[StuNum - 1].seatId) ? StuNum - 1 : s[StuNum - 1].seatId;

    // 2. 누락된 좌석 번호 채우기
    k = n;  // 배열 끝에서부터 사용
    for (i = 1; i <= maxseatId; i++) {
        if (i == 1 && s[1].seatId > 1) {
            // 1번 좌석 누락 시 마지막 학생에게 할당
            s[k].seatId = 1;
            k--;
            continue;
        }
        if (s[i].seatId - s[i - 1].seatId > 1) {
            // 연속되지 않는 좌석 번호 발견 시 누락된 번호를 채움
            s[k].seatId = s[i - 1].seatId + 1;
            k--;
        }
    }

    // 3. 다시 좌석번호 기준 정렬 (변경된 좌석 적용 후)
    qsort(s, StuNum, sizeof(struct seat), cmp);

    // 현재 최대 좌석 번호부터 시작하여 중복 좌석을 정리
    k = maxseatId = s[StuNum - 1].seatId;
    for (i = 2; i <= n; i++) {
        if (s[i].seatId - s[i - 1].seatId == 0) {
            // 중복 좌석 발견 → 뒤에 있는 사람의 좌석 번호 증가
            k++;
            s[i].seatId = k;
        }
    }

    // 4. 학번 기준 정렬
    qsort(s, StuNum, sizeof(struct seat), cmp2);

    // 5. 결과 출력
    for (i = 1; i < StuNum; i++)
        fprintf(fp2, "%s %s %d\n", s[i].Id, s[i].name, s[i].seatId);

    return 0;
}

// 좌석 번호 기준 정렬 함수 (같으면 입력 순서 기준)
int cmp(const void* p1, const void* p2) {
    struct seat* a = (struct seat*)p1;
    struct seat* b = (struct seat*)p2;
    if (a->seatId != b->seatId)
        return a->seatId - b->seatId;
    else
        return a->when - b->when;  // 입력 순서 유지
}

// 학번 기준 정렬 함수
int cmp2(const void* p1, const void* p2) {
    struct seat* a = (struct seat*)p1;
    struct seat* b = (struct seat*)p2;
    return strcmp(a->Id, b->Id);
}

