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
#define BLOCK_SIZE 8
/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */

void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
    /*
     * Please define your local variables here. 
     * You are allowed to define at most 12 local variables.
    */
    
    int i, j, ii, jj;
    int val1, val2, val3, val4, val5, val6, val7, temp;

    if (M == 16) {  // 16x16 행렬 처리
        for (ii = 0; ii < N; ii += 8) {
            for (jj = 0; jj < M; jj += 8) {
                for (i = ii; i < ii + 8 && i < N; i++) {
                    for (j = jj; j < jj + 8 && j < M; j++) {
                        if (i != j) {
                            B[j][i] = A[i][j];
                        } else {
                            val1 = A[i][j];
                        }
                    }
                    if (ii == jj) {
                        B[i][i] = val1;
                    }
                }
            }
        }
    } else if (M == 32) {  // 32x32 행렬 처리
        for (ii = 0; ii < N; ii += 8) {
            for (jj = 0; jj < M; jj += 8) {
                for (i = ii; i < ii + 4 && i < N; i++) {
                    // 첫 번째 블록에 대해 행렬 값을 읽어들임
                    temp = A[i][jj];
                    val1 = A[i][jj + 1];
                    val2 = A[i][jj + 2];
                    val3 = A[i][jj + 3];
                    val4 = A[i][jj + 4];
                    val5 = A[i][jj + 5];
                    val6 = A[i][jj + 6];
                    val7 = A[i][jj + 7];

                    // 두 번째 블록으로 값을 저장
                    B[jj][i] = temp;
                    B[jj + 1][i] = val1;
                    B[jj + 2][i] = val2;
                    B[jj + 3][i] = val3;
                    B[jj][i + 4] = val4;
                    B[jj + 1][i + 4] = val5;
                    B[jj + 2][i + 4] = val6;
                    B[jj + 3][i + 4] = val7;
                }

                for (i = jj; i < jj + 4 && i < M; i++) {
                    // B의 나머지 부분에 대해서도 같은 작업을 처리
                    temp = B[i][ii + 4];
                    val1 = B[i][ii + 5];
                    val2 = B[i][ii + 6];
                    val3 = B[i][ii + 7];

                    val4 = A[ii + 4][i];
                    val5 = A[ii + 5][i];
                    val6 = A[ii + 6][i];
                    val7 = A[ii + 7][i];

                    B[i][ii + 4] = val4;
                    B[i][ii + 5] = val5;
                    B[i][ii + 6] = val6;
                    B[i][ii + 7] = val7;
                    B[i + 4][ii] = temp;
                    B[i + 4][ii + 1] = val1;
                    B[i + 4][ii + 2] = val2;
                    B[i + 4][ii + 3] = val3;
                }
                
                for(i = ii + 4; i < ii + 8; i++){
                	temp = A[i][jj + 4];
	                val1 = A[i][jj + 5];
	                val2 = A[i][jj + 6];
	                val3 = A[i][jj + 7];
	                B[jj + 4][i] = temp;
	                B[jj + 5][i] = val1;
	                B[jj + 6][i] = val2;
	                B[jj + 7][i] = val3;
				}
            }
        }
    }
}




