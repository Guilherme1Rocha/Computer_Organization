#ifndef L2CACHE_H
#define L2CACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "math.h"
#include "Cache.h"

#define L1_NUMBER_BLOCKS (uint32_t)(L1_SIZE/BLOCK_SIZE)
#define L2_NUMBER_BLOCKS (uint32_t)(L2_SIZE/BLOCK_SIZE)

#define TOTAL_OFFSET_BITS (uint32_t)(log2(BLOCK_SIZE))
#define WORD_OFFSET_BITS (uint32_t) (log2(WORD_SIZE))
#define INDEX_BITS_L1 (uint32_t)(log2(L1_NUMBER_BLOCKS))
#define INDEX_BITS_L2 (uint32_t)(log2(L2_NUMBER_BLOCKS))

void resetTime();

uint32_t getTime();

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** Cache *************************/

void initCache();
void accessL1(uint32_t, uint8_t *, uint32_t);
void accessL2(uint32_t, uint8_t *, uint32_t);

typedef struct CacheLine {
  uint8_t Valid;
  uint8_t Dirty;
  uint32_t Tag;
  uint8_t Data[BLOCK_SIZE];
} CacheLine;

typedef struct CacheL1 {
  uint32_t init;
  CacheLine line[L1_SIZE/BLOCK_SIZE];
} CacheL1;

typedef struct CacheL2 {
  uint32_t init;
  CacheLine line[L2_SIZE/BLOCK_SIZE];
} CacheL2;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif
