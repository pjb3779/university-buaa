#include <stdio.h>
#include <stdlib.h>

#define MAX 1000

typedef struct {
    int id, u, v, weight;
} Edge;

int parent[MAX];
Edge edges[MAX];
int result[MAX];
int n, e;

// 유니온 파인드용
int find(int x) {
    return x == parent[x] ? x : (parent[x] = find(parent[x]));
}

void unite(int a, int b) {
    parent[find(a)] = find(b);
}

int cmp(const void* a, const void* b) {
    return ((Edge*)a)->weight - ((Edge*)b)->weight;
}

int cmp_id(const void* a, const void* b) {
    return *(int*)a - *(int*)b;
}

int main() {
    scanf("%d %d", &n, &e);

    for (int i = 0; i < e; i++) {
        scanf("%d %d %d %d", &edges[i].id, &edges[i].u, &edges[i].v, &edges[i].weight);
    }

    qsort(edges, e, sizeof(Edge), cmp);

    for (int i = 0; i < n; i++) parent[i] = i;

    int total = 0, count = 0;
    for (int i = 0; i < e && count < n - 1; i++) {
        int u = edges[i].u, v = edges[i].v;
        if (find(u) != find(v)) {
            unite(u, v);
            total += edges[i].weight;
            result[count++] = edges[i].id;
        }
    }

    qsort(result, count, sizeof(int), cmp_id);

    printf("%d\n", total);
    for (int i = 0; i < count; i++) {
        printf("%d", result[i]);
        if (i < count - 1) printf(" ");
    }
    printf("\n");

    return 0;
}

