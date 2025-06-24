#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 전역 변수
int a[109];  // 정렬할 배열
int A;       // 비교 횟수를 세는 변수

// 두 배열 요소를 교환하는 함수
void swap(int *v, int i, int j)
{
    int tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

// 정렬된 결과와 비교 횟수 출력
void output(int n)
{
    for (int i = 0; i < n; i++)
        printf("%d ", a[i]);
    printf("\n%d\n", A);  // 총 비교 횟수 출력
}

// 버블 정렬 (Bubble Sort)
void bubbSort(int *k, int n)
{
    int i, j, flag = 1, m = n;
    for (i = 0; flag == 1 && i < n - 1; i++, m--)
    {
        flag = 0;
        for (j = 0; j < m - 1; j++)
        {
            ++A;  // 비교 1회
            if (k[j] > k[j + 1])
            {
                // XOR를 이용한 스왑
                k[j + 1] = k[j + 1] ^ k[j];
                k[j] = k[j + 1] ^ k[j];
                k[j + 1] = k[j + 1] ^ k[j];
                flag = 1;
            }
        }
    }
}

// 힙을 조정하는 함수 (내림차순 정렬용 최대 힙 기준)
void adjust(int *k, int i, int n)
{
    int j, temp;
    temp = k[i];  // 현재 노드 저장
    j = 2 * i + 1;  // 왼쪽 자식 인덱스

    // 자식과 비교해가며 아래로 내려가는 과정
    while (j < n)
    {
        // 오른쪽 자식이 더 크면 오른쪽 자식 선택
        if (j < n - 1 && k[j] < k[j + 1])
            j++;
        ++A;  // 비교 1회
        if (temp >= k[j]) break;  // 조건 만족 시 종료

        // 자식을 부모로 올림
        k[(j - 1) / 2] = k[j];
        j = 2 * j + 1;
    }
    k[(j - 1) / 2] = temp;  // 저장된 노드 복원
}

// 힙 정렬 (Heap Sort)
void heapSort(int *k, int n)
{
    int i;

    // 최대 힙 구성
    for (i = n / 2 - 1; i >= 0; i--)
        adjust(k, i, n);

    // 루트와 끝 요소를 교환한 후 힙 재조정
    for (i = n - 1; i >= 1; i--)
    {
        // XOR 스왑
        k[i] = k[i] ^ k[0];
        k[0] = k[i] ^ k[0];
        k[i] = k[i] ^ k[0];
        adjust(k, 0, i);
    }
}

// 병합 함수 (Merge Sort 내부에서 두 정렬된 구간을 병합)
void merge(int *x, int *tmp, int left, int leftend, int rightend)
{
    int i = left, j = leftend + 1, q = left;

    while (i <= leftend && j <= rightend)
    {
        ++A;  // 비교 1회
        if (x[i] <= x[j])
            tmp[q++] = x[i++];
        else
            tmp[q++] = x[j++];
    }

    // 남은 요소 복사
    while (i <= leftend)
        tmp[q++] = x[i++];
    while (j <= rightend)
        tmp[q++] = x[j++];

    // tmp -> x로 복사
    for (i = left; i <= rightend; i++)
        x[i] = tmp[i];
}

// 병합 정렬 내부 재귀 함수
void mSort(int *k, int *tmp, int left, int right)
{
    int center;
    if (left < right)
    {
        center = (left + right) / 2;
        mSort(k, tmp, left, center);
        mSort(k, tmp, center + 1, right);
        merge(k, tmp, left, center, right);
    }
}

// 병합 정렬 전체 함수 (Merge Sort)
void mergeSort(int *k, int n)
{
    int *tmp = (int *)malloc(sizeof(int) * n);
    mSort(k, tmp, 0, n - 1);
    free(tmp);
}

// 퀵 정렬 함수 (Quick Sort)
void Qsort(int k[], int left, int right)
{
    int i, last;
    if (left < right)
    {
        last = left;
        for (i = left + 1; i <= right; i++)
        {
            ++A;  // 비교 1회
            if (k[i] < k[left])
                swap(k, ++last, i);
        }
        swap(k, left, last);  // 피벗 제자리 이동
        Qsort(k, left, last - 1);
        Qsort(k, last + 1, right);
    }
}

// 메인 함수
int main(void)
{
    int i, n, mode;

    // 입력: 정렬할 수의 개수, 정렬 방식
    scanf("%d%d", &n, &mode);
    for (i = 0; i < n; i++)
        scanf("%d", &a[i]);

    // 정렬 방식에 따라 정렬 수행
    switch (mode)
    {
    case 1: // 모드 1: 비교 횟수만 계산 (최대 비교 수로 강제 세팅)
        heapSort(a, n);
        A = n * (n - 1) / 2;
        break;
    case 2: // 버블 정렬
        bubbSort(a, n);
        break;
    case 3: // 힙 정렬
        heapSort(a, n);
        break;
    case 4: // 병합 정렬
        mergeSort(a, n);
        break;
    case 5: // 퀵 정렬
        Qsort(a, 0, n - 1);
        break;
    }

    // 결과 출력
    output(n);
}

