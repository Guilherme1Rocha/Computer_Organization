#ifndef SIMPLECACHE_H
#define SIMPLECACHE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

#define L1_NUMBER_BLOCKS (uint32_t)(L1_SIZE / BLOCK_SIZE)
#define OFFSET_BITS (uint32_t)(log2(BLOCK_SIZE))
#define BYTE_OFFSET_BITS (uint8_t) (log2(WORD_SIZE))
#define INDEX_BITS (uint32_t)(log2(L1_NUMBER_BLOCKS))

void resetTime();

uint32_t getTime();

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** Cache *************************/

void initCache();
void accessL1(uint32_t, uint8_t *, uint32_t);

typedef struct CacheLine {
  uint8_t Valid;
  uint8_t Dirty;
  uint32_t Tag;
  uint8_t Data[BLOCK_SIZE];
} CacheLine;

typedef struct Cache {
  uint32_t init;
  CacheLine line[L1_SIZE/BLOCK_SIZE];
} Cache;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif