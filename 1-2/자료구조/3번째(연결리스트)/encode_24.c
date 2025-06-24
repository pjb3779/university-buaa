#define _CRT_SECURE_NO_WARNINGS
#define N 1010
#include <stdio.h>
#include <string.h>
#include <ctype.h>
char s[N];
char r[N];
char p[N];
char temp2[N];
int i, j, x = 0, n = 0, flag = 0, temp = 0;
void sort(char s[], char r[]);
void sort2(char r[], char c);
void sort(char s[], char r[])
{
	for (i = 0; i < strlen(s); i++)
	{
		for (j = x - 1; j >= 0; j--)
		{
			if (s[i] == r[j])
			{
				flag = 1;
				break;
			}
		}
		if (flag)
		{
			flag = 0;
			continue;
		}
		r[x++] = s[i];
	}
	for (i = 32; i <= 126; i++)
	{
		for (j = x - 1; j >= 0; j--)
		{
			if (i == r[j])
			{
				flag = 1;
				break;
			}
		}
		if (flag)
		{
			flag = 0;
			continue;
		}
		r[x++] = i;
	}
}
void sort2(char r[], char c)
{
	for (n = 0; n < 95; n++)
	{
		temp = (int)r[i];
		strcpy(temp2, &r[i + 1]);
		strcpy(&r[i], temp2);
		x--;
		for (j = 1; j < temp; j++)
		{
			i++;
			if (x == 0)
			{
				break;
			}
			if (i >= x)
			{
				i %= x;
			}
		}
		p[temp] = r[i];
	}
	p[temp] = c;

}
int main()
{

	char c; 
	gets(s);
	sort(s, r);
	i = 0;
	c = r[0];
	sort2(r, c);

	freopen("in.txt", "r", stdin);
	freopen("in_crpyt.txt", "w", stdout);
	while (scanf("%c", &c) != EOF)
	{
		if (c >= 32 && c <= 126)
		{
			printf("%c", p[(int)c]);
		}
		else
		{
			printf("%c", c);
		}
	}

	fclose(stdin);
	fclose(stdout);

	return 0;
}


