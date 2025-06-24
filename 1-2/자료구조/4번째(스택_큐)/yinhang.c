#include <stdio.h>
#include <stdlib.h>

#define MAX_CLIENTS 1000
#define MAX_WINDOWS 5
#define BASE_WINDOWS 3

typedef struct {
    int id;
    int service_time;  // 1~3
    int wait_time;
} Client;

typedef struct {
    int remain_time;   // 창구가 바쁠 경우 > 0
    int serving_id;    // 현재 처리 중인 고객 ID
} Window;

Client queue[MAX_CLIENTS];
int front = 0, rear = 0;
int time_periods, client_counts[100];
int client_services[MAX_CLIENTS];
int total_clients = 0;

Window windows[MAX_WINDOWS];  // 0~2: 기본, 3~4: 전환 가능

// 큐 연산
void enqueue(int id, int service_time) {
    queue[rear].id = id;
    queue[rear].service_time = service_time;
    queue[rear].wait_time = 0;
    rear++;
}
Client dequeue() {
    return queue[front++];
}
int queue_size() {
    return rear - front;
}

// 평균 대기열 계산
double average_wait_per_window(int active_window_count) {
    if (active_window_count == 0) return 0;
    return (double)(queue_size()) / active_window_count;
}

// 최소 창구 수로 1인당 대기 < 7 되게
int calculate_needed_windows() {
    for (int i = BASE_WINDOWS; i <= MAX_WINDOWS; i++) {
        if (average_wait_per_window(i) < 7.0) {
            return i;
        }
    }
    return MAX_WINDOWS;
}

int main() {
    scanf("%d", &time_periods);
    for (int i = 0; i < time_periods; i++) {
        scanf("%d", &client_counts[i]);
    }
    for (int i = 0; i < time_periods; i++) {
        for (int j = 0; j < client_counts[i]; j++) {
            scanf("%d", &client_services[total_clients++]);
        }
    }

    int now = 0;        // 현재 시각 (타임사이클)
    int current_client = 0;
    int active_window_count = BASE_WINDOWS;

    int result[MAX_CLIENTS] = {0};

    while (front < total_clients || now < time_periods || queue_size() > 0) {
        // 1. 새로운 고객 도착
        if (now < time_periods) {
            int count = client_counts[now];
            for (int i = 0; i < count; i++) {
                enqueue(current_client + 1, client_services[current_client]);
                current_client++;
            }

            // 고객 도착 시에만 창구 추가 가능
            active_window_count = calculate_needed_windows();
        }

        // 2. 대기 고객들 대기 시간 증가
        for (int i = front; i < rear; i++) {
            queue[i].wait_time++;
        }

        // 3. 각 창구에서 처리 중인 고객 시간 감소
        for (int i = 0; i < active_window_count; i++) {
            if (windows[i].remain_time > 0) {
                windows[i].remain_time--;
            }

            // 4. 완료되면 다음 고객 할당
            if (windows[i].remain_time == 0 && queue_size() > 0) {
                Client c = dequeue();
                result[c.id] = c.wait_time - 1; // 마지막 증가 제외
                windows[i].remain_time = c.service_time;
                windows[i].serving_id = c.id;
            }
        }

        // 5. 대기 줄 감소했을 경우 창구 줄이기
        if (average_wait_per_window(active_window_count) < 7.0 && active_window_count > BASE_WINDOWS) {
            active_window_count--;
        }

        now++;
    }

    // 출력
    for (int i = 1; i <= total_clients; i++) {
        printf("%d : %d\n", i, result[i]);
    }

    return 0;
}

