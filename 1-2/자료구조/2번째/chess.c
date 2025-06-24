#include <stdio.h>

int arr[19][19];

int check(int i, int j) {
    if (arr[i][j] == arr[i][j + 1] &&
        arr[i][j] == arr[i][j + 2] &&
        arr[i][j] == arr[i][j + 3]) {	//가로 
        if (j > 0 && arr[i][j - 1] == 0)
            return arr[i][j];
        if (j + 3 < 18 && arr[i][j + 4] == 0)
            return arr[i][j];
    }
    if (arr[i][j] == arr[i + 1][j] &&
        arr[i][j] == arr[i + 2][j] &&
        arr[i][j] == arr[i + 3][j]) {	//세로 
        if (i > 0 && arr[i - 1][j] == 0)
            return arr[i][j];
        if (i + 3 < 18 && arr[i + 4][j] == 0)
                return arr[i][j];
            }
    if (arr[i][j] == arr[i + 1][j + 1] &&
       		arr[i][j] == arr[i + 2][j + 2] &&
        	arr[i][j] == arr[i + 3][j + 3]) {	//오른 대각 
        if (i > 0 && j > 0 && arr[i - 1][j - 1] == 0)
            return arr[i][j];
    	if (i + 3 < 18 && j + 3 < 18 && arr[i + 4][j + 4] == 0)
        	return arr[i][j];
    }
 
    if (arr[i][j] == arr[i + 1][j - 1] &&
        arr[i][j] == arr[i + 2][j - 2] &&
        arr[i][j] == arr[i + 3][j - 3]) {	//왼쪽 대각 
        if (i > 0 && j < 18 && arr[i - 1][j + 1] == 0)
            return arr[i][j];
        if (i + 3 < 18 && j - 3 > 0 && arr[i + 4][j - 4] == 0)
            return arr[i][j];
        }
    return 0;
}

int main(){
	int index = 0;
	int flag = 0;
		
	for(int i = 0; i < 19; i++){
		for(int j = 0; j < 19; j++){
			scanf("%d", &arr[i][j]);
			if(arr[i][j] == 1 || arr[i][j] == 2){
				if(flag == 0){
					index = i;
					flag = 1;
				}	
			}
		}
	}
	
	int winner = 0;
	
	for(int i = index; i < 19; i++){
		for(int j = 0; j < 19; j++){
			if(arr[i][j] == 1 || arr[i][j] == 2){
				if((winner = check(i, j)) != 0){
					printf("%d:%d,%d\n", winner, i + 1, j + 1);
					break;
				}	
			}
		}
		if(winner != 0)	//두번째 포문 탈출 
			break;
	}
	
	if(winner == 0){
		printf("No\n");
	}
	
	return 0;
}
