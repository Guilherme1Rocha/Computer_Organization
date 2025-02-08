#include "L2ACache.h"

uint8_t L1CacheData[L1_SIZE];
uint8_t L2CacheData[L2_SIZE];
uint8_t DRAM[DRAM_SIZE];
uint32_t time;
CacheL1 Cache_L1;
CacheL2 Cache_L2;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {
  /*
  Simulates access to main memory (DRAM) updating a global time variable 
  according to the requested mode;

  @param address    The byte address of the memory location being accessed
  @param *data:     A pointer to the data that will be read from or written to the cache
  @param mode       Indicates the access mode, either MODE_READ or MODE_WRITE

  @note The function will terminate with an exit code of -1 if the address is out of bounds
  */

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
  /*
  Simulates access to a L1 Cache (directly-mapped). Checks if the requested data is in the cache
  handling misses and implementing a Write Back policy in case of a dirty Block; 
  updates a global time variable according to the requested mode;

  @param address    The byte address of the memory location being accessed
  @param *data:     A pointer to the data that will be read from or written to the cache
  @param mode       Indicates the access mode, either MODE_READ or MODE_WRITE
  */
  uint32_t Index, Tag, Offset, Word_number, Tag_index;
  uint8_t TempBlock[BLOCK_SIZE];

  /* init cache */
  if (Cache_L1.init == 0) {
    for (uint32_t i = 0; i < L1_NUMBER_BLOCKS; i++){
      Cache_L1.Line[i].Valid = 0;
      Cache_L1.Line[i].Dirty = 0;
      Cache_L1.Line[i].Tag = 0;
    }
    Cache_L1.init = 1;
  }

  /*Taking the necessary information from the address: Tag, Index and Offset*/
  Offset = address % BLOCK_SIZE;
  Tag_index = address >> TOTAL_OFFSET_BITS; 

  Index = Tag_index % L1_NUMBER_BLOCKS;
  Tag = Tag_index >> INDEX_BITS_L1;

  CacheLine * Line = &Cache_L1.Line[Index]; // identifying the cache line with the Index
  
  /* access Cache*/
  if (!Line->Valid || Line->Tag != Tag) {    // if block not present - miss

    accessL2(address, TempBlock, MODE_READ); // get new block from L2 Cache
  
    if ((Line->Valid) && (Line->Dirty)) { // Line has dirty block

      /*get address of the block in the cache, then write back old block*/
      accessL2(((Line->Tag << INDEX_BITS_L1) + Index) << TOTAL_OFFSET_BITS, Line->Data, MODE_WRITE); // then write back old block
    }
    memcpy(Line->Data, TempBlock, BLOCK_SIZE); // copy new block to cache Line
    Line->Valid = 1;
    Line->Tag = Tag;
    Line->Dirty = 0;
  } // if miss, then replaced with the correct block
  Word_number = Offset >> WORD_OFFSET_BITS;

  if (mode == MODE_READ) {    // read data from cache line
    memcpy(data, &(Line->Data[Word_number * WORD_SIZE]), WORD_SIZE);
    time += L1_READ_TIME;
  }

  if (mode == MODE_WRITE) {   // write data from cache Line
    memcpy(&(Line->Data[Word_number * WORD_SIZE]), data, WORD_SIZE);
    time += L1_WRITE_TIME;
    Line->Dirty = 1;  // Inferior levels of memory are not updated yet
  }
}

