
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>


int main(){	
	int runsyst = system("pgrep -n nm-applet");
	if (runsyst == 0){
		system("killall nm-applet && nm-applet &");
	}
	return 0;
}
