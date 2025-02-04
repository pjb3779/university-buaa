#include <stdio.h>

int main() {
    int T;
    scanf("%d", &T);

    while (T--) {
        int x, y, a, b;
        scanf("%d %d %d %d", &x, &y, &a, &b);

        int toys = 0;

        int min_parts = (a < b) ? a : b;
        int max_parts = (a > b) ? a : b;


        int toys_from_xy = min_parts / x;
        int toys_from_yx = min_parts / y;

        if (toys_from_xy > toys_from_yx) {
            toys += toys_from_xy;
            a -= x * toys_from_xy;
            b -= y * toys_from_xy;
        } else {
            toys += toys_from_yx;
            a -= y * toys_from_yx;
            b -= x * toys_from_yx;
        }

        toys += (a / (x + y)) + (b / (x + y));

        printf("%d\n", toys);
    }

    return 0;
}

