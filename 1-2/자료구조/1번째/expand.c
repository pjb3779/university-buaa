#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

int judge(char p, char b){
	if(p < b){
		if((isdigit(p) && isdigit(b)) || (isupper(p) && isupper(b)) || (islower(p) && islower(b))){
			for(char c = p + 1; c < b; c++){
				printf("%c", c);
			}
			return 0;
		}
	}
	return 1;
}

int main(){
	char arr[101];
	
	scanf("%s", arr);
	
	int len = strlen(arr);
	
	for(int i = 0; i < len; i++){
		if(arr[i] == '-'){
			if(judge(arr[i - 1] , arr[i + 1]) == 0)
				continue;
		}
		printf("%c", arr[i]);
	}
	
	return 0;
} 

//시험대비 
//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>
//
//// 같은 범주인지 확인 (소문자/대문자/숫자)
//int same_type(char a, char b) {
//    if (islower(a) && islower(b)) return 1;
//    if (isupper(a) && isupper(b)) return 1;
//    if (isdigit(a) && isdigit(b)) return 1;
//    return 0;
//}
//
//int main() {
//    char input[200];
//    scanf("%s", input);
//
//    int len = strlen(input);
//    int i = 0;
//
//    while (i < len) {
//        int start = i;
//
//        // 연속되는 문자를 탐색
//        while (i + 1 < len &&
//               input[i + 1] == input[i] + 1 &&
//               same_type(input[i], input[i + 1])) {
//            i++;
//        }
//
//        int seq_len = i - start + 1;
//
//        if (seq_len >= 3) {
//            // 3자 이상 연속되면 압축
//            printf("%c-%c", input[start], input[i]);
//        } else {
//            // 2자 이하면 그대로 출력
//            for (int j = start; j <= i; j++) {
//                printf("%c", input[j]);
//            }
//        }
//
//        i++;  // 다음 문자로 이동
//    }
//
//    printf("\n");
//    return 0;
//}
