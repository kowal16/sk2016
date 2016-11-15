#include<stdio.h>
#include<string.h>
int main()
{
	char s[1024];
	int i = 0;

	while(scanf("%c",&s[i]))
	{
		if(s[i]=='?')
		break;
		i++;
	}
	printf("dlugosc tablicy %d\n ",strlen(s));
	return 0;
}
