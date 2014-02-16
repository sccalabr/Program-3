#include<stdio.h>
#include<stdlib.h>
#include"memSim.h"

#define LOGGER  0

unsigned int frames = 0;
unsigned int physicalMemorySize = 0;

double pageHits = 0;
double pageMisses = 0;
double tlbHits = 0;
double tlbMisses = 0;

int sizeOfTLB = 0;
int sizeOfPageTable = 0;
int sizeOfMemBlock = 0;

unsigned int offSetMask = 0; 
unsigned int pageNumberMask = 0;
unsigned int frameNumberCounter = 0;

FILE *file = NULL;
FILE *binaryFile = NULL;

Node *tlb = NULL;
Node *startTLB = NULL;
Node *currentTLB = NULL;
Node *endTLB = NULL;

Node *startPageTable = NULL;
Node *currentPageTable = NULL;
Node *endPageTable = NULL;

MemBlock *startMemBlock = NULL;
MemBlock *currentMemBlock = NULL;
MemBlock *endMemBlock = NULL;

Node *checkIfInTLB(unsigned int pageNumber) {
   
   if(LOGGER) {
      printf("===== CHECK TLB =====\n");
   }
   
   if(sizeOfTLB == 0) {
      return NULL;
   }
   
   Node *iterator = startTLB;
   int i;
   for(i = 0; iterator && i < sizeOfTLB; i++, iterator = (Node *)iterator->next) {
      PageAndFrameNumber *data = (PageAndFrameNumber *)iterator->data;
      int thingsPageNum = data->pageNum;
      
      if(pageNumber == thingsPageNum) {
         return iterator;
      }
   }
   return NULL;
}

Node *checkIfInPageTable(unsigned int pageNumber) {
   
   if(LOGGER) {
      printf("===== CHECK PAGETABLE =====\n");
   }
   
   if(sizeOfPageTable == 0) {
      return NULL;
   }
   
   Node *iterator = startPageTable;
   int i;
   
   for(i = 0; iterator && i < sizeOfPageTable; i++, iterator = (Node *)iterator->next) {
      PageAndFrameNumber *data = (PageAndFrameNumber *)iterator->data;
      int thingsPageNum = data->pageNum;
      if(pageNumber == thingsPageNum) {
         return iterator;
      }
   }

   return NULL;
}

MemBlock *findMemBlock(unsigned int frameNumber) {
   
   if(LOGGER) {
      printf("===== FIND MEMBLOCK =====\n");
   }
   
   if(sizeOfMemBlock == 0) {
      return NULL;
   }
   
   MemBlock *iterator = startMemBlock;
   int i;
 /*  
   for(i = 0; iterator && i < frames; i++, iterator = (MemBlock  *)iterator->next) {
      int thingsPageNum = MemBlock->frameNum;
      if(frameNumber == thingsPageNum) {
         return iterator;
      }
   }
*/
   return NULL;
}
void makeNewTLBNode(unsigned int pageNumber, unsigned int frameNumber) {
   
   if(LOGGER) {
      printf("===== MAKE TLB =====\n");
   }


   Node *temp = (Node *)calloc(1, sizeof(Node));
   
   PageAndFrameNumber *pageFrame = (PageAndFrameNumber *)calloc(1, sizeof(PageAndFrameNumber));
   pageFrame->pageNum = pageNumber;
   pageFrame->frameNum = frameNumber;
   temp->data = pageFrame;
   
   if(startTLB == NULL) {
      startTLB = temp;
      endTLB = startTLB;
   }
   else {
      endTLB->next = temp;
      temp->prev = endTLB;
      endTLB = temp;
   }
   sizeOfTLB++;
}

void makeNewPageTableNode(unsigned int pageNumber) {
   if(LOGGER) {
      printf("===== MAKE PAGETABLE =====\n");
   }

   Node *temp = (Node *)calloc(1, sizeof(Node));
   
   PageAndFrameNumber *pageFrame = (PageAndFrameNumber *)calloc(1, sizeof(PageAndFrameNumber));
   pageFrame->pageNum = pageNumber;
   pageFrame->frameNum = frameNumberCounter;
   temp->data = pageFrame;
   
   if(startPageTable == NULL) {
      startPageTable = temp;
      endPageTable = startPageTable;
   }
   else {
      endPageTable->next = temp;
      temp->prev = endPageTable;
      endPageTable = temp;
   }

   
   sizeOfPageTable++;
}

