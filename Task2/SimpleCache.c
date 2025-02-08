#include "SimpleCache.h"
#include "math.h" 

uint8_t L1CacheData[L1_SIZE];
uint8_t L2CacheData[L2_SIZE];
uint8_t DRAM[DRAM_SIZE];
uint32_t time;
CacheL1 Cache_L1;
CacheL2 Cache_L2;

#define TOTAL_OFFSET_BITS (uint32_t)(log2(BLOCK_SIZE))
#define WORD_OFFSET_BITS (uint32_t) (log2(WORD_SIZE))
#define INDEX_BITS_L1 (uint32_t)(log2(L1_NUMBER_BLOCKS))
#define INDEX_BITS_L2 (uint32_t)(log2(L2_NUMBER_BLOCKS))

//memcpy (dest, src, size to transfer)
//Byte addressable memory

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

  if (address >= DRAM_SIZE - WORD_SIZE + 1)
    exit(-1);

  if (mode == MODE_READ) {
    memcpy(data, &(DRAM[address]), BLOCK_SIZE);
    time += DRAM_READ_TIME;
  }

  if (mode == MODE_WRITE) {
    memcpy(&(DRAM[address]), data, BLOCK_SIZE);
    time += DRAM_WRITE_TIME;
  }
}

/*********************** L1 cache *************************/

void initCache() { 
  Cache_L1.init = 0;
  Cache_L2.init = 0; 
}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {

  uint32_t Index, Tag, MemAddress, Offset, Word_number, Tag_index;
  uint8_t TempBlock[BLOCK_SIZE];

  /* init cache */
  if (Cache_L1.init == 0) {
    int i;
    for (i = 0; i < L1_NUMBER_BLOCKS; i++){
      Cache_L1.line[i].Valid = 0;
      Cache_L1.line[i].Dirty = 0;
      Cache_L1.line[i].Tag = 0;
    }
    Cache_L1.init = 1;
  }

  Offset = address % BLOCK_SIZE;
  Tag_index = address >> TOTAL_OFFSET_BITS; 

  Index = Tag_index % L1_NUMBER_BLOCKS;
  Tag = Tag_index >> INDEX_BITS_L1;

  CacheLine * Line = &Cache_L1.line[Index];

  // MemAddress = address >> TOTAL_OFFSET_BITS;
  // MemAddress = MemAddress << TOTAL_OFFSET_BITS; // address of the block in memory

  // printf("Adress: %d\n", address);
  // printf("Offset: %d\n", Offset);
  // printf("index: %d\n", Index);
  // printf("Tag: %d\n", Tag);
  // printf("Tag_index: %d\n", Tag_index);
  // printf("MemAdress: %d\n", MemAddress);
  // printf("Line_Tag: %d\n", Line->Tag);
  // printf("------------------------------\n");

  /* access Cache*/
  if (!Line->Valid || Line->Tag != Tag) {         // if block not present - miss
    accessL2(address, TempBlock, MODE_READ); // get new block from L2
    
    if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
      MemAddress = ((Line->Tag << INDEX_BITS_L1) + Index) << TOTAL_OFFSET_BITS;    // get address of the block in memory
      accessL2(MemAddress, &(L1CacheData[Index * BLOCK_SIZE]), MODE_WRITE); // then write back old block
    }

    memcpy(&(L1CacheData[Index * BLOCK_SIZE]), TempBlock, BLOCK_SIZE); // copy new block to cache line
    Line->Valid = 1;
    Line->Tag = Tag;
    Line->Dirty = 0;
  } // if miss, then replaced with the correct block
  Word_number = Offset >> WORD_OFFSET_BITS;

  if (mode == MODE_READ) {    // read data from cache line
    memcpy(data, &(L1CacheData[Index * BLOCK_SIZE + Word_number * WORD_SIZE]), WORD_SIZE);
    time += L1_READ_TIME;
  }

  if (mode == MODE_WRITE) {   // write data from cache line
    memcpy(&(L1CacheData[Index * BLOCK_SIZE + Word_number * WORD_SIZE]), data, WORD_SIZE);
    time += L1_WRITE_TIME;
    Line->Dirty = 1;
  }
}
void accessL2(uint32_t address, uint8_t * data, uint32_t mode){ // access to L2 cache means the CPU couldn't find the data in L1 cache
  uint32_t Index, Tag, MemAddress, Offset, Word_number, Tag_index;
  uint8_t TempBlock[BLOCK_SIZE];

  /* init cache */
  if (Cache_L2.init == 0) {
    int i;
    for (i = 0; i < L2_NUMBER_BLOCKS; i++){
      Cache_L2.line[i].Valid = 0;
      Cache_L2.line[i].Dirty = 0;
      Cache_L2.line[i].Tag = 0;
    }
    Cache_L2.init = 1;
  }

  Offset = address % BLOCK_SIZE;
  Tag_index = address >> TOTAL_OFFSET_BITS; 

  Index = Tag_index % L2_NUMBER_BLOCKS;
  Tag = Tag_index >> INDEX_BITS_L2;

  CacheLine * Line = &Cache_L2.line[Index];

  MemAddress = address >> TOTAL_OFFSET_BITS;
  MemAddress = MemAddress << TOTAL_OFFSET_BITS;
  if (!Line->Valid || Line->Tag != Tag) {         // if block not present - miss
    accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM
    
    if ((Line->Valid) && (Line->Dirty)) { // line has dirty block
      MemAddress = ((Line->Tag << INDEX_BITS_L2) + Index) << TOTAL_OFFSET_BITS;    // get address of the block in memory
      accessDRAM(MemAddress, &(L2CacheData[Index * BLOCK_SIZE]), MODE_WRITE); // then write back old block
    }

    memcpy(&(L2CacheData[Index * BLOCK_SIZE]), TempBlock, BLOCK_SIZE); // copy new block to cache line
    Line->Valid = 1;
    Line->Tag = Tag;
    Line->Dirty = 0;
  } // if miss, then replaced with the correct block

  Word_number = Offset >> WORD_OFFSET_BITS;

  if (mode == MODE_READ) {    // read data from cache line
    memcpy(data, &(L2CacheData[Index * BLOCK_SIZE + Word_number * WORD_SIZE]), WORD_SIZE);
    time += L2_READ_TIME;
  }

  if (mode == MODE_WRITE) {   // write data from cache line
    memcpy(&(L2CacheData[Index * BLOCK_SIZE + Word_number * WORD_SIZE]), data, WORD_SIZE);
    time += L2_WRITE_TIME;
    Line->Dirty = 1;
  }

}
void read(uint32_t address, uint8_t * data) {
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t * data) {
  accessL1(address, data, MODE_WRITE);
}