/*********************** L2 cache *************************/
void accessL2(uint32_t address, uint8_t * data, uint32_t mode){
  /*
  Simulates access to a L2 Cache (2-Way Associative). Checks if the requested data is in the cache
  handling misses, implementing a Write Back policy in case of a dirty Block and LRU policy in case of full sets;
  updates a global time variable according to the requested mode;

  @param address    The byte address of the memory location being accessed
  @param *data:     A pointer to the data that will be read from or written to the cache
  @param mode       Indicates the access mode, either MODE_READ or MODE_WRITE
  */
  uint32_t Index, Tag, MemAddress, Offset, Word_number, Tag_index;
  uint8_t TempBlock[BLOCK_SIZE];
  /* init cache */
  if (Cache_L2.init == 0) {
    for (uint32_t i = 0; i < L2_NUMBER_BLOCKS / N_WAY_ASSOC; i++){
        Cache_L2.sets[i].Line[LINE1].Valid = 0;
        Cache_L2.sets[i].Line[LINE1].Dirty = 0;
        Cache_L2.sets[i].Line[LINE1].Tag = 0;

        Cache_L2.sets[i].Line[LINE2].Valid = 0;
        Cache_L2.sets[i].Line[LINE2].Dirty = 0;
        Cache_L2.sets[i].Line[LINE2].Tag = 0;
    }
    Cache_L2.init = 1;
  }

  /*Taking the necessary information from the address: Tag, Index and Offset*/
  Offset = address % BLOCK_SIZE;
  Tag_index = address >> TOTAL_OFFSET_BITS; 

  Index = Tag_index % (L2_NUMBER_BLOCKS / N_WAY_ASSOC); // number of sets
  Tag = Tag_index >> INDEX_BITS_L2;

  Set * Set = &Cache_L2.sets[Index]; // identifying the Set with the Index

  MemAddress = address >> TOTAL_OFFSET_BITS;
  MemAddress = MemAddress << TOTAL_OFFSET_BITS;

  uint8_t Set_Block = 0;

  if (((!Set->Line[LINE1].Valid) || ((!Set->Line[LINE1].Tag) != Tag)) && ((!Set->Line[LINE2].Valid) || ((!Set->Line[LINE2].Tag) != Tag))){
    // if block not present in both blocks within the set - miss
    accessDRAM(MemAddress, TempBlock, MODE_READ); // get new block from DRAM

    //If there's an invalid line: use it
    if (!Set->Line[LINE1].Valid) Set_Block = 0;
    else if (!Set->Line[LINE2].Valid) Set_Block = 1;

    //In case that every line is invalid apply LRU policy
    if (Set->Line[LINE1].Valid && Set->Line[LINE2].Valid)
      Set_Block = !Set->Recent_Block;

    if ((Set->Line[Set_Block].Valid) && (Set->Line[Set_Block].Dirty)) { // Line has dirty block
      /*get address of the block in memory then write back old block*/
      accessDRAM(((Set->Line[Set_Block].Tag << INDEX_BITS_L2) + Index) << TOTAL_OFFSET_BITS, Set->Line[Set_Block].Data, MODE_WRITE);
    }
    memcpy(Set->Line[Set_Block].Data, TempBlock, BLOCK_SIZE); // copy new block to cache Line
    Set->Line[Set_Block].Valid = 1;
    Set->Line[Set_Block].Tag = Tag;
    Set ->Line[Set_Block].Dirty = 0;
  } // if miss, then replaced with the correct block
  Word_number = Offset >> WORD_OFFSET_BITS;

  if (Set->Line[LINE1].Tag == Tag) Set_Block = LINE1;
  else Set_Block = LINE2;
  

  if (mode == MODE_READ) {    // read data from cache Line
    memcpy(data, &(Set->Line[Set_Block].Data[Word_number * WORD_SIZE]), WORD_SIZE);
    Set->Recent_Block = Set_Block;
    time += L2_READ_TIME;
  }

  if (mode == MODE_WRITE) {   // write data from cache Line
    memcpy(&(Set->Line[Set_Block].Data[Word_number * WORD_SIZE]), data, WORD_SIZE);
    time += L2_WRITE_TIME;
    Set->Line[Set_Block].Dirty = 1; // Inferior levels of memory are not updated yet
  }
  Set->Recent_Block = Set_Block;
}
void read(uint32_t address, uint8_t * data) {
  /*performs a read operation from the cache*/
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t * data) {
  /*performs a write operation from the cache*/
  accessL1(address, data, MODE_WRITE);
}