void makeNewMemBlock(unsigned int page) {
   if(LOGGER) {
      printf("===== MAKE MEMBLOCK =====\n");
   }
   MemBlock *temp = (MemBlock *)calloc(1, sizeof(MemBlock));
   binaryFile = fopen("BACKING_STORE.bin", "r");
   
   temp->frameNum = frameNumberCounter++;
   
   fseek(binaryFile, page, SEEK_SET);
   fread(temp->data, sizeof(char), 256, binaryFile);
   
   if(startMemBlock == NULL) {
      startMemBlock = temp;
      endMemBlock = startMemBlock;
   }
   else {
      endMemBlock->next = temp;
      temp->prev = endMemBlock;
      endMemBlock = temp;
   }
   sizeOfMemBlock++;
}

void removeAndFreeStartNodeTLB() {
  
   Node *toFree = startTLB;
   startTLB = startTLB->next;
   
   free(toFree->data);
   free(toFree);
   sizeOfTLB--;
}

void removeAndFreeStartMemBlock() {
 // printf("HERE\n");
   MemBlock *toFree = startMemBlock;
 //  printf("StartMemBlock: %d\n", startMemBlock->frameNum);
   startMemBlock = startMemBlock->next;
   free(toFree);
   sizeOfMemBlock--;
}

void loadFrame(unsigned int page){
   
   if(sizeOfTLB < 16) {
      makeNewTLBNode(page, frameNumberCounter);
   }
   
   if(sizeOfPageTable < 256) {
      makeNewPageTableNode(page);
   }

   if(sizeOfMemBlock < frames) {
      makeNewMemBlock(page);
   }
}

void findByte(Node *inPageTable, unsigned int offset) {

   MemBlock *iterator = startMemBlock;
   for( ; iterator; iterator = iterator->next) {
      if(iterator->frameNum == inPageTable->data->frameNum) {
         break;
      }
   }
   
   printf("%d (%c)\n", iterator->frameNum, iterator->data[offset]);
}

void noReplacement() {
  int number = 0;
  

  fscanf (file, "%d", &number);    
  while (!feof (file)) { 
      //printf("Number: %d\n", number);
      //printf ("offset: %X\npageNumber: %X\n\n", offSetMask, pageNumberMask);
      //printf ("offset: %d\npageNumber: %d\n\n", number & offSetMask, number & pageNumberMask);
      unsigned int offset = number & offSetMask;
      unsigned int page = number & pageNumberMask;
      
      Node *inTLB = checkIfInTLB(page);
      if(inTLB) {
      tlbHits++;
         findByte(inTLB, offset);
      }
      else  {
      tlbMisses++;
         Node *inPageTable = checkIfInPageTable(page);
         
         if(!inPageTable) {
            pageMisses++;
            loadFrame(page);
            inPageTable = checkIfInPageTable(page);
         }
         else {
            pageHits++;
         }
         findByte(inPageTable, offset);
      }
      fscanf (file, "%d", &number);      
    }

}

void fifoReplacement() {
   int number = 0;
   fscanf (file, "%d", &number);    
   
   while (!feof (file)) { 
      unsigned int offset = number & offSetMask;
      unsigned int page = number & pageNumberMask;
      
      Node *inTLB = checkIfInTLB(page);
      
      if(inTLB) {
         tlbHits++;
         findByte(inTLB, offset);
      }
      else  {
         tlbMisses++;
         
         Node *inPageTable = checkIfInPageTable(page);
         
         if(!inPageTable) {
            pageMisses++;
            if(sizeOfTLB == 16) {
               removeAndFreeStartNodeTLB();
            }
            if(sizeOfMemBlock == frames) {
               removeAndFreeStartMemBlock();
            }  
            
            loadFrame(page);
            inPageTable = checkIfInPageTable(page);
         }
         else {
            pageHits++;
            
            if(sizeOfTLB == 16) {
               removeAndFreeStartNodeTLB();
            }
            makeNewTLBNode(page, inPageTable->data->frameNum);
            // if we have variable size memblock (not 256) then we are going to need to do somethin
            // similiar to what I did above I think...
            //makeNewMemBlock(inPageTable->data->frameNum) 
         }
         findByte(inPageTable, offset);
      }
      
      fscanf (file, "%d", &number); 
   }
}

