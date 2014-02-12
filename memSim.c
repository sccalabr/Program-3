#include<stdio.h>
#include<stdlib.h>
#include"memSim.h"

unsigned int frames = 0;
unsigned int physicalMemorySize = 0;
double pageTableLookUps = 0;
double pageFaults = 0;
double tlbHits = 0;
double tlbMisses = 0;
int sizeOfTLB = 0;

char *physicalMemory = NULL;

FILE *file = NULL;

Node *tlb = NULL;
Node *startTLB = NULL;
Node *currentTLB = NULL;
Node *endTLB = NULL;
NEED_TO_THINK_OF_A_GOOD_NAME pageTable[PAGE_TABLE_ENTRIES];

Node *checkIfInTLB(unsigned int pageNumber) {
   
   if(sizeOfTLB == 0) {
      return NULL;
   }
   
   Node *iterator = startTLB;
   int i;
   
   for(i = 0; iterator && i < TLB_TABLE_ENTRIES; i++, iterator = (Node *)iterator->next) {
      NEED_TO_THINK_OF_A_GOOD_NAME *thing = (NEED_TO_THINK_OF_A_GOOD_NAME *)iterator->thing;
      int thingsPageNum = thing->logicalMemoryPageNum;
      if(pageNumber == thingsPageNum) {
         return iterator;
      }
   }

   return NULL;
}

void makeNewNode() {

   Node *temp = (Node *)calloc(1, sizeof(Node));
  // need to fill temp->thing in
   endTLB->next = temp;
   temp->prev = endTLB;
   endTLB = temp;
}

void removeAndFreeStartNode() {
   makeNewNode();
   
   Node *toFree = startTLB;
   startTLB = startTLB->next;
   
   free(toFree->thing);
   free(toFree);
}

void noReplacement() {
   
}

void fifoReplacement() {
   int pageNumber;
   currentTLB = checkIfInTLB(pageNumber);
   if(!currentTLB) {
      if(sizeOfTLB == TLB_TABLE_ENTRIES) {
            makeNewNode();
            removeAndFreeStartNode();
         /*
          *
          *DO REPLACEMENT- This is either going to be replacing the Node which we 
          * would have to free OR replace values of the node;
          */
      }
      else {
         if(startTLB == NULL) {
            startTLB = (Node *) calloc(1, sizeof(Node));
            // need to fill start->thing in
            endTLB = startTLB;
            
            
         }
         else {
            makeNewNode();
         }
         sizeOfTLB++;
      }
   }
}

void lruReplacement() {
   int pageNumber;
   
   currentTLB = checkIfInTLB(pageNumber);
   
   if(currentTLB) {
      currentTLB->prev->next = currentTLB->next;
      currentTLB->next->prev = currentTLB->prev;
      endTLB->next = currentTLB;
      currentTLB->prev = endTLB;
      endTLB = currentTLB;
   }
   else {
      if(sizeOfTLB == TLB_TABLE_ENTRIES) {
      makeNewNode();
      removeAndFreeStartNode();
      
      /*
       *
       *DO REPLACEMENT- This is either going to be replacing the Node which we 
       * would have to free and add a new one to end OR replace values of the node;
       */
      }
      else {
         if(startTLB == NULL) {
            startTLB = (Node *)calloc(1, sizeof(Node));
            // need to fill start->thing in
            currentTLB = startTLB;
            endTLB = startTLB;
         }
         else {
            makeNewNode();
         }
         sizeOfTLB++;
      }
   }


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
