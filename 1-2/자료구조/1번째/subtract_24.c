#include <stdio.h>
#include <string.h>
#include <ctype.h>
int c[100] = {0,};

int judge(char a[], char b[])
{
    int i;
    if (strlen(a) < strlen(b)) {
        return 0;
    }
    else if (strlen(a) > strlen(b)) {
        return 1;
    }

    for (i = 0; i < strlen(a); i++) {
        if (a[i] < b[i]) {
            return 0;
        }
    }
    return 1;
}
void minus(char a[], char b[], int k1, int k2)
{
    int i, j;
    char k;
    for (i = 0; i < k1; i++, k1--)
    {
        k = a[i];
        a[i] = a[k1];
        a[k1] = k;
    }
    for (i = 0; i < k2; i++, k2--)
    {
        k = b[i]; b[i] = b[k2]; b[k2] = k;
    }
    for (i = 0; i < strlen(a); i++)
    {
        if (i >= strlen(b)) {
            c[i] = a[i] - '0';
        }
        else {
            c[i] = a[i] - b[i];
        }
    }
    for (i = 0; i < strlen(a); i++)
        while (c[i] < 0) {
            c[i] += 10;
            c[i + 1] -= 1;
        }
}
int main()
{
    int i, j, k1, k2;
    char a[100], b[100];
    gets(a);
    gets(b);
    k1 = strlen(a) - 1;
    k2 = strlen(b) - 1;
    if (judge(a, b)) {
        minus(a, b, k1, k2);
    }
    else {
        printf("-");
        minus(b, a, k2, k1);
    }
    i = 100;
    while (c[i] == 0 && i != 0) {
        i--;
    }
    printf("%d\n",i);
    for (j = i; j >= 0; j--) {
        printf("%d", c[j]);
    }
}

//#include <stdio.h>
//#include <string.h>
//
//char arr1[81], arr2[81], ans[81];
//
//void reverse(char arr[]){
//	int len = strlen(arr);
//	for(int i = 0; i < len / 2; i++){
//		char temp = arr[i]; arr[i] = arr[len -i - 1]; arr[len -i -1] = temp;
//	}
//}
//
//void minus(char arr1[], char arr2[], int len){
//	int carry = 0;
//	for(int i = 0; i < len; i++){
//		int digit1 = arr1[i] - '0';
//		int digit2 = arr2[i] - '0';
//		
//		int diff = digit1 - digit2 - carry;
//		if(diff < 0){
//			diff += 10;
//			carry = 1;
//		}else{
//			carry = 0;
//		}
//		ans[i] = diff + '0';
//	}
//	reverse(ans);
//	ans[len] = '\0';
//}
//
//int main(){
//	char input[81];
//	scanf("%s %s", arr1, arr2);
//	reverse(arr1);
//	reverse(arr2);
//	
//	int len1 = strlen(arr1);
//	int len2 = strlen(arr2);
//	
//	int flag = -1;
//	
//	if(len1 > len2)
//		flag = 0;
//	else if(len1 < len2)
//		flag = 1;
//	else{
//		for(int i = 0; i < len1; i++){
//			if(arr1[i] > arr2[i]){
//				flag = 0;
//				break;
//			}else if(arr1[i] < arr2[i]){
//				flag = 1;
//				break;
//			}
//		}
//	}
//	if(flag == 0)
//		minus(arr1, arr2, len1);
//	if(flag == 1){
//		minus(arr2, arr1, len2);
//		printf("-");
//	}
//	for(int i = 0; i < len2; i++){
//		printf("%c", ans[i]);
//	}
//	return 0;
//}

