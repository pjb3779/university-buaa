#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATIONS 1000
#define MAX_EDGES 2000
#define INF 999999

typedef struct {
    char name[64];
    int is_transfer;
    int index;
} Station;

typedef struct {
    int to;
    int line;
    int weight;
    int next;
} Edge;

Station stations[MAX_STATIONS];
int station_count = 0;

Edge edges[MAX_EDGES * 2];
int head[MAX_STATIONS];
int edge_count = 0;

int line_map[MAX_STATIONS][MAX_STATIONS]; // 출발-도착 기준의 노선 저장

int dist[MAX_STATIONS], prev[MAX_STATIONS];
int visited[MAX_STATIONS];

int get_station_index(const char *name) {
    for (int i = 0; i < station_count; i++) {
        if (strcmp(stations[i].name, name) == 0)
            return i;
    }
    return -1;
}

int add_station(const char *name, int is_transfer) {
    int idx = get_station_index(name);
    if (idx != -1) return idx;

    strcpy(stations[station_count].name, name);
    stations[station_count].is_transfer = is_transfer;
    stations[station_count].index = station_count;
    return station_count++;
}

void add_edge(int u, int v, int line) {
    edges[edge_count] = (Edge){v, line, 1, head[u]};
    head[u] = edge_count++;
    edges[edge_count] = (Edge){u, line, 1, head[v]};
    head[v] = edge_count++;

    line_map[u][v] = line;
    line_map[v][u] = line;
}

void dijkstra(int start) {
    for (int i = 0; i < station_count; i++) {
        dist[i] = INF;
        prev[i] = -1;
        visited[i] = 0;
    }
    dist[start] = 0;

    for (int i = 0; i < station_count; i++) {
        int u = -1;
        for (int j = 0; j < station_count; j++) {
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) {
                u = j;
            }
        }

        if (dist[u] == INF) break;

        visited[u] = 1;

        for (int e = head[u]; e != -1; e = edges[e].next) {
            int v = edges[e].to;
            if (dist[v] > dist[u] + edges[e].weight) {
                dist[v] = dist[u] + edges[e].weight;
                prev[v] = u;
            }
        }
    }
}

void print_path(int start, int end) {
    int path[MAX_STATIONS];
    int count = 0;

    for (int v = end; v != -1; v = prev[v]) {
        path[count++] = v;
    }

    int last_line = -1, ride_count = 0;
    printf("%s", stations[path[count - 1]].name);

    for (int i = count - 2; i >= 0; i--) {
        int u = path[i + 1];
        int v = path[i];
        int line = line_map[u][v];

        if (line != last_line) {
            if (last_line != -1) {
                printf("(%d)-%s", ride_count, stations[u].name);
            }
            printf("-%d", line);
            ride_count = 1;
            last_line = line;
        } else {
            ride_count++;
        }
    }

    printf("(%d)-%s\n", ride_count, stations[path[0]].name);
}

int main() {
    FILE *fp = fopen("bgstations.txt", "r");
    if (!fp) {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    int line_total;
    fscanf(fp, "%d", &line_total);
    memset(head, -1, sizeof(head));

    for (int i = 0; i < line_total; i++) {
        int line_no, station_num;
        fscanf(fp, "%d %d", &line_no, &station_num);

        int prev_idx = -1;
        for (int j = 0; j < station_num; j++) {
            char name[64];
            int is_transfer;
            fscanf(fp, "%s %d", name, &is_transfer);
            int idx = add_station(name, is_transfer);
            if (j > 0) {
                add_edge(prev_idx, idx, line_no);
            }
            prev_idx = idx;
        }
    }

    fclose(fp);

    char start_name[64], end_name[64];
    scanf("%s", start_name);
    scanf("%s", end_name);

    int start_idx = get_station_index(start_name);
    int end_idx = get_station_index(end_name);

    if (start_idx == -1 || end_idx == -1) {
        printf("해당 역을 찾을 수 없습니다.\n");
        return 1;
    }

    dijkstra(start_idx);
    print_path(start_idx, end_idx);

    return 0;
}

