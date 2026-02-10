#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
	int numArgs = argc -1;
	char* search = argv[1];
	char url[]= "xdg-open https://duckduckgo.com/?q=";
	if( numArgs > 1)
	{
		printf("too many arguments....\n");
		return 0;
	}
	if(numArgs == 0)
	{
		system("xdg-open https://duckduckgo.com/?q=");
		return 0;
	}
	strcat(url, search);
	system(url);
	return 0;
}
