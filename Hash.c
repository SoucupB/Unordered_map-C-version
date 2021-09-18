#include "Hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>

uint64_t genereateRandomBigNumber();
uint64_t insert(Hash self, uint64_t hash);
uint64_t insertInArray(Hash self, uint64_t *memBuffer, uint64_t hash);
Hash hash_InitWithSize(size_t size);
void redistribute(Hash *self);

Hash hash_Init() {
  Hash self = malloc(sizeof(struct Hash_t));
  self->hash = vct_Init(sizeof(uint64_t));
  self->randomNumbers = vct_Init(sizeof(uint64_t));
  self->indexes = vct_Init(sizeof(void **));
  self->indexesSizes = vct_Init(sizeof(size_t));
  self->size = 256;
  self->usedSpace = 0;
  self->ID = 0;
  for(size_t i = 0, c = 0; i < self->size; i++) {
    uint64_t number = 0;
    void *empty = NULL;
    vct_Push(self->hash, &number);
    vct_Push(self->indexes, &empty);
    vct_Push(self->indexesSizes, &c);
  }
  self->randomNumberSize = 128;
  for(size_t i = 0; i < self->randomNumberSize; i++) {
    uint64_t randomNumber = genereateRandomBigNumber();
    vct_Push(self->randomNumbers, &randomNumber);
  }
  return self;
}

Hash hash_InitWithSize(size_t newSize) {
  Hash self = malloc(sizeof(struct Hash_t));
  self->hash = vct_Init(sizeof(uint64_t));
  self->randomNumbers = vct_Init(sizeof(uint64_t));
  self->indexes = vct_Init(sizeof(void **));
  self->indexesSizes = vct_Init(sizeof(size_t));
  self->size = newSize;
  self->usedSpace = 0;
  self->ID = 0;
  for(size_t i = 0, c = 0; i < self->size; i++) {
    uint64_t number = 0;
    void *empty = NULL;
    vct_Push(self->hash, &number);
    vct_Push(self->indexes, &empty);
    vct_Push(self->indexesSizes, &c);
  }
  self->randomNumberSize = 128;
  for(size_t i = 0; i < self->randomNumberSize; i++) {
    uint64_t randomNumber = genereateRandomBigNumber();
    vct_Push(self->randomNumbers, &randomNumber);
  }
  return self;
}

uint64_t genereateRandomBigNumber() {
  uint8_t number[8] = {0};
  for(uint8_t i = 0; i < 7; i++) {
    number[i] = (rand() & 255);
  }
  return (((uint64_t *)number)[0] | 1ULL);
}

uint64_t getHashEquivalent(Hash self, void *buffer, uint32_t size) {
  uint64_t hashKey = 188546267763ULL;
  if(size >= self->randomNumberSize) {
    for(size_t i = self->randomNumberSize; i < size; i++) {
      uint64_t randomNumber = genereateRandomBigNumber();
      vct_Push(self->randomNumbers, &randomNumber);
    }
    self->randomNumberSize = size;
  }
  uint8_t *byteBuffer = buffer;
  uint64_t *hashNumber = self->randomNumbers->buffer;
  for(size_t i = 0; i < size; i++) {
    hashKey ^= (uint64_t)byteBuffer[i] * hashNumber[i];
  }
  return hashKey;
}


void *hash_GetValue(Hash self, void *key, size_t keySize) {
  uint64_t hash = getHashEquivalent(self, key, keySize);
  uint64_t *memBuffer = self->hash->buffer;
  void **valueBuffer = self->indexes->buffer;
  uint64_t naturalKey = hash % self->size;
  size_t currentIndex = 0;
  while(currentIndex < self->usedSpace && memBuffer[naturalKey] && memBuffer[naturalKey] != hash) {
    naturalKey = (naturalKey + 13) % self->size;
    currentIndex++;
  }

  if(memBuffer[naturalKey]) {
    return valueBuffer[naturalKey];
  }
  return NULL;
}

void hash_InsertHash(Hash self, uint64_t hash, void *value, size_t valueSize) {
  uint64_t *memBuffer = self->hash->buffer;
  void **valueBuffer = self->indexes->buffer;
  uint64_t naturalKey = hash % self->size;
  size_t *sizes = self->indexesSizes->buffer;
  while(memBuffer[naturalKey] && memBuffer[naturalKey] != hash) {
    naturalKey = (naturalKey + 13) % self->size;
  }
  if(!memBuffer[naturalKey]) {
    self->usedSpace++;
  }
  if(!memBuffer[naturalKey] || valueSize >= sizes[naturalKey]) {
    if(memBuffer[naturalKey]) {
      free(valueBuffer[naturalKey]);
    }
    valueBuffer[naturalKey] = malloc(valueSize);
    sizes[naturalKey] = valueSize;
  }
  memcpy(valueBuffer[naturalKey], value, valueSize);
  memBuffer[naturalKey] = hash;
}

void hash_Save(Hash *self, void *key, size_t keySize, void *value, size_t valueSize) {
  if((*self)->usedSpace >= (*self)->size) {
    redistribute(self);
    hash_Save(self, key, keySize, value, valueSize);
  }
  uint64_t hash = getHashEquivalent((*self), key, keySize);
  hash_InsertHash((*self), hash, value, valueSize);
}

void redistribute(Hash *self) {
  const size_t lastSize = (*self)->size << 3;
  Hash newHash = hash_InitWithSize(lastSize);
  vct_Delete(newHash->randomNumbers);
  newHash->randomNumbers = vct_Init(sizeof(uint64_t));
  Vector randNums = (*self)->randomNumbers;
  for(size_t i = 0, c = randNums->size; i < c; i++) {
    uint64_t currentValue = ((uint64_t *)(*self)->randomNumbers->buffer)[i];
    vct_Push(newHash->randomNumbers, &currentValue);
  }
  uint64_t *memBuffer = (*self)->hash->buffer;
  void **prevBuffer = (*self)->indexes->buffer;
  size_t *sizes = (*self)->indexesSizes->buffer;
  for(size_t i = 0, c = (*self)->indexes->size; i < c; i++) {
    if(prevBuffer[i]) {
      hash_InsertHash(newHash, memBuffer[i], prevBuffer[i], sizes[i]);
    }
  }
  hash_Delete((*self));
  (*self) = newHash;

}

Iter hash_Begin(Hash *self) {
  // void **beginKey = (*self)->hash->buffer;
  // void **beginValue = (*self)->hash->buffer;
  return NULL;
}

size_t hash_GetNumber(Hash *self, void *buffer, size_t size) {
  size_t *currentID = hash_GetValue(*self, buffer, size);
  if(currentID) {
    return *currentID;
  }
  size_t IDElement = (*self)->ID;
  hash_Save(self, buffer, size, &IDElement, sizeof(size_t));
  return (*self)->ID++;
}

void hash_Delete(Hash self) {
  void **valueBuffers = self->indexes->buffer;
  for(size_t i = 0, c = self->indexes->size; i < c; i++) {
    if(valueBuffers[i]) {
      free(valueBuffers[i]);
    }
  }
  vct_Delete(self->indexes);
  vct_Delete(self->hash);
  vct_Delete(self->randomNumbers);
  vct_Delete(self->indexesSizes);
  free(self);
}