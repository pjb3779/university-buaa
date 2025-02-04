#include <stdio.h>

int main(void) {
    int t;
    scanf("%d", &t);

    while (t--) {
        int n;
        scanf("%d", &n);
        long long A[201][201];
        long long B[201][201];

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%lld", &A[i][j]);
            }
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                scanf("%lld", &B[i][j]);
            }
        }
        for (int i = 0; i < n; i++) {
        	long long ans = 0;
            for (int j = 0; j < n; j++) {
                for (int k = 0; k < n; k++) {
                    ans += A[i][k] * B[k][j];
                }
                printf("%lld ", ans);
                ans = 0;
            }
            printf("\n");
        }
    }

    return 0;
}
