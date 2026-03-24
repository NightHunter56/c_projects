#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]){
	int numArgs = argc -1;
	char* search = argv[1];
	char url[]= "lynx https://duckduckgo.com/?q=";
	int i;
	if( numArgs > 1)
	{
		printf("too many arguments....\n");
		return 0;
	}
	if(numArgs == 0)
	{
		system(url);
		return 0;
	}
	for(i=0; i<= strlen(search); i++){
		if (search[i]== ' '){
			search[i]='+';
		}
	}
	strcat(url, search);
	printf("%s\n", url);
	system(url);
	exit(1);
}