void lruReplacement() {
 int number = 0;
   fscanf (file, "%d", &number);    
   
   while (!feof (file)) { 
      unsigned int offset = number & offSetMask;
      unsigned int page = number & pageNumberMask;
      
      Node *inTLB = checkIfInTLB(page);

      if(inTLB) {
      printf("=====123===\n");
         tlbHits++;
         inTLB->prev->next = inTLB->next;
         inTLB->next->prev = inTLB->prev;
         endTLB->next = inTLB;
         inTLB->prev = endTLB;
         endTLB = inTLB;
         
         //MemBlock *currentMemBlock = findMemBlock(); 
         printf("this is going to seg hard\n");
         currentMemBlock->prev->next = currentMemBlock->next;
         currentMemBlock->next->prev = currentMemBlock->prev;
         endMemBlock->next = currentMemBlock;
         currentMemBlock->prev = endMemBlock;
         endMemBlock = currentMemBlock;
      }
      else {
         tlbMisses++;
         printf("====0====\n");
         Node *inPageTable = checkIfInPageTable(page);
         
         if(!inPageTable) {
            pageMisses++;        
            printf("====1====\n");
            if(sizeOfTLB == 16) {
               removeAndFreeStartNodeTLB();
            }
            if(sizeOfMemBlock == frames) {
               removeAndFreeStartMemBlock();
            }  
            printf("====2====\n");
            loadFrame(page);
            inPageTable = checkIfInPageTable(page);
         }
         else {
            pageHits++;
            printf("====3====\n");
            if(sizeOfTLB == 16) {
               removeAndFreeStartNodeTLB();
            }
            makeNewTLBNode(page, inPageTable->data->frameNum);
            // if we have variable size memblock (not 256) then we are going to need to do somethin
            // similiar to what I did above I think...
            //makeNewMemBlock(inPageTable->data->frameNum) 
            printf("this is going to seg hard\n");
            currentMemBlock->prev->next = currentMemBlock->next;
            currentMemBlock->next->prev = currentMemBlock->prev;
            endMemBlock->next = currentMemBlock;
            currentMemBlock->prev = endMemBlock;
            endMemBlock = currentMemBlock;
         }
         findByte(inPageTable, offset);
      }
      
      fscanf (file, "%d", &number);   
      printPageTable();
   }
}

void optReplacement() {


}

void printPageTable() {

   MemBlock *iterator = startMemBlock;
   printf("==============\n");
   for(; iterator; iterator = iterator->next) {
      printf("FrameNumber: %d\n", iterator->frameNum);
   }
   printf("==============\n");
}

void printHitAndMisses() {
   printf("PAGEFAULT: %lf PAGEFAULT_RATE: %lf\n", pageHits, pageMisses / (pageHits + pageMisses));
   printf("TLBHITs: %lf TLBMISSES: %lf TLB_MISS_RATE: %lf\n", tlbHits, tlbMisses, tlbMisses / (tlbHits + tlbMisses));
}

int main(int argc, char **argv) 
{

   file = fopen(argv[1], "r");
   offSetMask = (1 << 8) - 1; 
   pageNumberMask = ((1 << 16) - 1) ^ offSetMask;

   frames = atoi(argv[2]);

   if(argc == 3) {
      noReplacement();
   }
   else if(!strcmp(argv[3], "fifo")) {
      fifoReplacement();
   }
   else if(!strcmp(argv[3], "lru")) {
      lruReplacement();
   }
   else if(!strcmp(argv[3], "opt")){
      optReplacement();
   }
   else {
      printf("NOT SUPPORTED\n");
      exit(0);
   }
   printHitAndMisses();
   fclose(file);
}
