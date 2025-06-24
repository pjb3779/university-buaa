#include <stdio.h>
#include <string.h>

int ans[81] = {0,};

int compare(char arr1[], char arr2[]){
	int a = strlen(arr1);
	int b = strlen(arr2);
	
	if(a > b){
		return 0;
	} else if(a < b){
		return 1;
	}
	
	for(int i = 0; i < a; i++){
		if(arr1[i] > arr2[i]){
			return 0;
		} else if(arr1[i] < arr2[i]){
			return 1;
		}
	}
	return 2;
}

void minus(char arr1[], char arr2[]){
	int a = strlen(arr1);
	int b = strlen(arr2);
	int carry = 0;
	char temp;
	
	for(int i = 0, j = a - 1; i < j; i++, j--){
		temp = arr1[i];
		arr1[i] = arr1[j];
		arr1[j] = temp;
	}
	
	for(int i = 0,  j = b - 1; i < j; i++, j--){
		temp = arr2[i];
		arr2[i] = arr2[j];
		arr2[j] = temp;
	}

	for(int i = 0; i < a; i++){
		if (i >= b) {
            ans[i] = arr1[i] - '0';
        }
        else {
            ans[i] = arr1[i] - arr2[i];
        }
	}
	
	for (int i = 0; i < a; i++)	//캐리 처리 
        while (ans[i] < 0) {
            ans[i] += 10;
            ans[i + 1] -= 1;
        }
        
    int i = 81;
    while (ans[i] == 0 && i != 0) {
        i--;
    }
    for (int j = i; j >= 0; j--) {
        printf("%d", ans[j]);
    }
}

int main(){
	char arr1[81];
	char arr2[81];
	
	scanf("%s", arr1);
	scanf("%s", arr2);
	
	int result = compare(arr1, arr2);
	
	if(result == 0){
		minus(arr1, arr2);
	} else if(result == 1){
		printf("-");
		minus(arr2, arr1);
	} else {
		printf("0\n");
	}
	
	return 0;
}


