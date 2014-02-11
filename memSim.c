#include<stdio.h>
#include<stdlib.h>

int frames = 0;
FILE *file = null;

int main(int argc, char **argv) 
{
	
	file = fopen(argv[1], "r");
	frames = atoi(argv[2]);
	
	if(argc == 3) {
		noReplacement();
	}
	else if(strcmp(argv[3], "fifo")) {
		fifoReplacement();
	}
	else if(strcmp(argv[3], "lru")) {
		lruReplacement();
	}
	else if(strcmp(argv[3], "opt")){
		optReplacement();
	}
	else {
		printf("NOT SUPPORTED\n");
		exit(0);
	}
	
	fclose(file);
}
