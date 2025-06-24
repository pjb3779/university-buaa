#include <stdio.h>
#include <string.h>

char s[505];
char encrypt[1005];
int flag[205];
int main()
{
    FILE *fp1 = fopen("encrypt.txt","r");
    FILE *fp2 = fopen("output.txt","w");
    char p;
    int lens = 0,i;
    while((p = getchar())!= EOF && p != '\n') s[lens++] = p;     
    int cnt = 0;
    for(i = 0; i < lens; i++)
        if(flag[s[i]-'a'] == 0) {                              
            s[cnt++] = s[i];
            flag[s[i]-'a'] = 1;
        }
    for( i = 25; i >=0; i--)
        if(flag[i] == 0) s[cnt++] = 'a'+i;                      
    char c; int k = 0;
    while((c = fgetc(fp1))!= EOF) encrypt[k++] = c;
    for(i = 0; i < k; i++)
        if(isalpha(encrypt[i])) fputc(s[encrypt[i] - 'a'],fp2);
        else fputc(encrypt[i],fp2);
    fclose(fp1);
    fclose(fp2);
    return 0;
}


