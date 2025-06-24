#include <stdio.h>
#include <string.h>

//시험대비 반대로 구현해보기 

int main(){
	char arr[101];
	int len;
	int index = 0;
	
	scanf("%s", arr);
	len = strlen(arr);
	
	if(arr[0] == '0'){
		int i = 2;
		while(arr[i] == '0'){
			index++;
			i++;
		}
		
		if(arr[i + 1] != '\0'){	//숫자가 하나가 아니라면 . 붙이기 
			printf("%c.", arr[i++]);
		}	
		 
	 	for(; i < len; i++){ 
			printf("%c", arr[i]);
		}
		
		printf("e-%d\n",index + 1); 
	
	} else if(arr[1] == '.'){	//e만 붙이면 되는경우 
		printf("%se0\n", arr);
	} else {
		for(int i = 0; i < len; i++){
			if(arr[i] == '.'){
				index = i;
			} else if(i == 1){
				printf(".%c", arr[i]);
			} else {
				printf("%c", arr[i]);
			}
		}
		printf("e%d\n", index - 1);
	}	
	return 0;
} 
//시험 반대 
//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//
//int main() {
//    char sci[110];  // 과학적 표기 입력
//    scanf("%s", sci);
//
//    // 밑수와 지수 분리
//    char *e_ptr = strchr(sci, 'e');
//    if (e_ptr == NULL) {
//        printf("Invalid input\n");
//        return 1;
//    }
//
//    // 지수 부분 정수로 변환
//    int exp = atoi(e_ptr + 1);
//
//    // 밑수 부분 추출
//    char base[110];
//    strncpy(base, sci, e_ptr - sci);
//    base[e_ptr - sci] = '\0';
//
//    // 소수점 위치 찾기
//    char *dot_ptr = strchr(base, '.');
//    int dot_index = dot_ptr ? (dot_ptr - base) : strlen(base);
//
//    // 밑수에서 소수점 제거하고 숫자만 추출
//    char digits[110] = {0};
//    int k = 0;
//    for (int i = 0; base[i] != '\0'; i++) {
//        if (base[i] != '.') {
//            digits[k++] = base[i];
//        }
//    }
//
//    // 출력
//    if (exp >= 0) {
//        int len = strlen(digits);
//        if (exp + 1 >= len) {
//            // 소수점 뒤로 밀기
//            printf("%s", digits);
//            for (int i = 0; i < exp + 1 - len; i++)
//                printf("0");
//            printf("\n");
//        } else {
//            // 중간에 소수점 넣기
//            for (int i = 0; i <= exp; i++)
//                putchar(digits[i]);
//            putchar('.');
//            for (int i = exp + 1; i < len; i++)
//                putchar(digits[i]);
//            printf("\n");
//        }
//    } else {
//        // 지수가 음수인 경우: 0.00...숫자
//        printf("0.");
//        for (int i = 1; i < -exp; i++)
//            putchar('0');
//        printf("%s\n", digits);
//    }
//
//    return 0;
//}

