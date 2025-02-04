#include <stdio.h>

#define MAX_N 501
#define MAX_M 501

int main() {
    int tt;
    scanf("%d", &tt);

    while (tt--) {
        int n, m, h;
        scanf("%d %d %d", &n, &m, &h);
        int a[MAX_N][MAX_M];
        long long dp[MAX_N][MAX_M];
        
        // 입력 받기
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                scanf("%d", &a[i][j]);
            }
        }

        // 초기화
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                dp[i][j] = 0;
            }
        }

        dp[0][0] = h + a[0][0];
        int mx = 0;

        // DP 계산
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (i - 1 >= 0 && dp[i - 1][j] > 0) {
                    if (dp[i - 1][j] - 1 > 0) dp[i][j] = (dp[i][j] > dp[i - 1][j] - 1 + a[i][j]) ? dp[i][j] : dp[i - 1][j] - 1 + a[i][j];
                    mx = (mx > i) ? mx : i;
                }
                if (j - 1 >= 0 && dp[i][j - 1] > 0) {
                    if (dp[i][j - 1] - 1 > 0) dp[i][j] = (dp[i][j] > dp[i][j - 1] - 1 + a[i][j]) ? dp[i][j] : dp[i][j - 1] - 1 + a[i][j];
                    mx = (mx > i) ? mx : i;
                }
            }
        }

        // 결과 출력
        int ok = 0;
        for (int j = 0; j < m; j++) {
            if (dp[n - 1][j] > 0) {
                ok = 1;
                break;
            }
        }

        if (ok) {
            printf("Yes\n");
        } else {
            printf("%d\n", mx + 1);
        }
    }

    return 0;
}

