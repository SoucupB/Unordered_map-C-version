#pragma once
#include <stdint.h>
#include "Vector.h"

typedef struct Hash_t *Hash;
typedef struct Iter_t *Iter;

struct Hash_t {
  Vector hash;
  Vector indexes;
  Vector randomNumbers;
  Vector indexesSizes;
  size_t size;
  size_t randomNumberSize;
  size_t usedSpace;
  size_t ID;
  Hash *keys;
};

struct Iter_t {
  void *key;
  void *value;
};

Hash hash_Init();
uint32_t hash_GetNumberByString(void *buffer, size_t size);
size_t hash_GetNumber(Hash *self, void *buffer, size_t size);
uint64_t getHashEquivalent(Hash self, void *buffer, uint32_t size);
void hash_Save(Hash *self, void *key, size_t keySize, void *value, size_t valueSize);
void *hash_GetValue(Hash self, void *key, size_t keySize);
void hash_Delete(Hash self);

