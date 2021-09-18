#pragma once
#include <stdlib.h>
#include <stdint.h>

struct Vector_t {
  void *buffer;
  size_t size;
  size_t capacity;
  size_t objSize;
};

typedef struct Vector_t *Vector;

Vector vct_Init(size_t size);
void vct_Push(Vector self, void *buffer);
void vct_Delete(Vector self);
Vector vct_DeepCopy(Vector self);
void vct_Pop(Vector self);
void *vct_Random(Vector self);
void vct_Shuffle(Vector self);
Vector vct_InitWithCap(size_t size, size_t count);
Vector vct_InitWithAllocator(size_t size, void **tempBuffer, size_t capacity);
void vct_Realloc(Vector self, size_t newSize);