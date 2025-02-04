/*
 * trans.c - Matrix transpose B = A^T
 *
 * Your transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * The transpose function is evaluated by counting the number of misses
 * on a 512B direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */

void transpose_submit(int M, int N, int A[N][M], int B[M][N]) {
    /*
     * Please define your local variables here.
     * You are allowed to define at most 12 local variables.
     */
    int i, j, k, l;
    int temp1, temp2, temp3, temp4, temp5, temp6, temp7;
    for (i = 0; i < N; i += 8) {
        for (j = 0; j < M; j += 8) {
            for (k = i; k < i + 4; k++) {
                l = A[k][j];
                temp1 = A[k][j + 1];
                temp2 = A[k][j + 2];
                temp3 = A[k][j + 3];
                temp4 = A[k][j + 4];
                temp5 = A[k][j + 5];
                temp6 = A[k][j + 6];
                temp7 = A[k][j + 7];
                B[j][k] = l;
                B[j + 1][k] = temp1;
                B[j + 2][k] = temp2;
                B[j + 3][k] = temp3;
                B[j][k + 4] = temp4;
                B[j + 1][k + 4] = temp5;
                B[j + 2][k + 4] = temp6;
                B[j + 3][k + 4] = temp7;
            }

            for (k = j; k < j + 4; k++) {
                l = B[k][i + 4];
                temp1 = B[k][i + 5];
                temp2 = B[k][i + 6];
                temp3 = B[k][i + 7];
                temp4 = A[i + 4][k];
                temp5 = A[i + 5][k];
                temp6 = A[i + 6][k];
                temp7 = A[i + 7][k];
                B[k][i + 4] = temp4;
                B[k][i + 5] = temp5;
                B[k][i + 6] = temp6;
                B[k][i + 7] = temp7;
                B[k + 4][i] = l;
                B[k + 4][i + 1] = temp1;
                B[k + 4][i + 2] = temp2;
                B[k + 4][i + 3] = temp3;
            }
            for (k = i + 4; k < i + 8; k++) {
                l = A[k][j + 4];
                temp1 = A[k][j + 5];
                temp2 = A[k][j + 6];
                temp3 = A[k][j + 7];
                B[j + 4][k] = l;
                B[j + 5][k] = temp1;
                B[j + 6][k] = temp2;
                B[j + 7][k] = temp3;
            }
        }
    }
}
