#include <stdio.h>
#include <limits.h>

#define INF LLONG_MAX

typedef long long ll;

void floydWarshall(int vertices, int edges, ll graph[vertices][vertices]) {
    for (int k = 0; k < vertices; k++) {
        for (int i = 0; i < vertices; i++) {
            for (int j = 0; j < vertices; j++) {
                if (graph[i][k] < INF && graph[k][j] < INF) {
                    if (graph[i][j] > graph[i][k] + graph[k][j]) {
                        graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
            }
        }
    }
}

int main() {
    int numVertices, numEdges;
    scanf("%d %d", &numVertices, &numEdges);

    ll distanceMatrix[numVertices][numVertices];

    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            distanceMatrix[i][j] = INF;
        }
        distanceMatrix[i][i] = 0;
    }

    for (int i = 0; i < numEdges; i++) {
        int u, v, w;
        scanf("%d %d %d", &u, &v, &w);

        if (distanceMatrix[u - 1][v - 1] > w) {
            distanceMatrix[u - 1][v - 1] = w;
        }
    }

    floydWarshall(numVertices, numEdges, distanceMatrix);

    int numQueries;
    scanf("%d", &numQueries);

    while (numQueries--) {
        int startVertex, endVertex;
        scanf("%d %d", &startVertex, &endVertex);

        if (distanceMatrix[startVertex - 1][endVertex - 1] == INF) {
            printf("-1\n");
        } else {
            printf("%lld\n", distanceMatrix[startVertex - 1][endVertex - 1]);
        }
    }

    return 0;
}

