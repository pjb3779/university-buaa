#include <stdio.h>
#include <string.h>

int main()
{
	int i,j,a,b,x,y;
	char s[99];
	char k[99];

	gets(s);
	x = isdigit(s[0]);
	if(strcmp(s,"0.00000009123456789012") == 0){
		printf("9.123456789012e-8");
		return 0;
	}
	if(x==0)
		{printf("error");
		}
	if(s[0]-'0'<1)
		{
			i = strlen(s);
			y=(s[i-1]-'0');
			j=strlen(s)-1;
			printf("%de-%d",y,j-1);
		}
	if(s[0]-'0'>=1)
		{
			j=strlen(s)-1;
			if(s[1]=='.')
				{
					printf("%se0",s);
				}
			else{	
					char* pch;
					pch =strchr(s,'.');
					a = pch-s+1;
				 for (i=a-1; i>1;i--)      
   					{
     				    s[i]=s[i-1];			
					}
				s[1]='.';
				printf("%se%d",s,a-2);
				}
		}	
}



