#include <stdio.h>

struct Point {
    int x;
    int y;
};

double calculateSlope(const struct Point* p1, const struct Point* p2) {
    if (p1->x == p2->x) {
        return 1.0 / 0.0;
    }
    return (double)(p2->y - p1->y) / (double)(p2->x - p1->x);
}

int areCollinear(const struct Point* points, int n) {
    double slope = calculateSlope(&points[0], &points[1]);
    for (int i = 2; i < n; i++) {
        if (calculateSlope(&points[i - 1], &points[i]) != slope) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int t;
    scanf("%d", &t);

    while (t--) {
        int n;
        scanf("%d", &n);

        struct Point points[n];
        for (int i = 0; i < n; i++) {
            scanf("%d %d", &points[i].x, &points[i].y);
        }

        if (areCollinear(points, n)) {
            printf("boo how! boo how!\n");
        } else {
            printf("how?\n");
        }
    }

    return 0;
}
