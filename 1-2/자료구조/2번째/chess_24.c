#include <stdio.h>

#define SIZE 19
int board[SIZE][SIZE];

int check(int i, int j) {
    if (board[i][j] == board[i][j + 1] &&
        board[i][j] == board[i][j + 2] &&
        board[i][j] == board[i][j + 3]) {
        if (j > 0 && board[i][j - 1] == 0)
            return board[i][j];
        if (j + 3 < SIZE - 1 && board[i][j + 4] == 0)
            return board[i][j];
    }
    if (board[i][j] == board[i + 1][j] &&
        board[i][j] == board[i + 2][j] &&
        board[i][j] == board[i + 3][j]) {
        if (i > 0 && board[i - 1][j] == 0)
            return board[i][j];
        if (i + 3 < SIZE - 1 && board[i + 4][j] == 0)
                return board[i][j];
            }
    if (board[i][j] == board[i + 1][j + 1] &&
       		board[i][j] == board[i + 2][j + 2] &&
        	board[i][j] == board[i + 3][j + 3]) {
        if (i > 0 && j > 0 && board[i - 1][j - 1] == 0)
            return board[i][j];
    	if (i + 3 < SIZE - 1 && j + 3 < SIZE - 1 && board[i + 4][j + 4] == 0)
        return board[i][j];
    }
 
    if (board[i][j] == board[i + 1][j - 1] &&
        board[i][j] == board[i + 2][j - 2] &&
        board[i][j] == board[i + 3][j - 3]) {
        if (i > 0 && j < SIZE - 1 && board[i - 1][j + 1] == 0)
            return board[i][j];
        if (i + 3 < SIZE - 1 && j - 3 > 0 && board[i + 4][j - 4] == 0)
            return board[i][j];
        }
    return 0;
}

int main() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            scanf("%d", &board[i][j]);
        }
    }
    int winner;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            winner = check(i,j);
            if(winner != 0){
            	printf("%d:%d,%d",winner,i + 1,j + 1);
				break;
			}
        }
        if(winner != 0){
				break;
			}
    }
    if(winner == 0)
    	printf("No\n");
    return 0;
}


