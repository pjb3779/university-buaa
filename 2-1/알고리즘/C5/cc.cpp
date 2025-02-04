#include <stdio.h>
#include <math.h>

// 두 점 사이의 거리를 계산하는 함수
double distance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}
// 점 A와 선분 PQ 사이의 최소 거리를 계산하는 함수
double minDistance(double xA, double yA, double xP, double yP, double xQ, double yQ) {
    double minDist = distance(xA, yA, xP, yP);
    double dist = distance(xA, yA, xQ, yQ);
    // PQ 선분의 길이와 방향 벡터 계산
    double dx = xQ - xP;
    double dy = yQ - yP;
    // PQ가 점이 아닌 선분인 경우
    if (dx != 0 || dy != 0) {
        // 점 A를 선분 PQ에 투영한 지점의 파라미터 t 계산
        double t = ((xA - xP) * dx + (yA - yP) * dy) / (dx * dx + dy * dy);
        // t 값에 따라 최소 거리 계산
        if (t > 1) {
            dist = distance(xA, yA, xQ, yQ);
        } else if (t > 0) {
            double x = xP + t * dx;
            double y = yP + t * dy;
            dist = distance(xA, yA, x, y);
        } else {
            dist = distance(xA, yA, xP, yP);
        }
        // 최소 거리 갱신
        if (dist < minDist) {
            minDist = dist;
        }
    }
    return minDist;
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        double xA, yA, xP, yP, xQ, yQ;
        scanf("%lf %lf", &xA, &yA);
        scanf("%lf %lf %lf %lf", &xP, &yP, &xQ, &yQ);
        // 최소 거리 계산 및 출력
        double result = minDistance(xA, yA, xP, yP, xQ, yQ);
        printf("%.3lf\n", result);
    }
    return 0;
}

