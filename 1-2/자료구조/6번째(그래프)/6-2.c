#include <stdio.h>
#include <stdlib.h>

#define MAXV 100     // 최대 정점 수
#define MAXE 1000    // 최대 간선 수

typedef struct Edge {
    int id;
    int to;
    struct Edge* next;
} Edge;

Edge* graph[MAXV];          // 인접 리스트
int n, e;                   // 정점 수, 간선 수
int visited[MAXV];          // 정점 방문 여부
int path[MAXE];             // 현재 경로 (간선 번호 저장)
int path_len = 0;           // 현재 경로 길이

// 경로를 저장하기 위한 구조체
typedef struct {
    int edges[MAXE];
    int length;
} Path;

Path all_paths[MAXE];
int path_count = 0;

// 간선 정렬용 비교 함수 (사전순 출력을 위해)
int cmp_path(const void* a, const void* b) {
    const Path* pa = (const Path*)a;
    const Path* pb = (const Path*)b;
    int min_len = pa->length < pb->length ? pa->length : pb->length;
    for (int i = 0; i < min_len; i++) {
        if (pa->edges[i] != pb->edges[i])
            return pa->edges[i] - pb->edges[i];
    }
    return pa->length - pb->length;
}

// 간선 추가 함수 (무방향 그래프)
void add_edge(int id, int from, int to) {
    Edge* e1 = (Edge*)malloc(sizeof(Edge));
    e1->id = id;
    e1->to = to;
    e1->next = graph[from];
    graph[from] = e1;

    Edge* e2 = (Edge*)malloc(sizeof(Edge));
    e2->id = id;
    e2->to = from;
    e2->next = graph[to];
    graph[to] = e2;
}

// DFS를 통한 경로 탐색
void dfs(int curr) {
    if (curr == n - 1) {
        // 도착점에 도달한 경우 경로 저장
        all_paths[path_count].length = path_len;
        for (int i = 0; i < path_len; i++) {
            all_paths[path_count].edges[i] = path[i];
        }
        path_count++;
        return;
    }

    visited[curr] = 1;

    // 인접 간선을 탐색 (사전순 출력을 위해 간선 정렬된 링크드리스트 전제로 사용)
    for (Edge* e = graph[curr]; e != NULL; e = e->next) {
        int next = e->to;
        if (!visited[next]) {
            path[path_len++] = e->id;
            dfs(next);
            path_len--;
        }
    }

    visited[curr] = 0;
}

int edge_cmp(const void* a, const void* b) {
    return ((Edge*)a)->id - ((Edge*)b)->id;
}

void sort_graph_edges() {
    for (int i = 0; i < n; i++) {
        // 간선 개수 세기
        int count = 0;
        for (Edge* e = graph[i]; e != NULL; e = e->next) {
            count++;
        }

        // 배열에 옮겨 정렬
        Edge** arr = (Edge**)malloc(sizeof(Edge*) * count);
        int idx = 0;
        for (Edge* e = graph[i]; e != NULL; e = e->next) {
            arr[idx++] = e;
        }

        qsort(arr, count, sizeof(Edge*), edge_cmp);

        // 다시 링크드리스트로 복원
        for (int j = 0; j < count - 1; j++) {
            arr[j]->next = arr[j + 1];
        }
        if (count > 0) {
            arr[count - 1]->next = NULL;
            graph[i] = arr[0];
        }

        free(arr);
    }
}

int main() {
    scanf("%d %d", &n, &e);

    for (int i = 0; i < e; i++) {
        int id, u, v;
        scanf("%d %d %d", &id, &u, &v);
        add_edge(id, u, v);
    }

    // 간선 정렬 (사전순 출력을 위해 각 정점의 인접 리스트 정렬)
    sort_graph_edges();

    // DFS 시작
    dfs(0);

    // 경로 정렬
    qsort(all_paths, path_count, sizeof(Path), cmp_path);

    // 출력
    for (int i = 0; i < path_count; i++) {
        for (int j = 0; j < all_paths[i].length; j++) {
            printf("%d", all_paths[i].edges[j]);
            if (j < all_paths[i].length - 1) printf(" ");
        }
        printf("\n");
    }

    return 0;
}

