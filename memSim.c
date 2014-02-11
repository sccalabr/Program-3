#include<stdio.h>
#include<stdlib.h>
#include"memSim.h"

unsigned int frames = 0;
unsigned int physicalMemorySize = 0;
unsigned double pageTableLookUps = 0;
unsigned double pageFaults = 0;
unsigned double tlbHits = 0;
unsigned double tlbMisses = 0;

char *physicalMemory = NULL;

FILE *file = NULL;

NEED_TO_THINK_OF_A_GOOD_NAME tlb[TLB_TABLE_ENTRIES];
NEED_TO_THINK_OF_A_GOOD_NAME pageTable[PAGE_TABLE_ENTRIES];


void noReplacement() {


}

void fifoReplacement() {


}

void lruReplacement() {


}

void optReplacement() {


}

int main(int argc, char **argv) 
{
	
	file = fopen(argv[1], "r");
	
	frames = atoi(argv[2]);
	physicalMemorySize = PAGE_SIZE * frames;
	
	physicalMemory = (char *)calloc(1, physicalMemorySize);
	
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
