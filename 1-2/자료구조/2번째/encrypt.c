#include <stdio.h>
#include <ctype.h>
 
char key[51];
char encrypt[1001];
int flag[25];
int main()
{
    FILE *fp1 = fopen("encrypt.txt","r");
    FILE *fp2 = fopen("output.txt","w");
    char c;
    int lens = 0;
	int cnt = 0;
    
	while((c = getchar()) != '\n') key[lens++] = c;     
    
    for(int i = 0; i < lens; i++){	//키 생성 중복제거 
        if(flag[key[i]-'a'] == 0) {                              
            key[cnt++] = key[i];
            flag[key[i]-'a'] = 1;
        }
	}
        
    for(int i = 25; i >=0; i--){	//끝부터 키 생성 
        if(flag[i] == 0) key[cnt++] = 'a'+i;                      
	}
    
	int j = 0;
	
    while((c = fgetc(fp1))!= EOF) encrypt[j++] = c;
    for(int i = 0; i < j; i++){
        if(isalpha(encrypt[i])){	//알파벳이면 암호화 
			fputc(key[encrypt[i] - 'a'],fp2);	
        } else {	//아니면 그대로 입력 
			fputc(encrypt[i],fp2);
        }
	}
    fclose(fp1);
    fclose(fp2);
    return 0;
}


