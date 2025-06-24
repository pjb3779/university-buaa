#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//1.트리 번호 입력받기
//2.트리 순회하며 리프 노드 저장(비행기)
//3.탑승구 번호 및 승객수 입력받기 
//4.승객수 정렬 및 리프 노드 저장한거랑 순서대로 출력 

// 트리 노드 구조체 정의
typedef struct TreeNode {
    int Id;                     // 노드 ID (탑승구 또는 분기점 번호)
    struct TreeNode* children[4];  // 자식 노드 포인터 배열 (최대 3개, 4개는 예외 처리용 여유)
} TreeNode, *Tree;

// 탑승구 유량 정보 구조체
struct PortData {
    int Id;     // 탑승구 번호
    int size;   // 유량
} ports[2000];  // 탑승구 최대 2000개까지 저장 가능

TreeNode nodes[2000];     // 노드들을 미리 배열로 선언
int leafNodes[1000];      // 리프 노드(탑승구)의 ID를 저장할 배열
int rootId, temp, childId;
int index, j, leafCnt = 0, treecnt = 0;  // leafCnt: 리프 노드 개수, treecnt: 트리 루트 개수 카운트

// 트리 순회 함수 (BFS 방식) → 리프 노드들을 leafNodes[]에 저장
void traverse(Tree node) {
    Tree queue[200], curNode;  // BFS용 큐 선언
    int front = -1, rear = 0;

    if (node != NULL) {
        queue[0] = node;  // 루트 노드를 큐에 넣음

        while (front < rear) {
            curNode = queue[++front];  // 큐에서 노드 하나 꺼냄
            int isLeaf = 1;

            // 자식 노드가 있는지 확인
            for (int i = 0; i < 4; i++) {
                if (curNode->children[i] != NULL) {
                    queue[++rear] = curNode->children[i];  // 자식 노드를 큐에 추가
                    isLeaf = 0;  // 자식이 있으면 리프가 아님
                }
            }

            if (isLeaf) {
                // 자식이 하나도 없다면 리프 노드
                leafNodes[leafCnt++] = curNode->Id;
            }
        }
    }
}

// 포트 유량 비교 함수 (정렬용)
// 유량 내림차순, 유량이 같으면 ID 오름차순
int compare(const void* a, const void* b) {
    struct PortData* portA = (struct PortData*)a;
    struct PortData* portB = (struct PortData*)b;

    if (portA->size != portB->size) {
        return portB->size - portA->size;  // 유량 내림차순
    } else {
        return portA->Id - portB->Id;      // ID 오름차순
    }
}

int main() {
    // 트리 구조 입력 받기
    while (1) {
        scanf("%d", &rootId);  // 루트 또는 부모 노드 ID 입력
        if (rootId == -1) break;  // -1 입력 시 종료

        treecnt++;  // 트리 개수 카운트
        nodes[rootId].Id = rootId;
        index = 0;  // 자식 인덱스 초기화

        // 자식 노드들 입력 받기
        while (1) {
            scanf("%d", &childId);
            if (childId == -1) break;  // -1로 종료

            nodes[rootId].children[index++] = &nodes[childId];  // 자식 노드를 포인터로 연결
            nodes[childId].Id = childId;  // 자식 노드 ID 설정
        }
    }

    // 트리 탐색 → 리프 노드 수집
    traverse(&nodes[100]);  // 루트 노드는 항상 100번으로 시작 (문제 조건)

    // 탑승구 유량 정보 입력
    for (int i = 0; i < leafCnt; i++) {
        scanf("%d %d", &ports[i].Id, &ports[i].size);
    }

    // 유량 기준으로 탑승구 정렬
    qsort(ports, leafCnt, sizeof(struct PortData), compare);

    // 매핑 결과 출력: 정렬된 탑승구 → 트리의 리프 노드 위치에 재배치
    for (int i = 0; i < leafCnt; i++) {
        printf("%d->%d\n", ports[i].Id, leafNodes[i]);
    }

    return 0;
}

