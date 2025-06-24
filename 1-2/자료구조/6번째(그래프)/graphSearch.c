#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 100

int graph[MAX][MAX];     // 인접 행렬: 정점 간 연결 여부 및 간선 정보 저장
bool visited[MAX];       // DFS/BFS 탐색 시 각 정점 방문 여부를 저장
int queue[MAX];          // BFS에서 사용할 큐
int front, rear;         // 큐의 앞/뒤 인덱스

int n; // 정점 개수 (0 ~ n-1)

// 방문 배열 초기화 함수
void reset_visited() {
    for (int i = 0; i < n; i++)
        visited[i] = false;
}

// 깊이 우선 탐색(DFS) 함수
// v: 현재 정점, delete_v: 삭제할 정점
void dfs(int v, int delete_v) {
    if (v == delete_v) return; // 삭제된 정점은 방문하지 않음
    visited[v] = true;         // 현재 정점 방문 처리
    printf("%d ", v);          // 정점 출력

    // 인접 정점 순회 (번호가 작은 것부터)
    for (int i = 0; i < n; i++) {
        if (graph[v][i] && !visited[i] && i != delete_v) {
            dfs(i, delete_v);  // 재귀적으로 DFS 호출
        }
    }
}

// 너비 우선 탐색(BFS) 함수
void bfs(int start, int delete_v) {
    front = rear = 0;         // 큐 초기화
    queue[rear++] = start;    // 시작 정점을 큐에 삽입
    visited[start] = true;    // 시작 정점 방문 처리

    while (front < rear) {
        int v = queue[front++];  // 큐에서 꺼냄
        printf("%d ", v);        // 정점 출력

        // 인접 정점 순회
        for (int i = 0; i < n; i++) {
            if (graph[v][i] && !visited[i] && i != delete_v) {
                queue[rear++] = i;   // 인접 정점 큐에 삽입
                visited[i] = true;   // 방문 처리
            }
        }
    }
}

int main() {
    int m; // 간선 수
    scanf("%d %d", &n, &m);  // 정점 수 n, 간선 수 m 입력

    // 간선 정보 입력
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d %d", &u, &v);   // 정점 u와 v 연결
        graph[u][v] = 1;
        graph[v][u] = 1;          // 무방향 그래프이므로 양방향 연결
    }

    int delete_v;   // 삭제할 정점 번호
    scanf("%d", &delete_v);

    // === 삭제 전 ===
    reset_visited();       // 방문 배열 초기화
    dfs(0, -1);            // DFS 실행 (-1은 아무 정점도 삭제하지 않음)
    printf("\n");

    reset_visited();
    bfs(0, -1);            // BFS 실행
    printf("\n");

    // === 삭제 후 ===
    reset_visited();
    dfs(0, delete_v);      // DFS 실행 (delete_v 제외)
    printf("\n");

    reset_visited();
    bfs(0, delete_v);      // BFS 실행 (delete_v 제외)
    printf("\n");

    return 0;
}

