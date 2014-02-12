#ifndef MEMSIM_H
#define MEMSIM_H

#define PAGE_TABLE_ENTRIES 256
#define TLB_TABLE_ENTRIES 16
#define PAGE_SIZE 256
#define FRAME_SIZE 256

typedef struct NEED_TO_THINK_OF_A_GOOD_NAME{
   int logicalMemoryPageNum;
   int physicalMemoryFrameNum;
} NEED_TO_THINK_OF_A_GOOD_NAME;


typedef struct Node{
   struct Node *prev;
   struct Node *next;
   struct NEED_TO_THINK_OF_A_GOOD_NAME *thing;
} Node;

void noReplacement();

void fifoReplacement();

void lruReplacement();

void optReplacement();
#endif
