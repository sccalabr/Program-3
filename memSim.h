#ifndef MEMSIM_H
#define MEMSIM_H

#define PAGE_TABLE_ENTRIES 256
#define TLB_TABLE_ENTRIES 16
#define PAGE_SIZE 256
#define FRAME_SIZE 256

typedef struct PageAndFrameNumber{
   int pageNum;
   int frameNum;
} PageAndFrameNumber;


typedef struct Node{
   struct Node *prev;
   struct Node *next;
   struct PageAndFrameNumber *data;
} Node;

typedef struct MemBlock{
   int frameNum;
   char data[256];
   struct MemBlock *next;
   struct MemBlock *prev;
} MemBlock;

Node *checkIfInTLB(unsigned int pageNumber);

Node *checkIfInPageTable(unsigned int pageNumber);

void makeNewTLBNode(unsigned int pageNumber, unsigned int frameNumber);

void makeNewPageTableNode(unsigned int pageNumber);

void makeNewMemBlock(unsigned int page);

void removeAndFreeStartNodeTLB();

void removeAndFreeStartMemBlock();

void loadFrame(unsigned int page);

void findByte(Node *inPageTable, unsigned int offset);

void noReplacement();

void fifoReplacement();

void lruReplacement();

void optReplacement();
#endif
