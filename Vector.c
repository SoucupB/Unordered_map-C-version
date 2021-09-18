#include "Vector.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Vector vct_Init(size_t size) {
  Vector self = malloc(sizeof(struct Vector_t));
  self->buffer = malloc(size);
  memset(self->buffer, 0, size);
  self->size = 0;
  self->capacity = 1;
  self->objSize = size;
  return self;
}

Vector vct_InitWithCap(size_t size, size_t count) {
  Vector self = malloc(sizeof(struct Vector_t));
  self->buffer = malloc(size * count);
  memset(self->buffer, 0, size * count);
  self->size = 0;
  self->capacity = count;
  self->objSize = size;
  return self;
}

void copyData(Vector self, void *buffer) {
  memcpy(self->buffer + (self->size * self->objSize), buffer, self->objSize);
  self->size++;
}

void vct_Push(Vector self, void *buffer) {
  if(self->size >= self->capacity) {
    self->capacity <<= 2;
    void *newBuffer = malloc(self->capacity * self->objSize);
    memset(newBuffer, 0, self->capacity * self->objSize);
    memcpy(newBuffer, self->buffer, self->size * self->objSize);
    free(self->buffer);
    self->buffer = newBuffer;
  }
  copyData(self, buffer);
}

void vct_Delete(Vector self) {
  free(self->buffer);
  free(self);
}

void vct_Pop(Vector self) {
  self->size--;
}

void *vct_Random(Vector self) {
  size_t randomIndex = ((size_t)rand() * (size_t)rand()) % self->size;
  return (uint8_t *)self->buffer + (randomIndex * self->objSize);
}

size_t getRandomNumber(size_t a, size_t b) { // [a, b)
  return ((size_t)rand() * (size_t)rand()) % (b - a) + a;
}

void vct_Shuffle(Vector self) {
  const size_t size = self->objSize;
  uint8_t temp[size];
  for(size_t i = 0; i < self->size; i++) {
    size_t index = getRandomNumber(i, self->size);
    uint8_t *src = (uint8_t *)self->buffer + (i * self->objSize);
    uint8_t *dest = (uint8_t *)self->buffer + (index * self->objSize);
    memcpy(temp, src, self->objSize);
    memcpy(src, dest, self->objSize);
    memcpy(dest, temp, self->objSize);
  }
}

void vct_Realloc(Vector self, size_t newSize) {
  const size_t realNewSize = (newSize * self->objSize) * 2;
  void *newBuffer = malloc(realNewSize);
  memset(newBuffer, 0, realNewSize);
  memcpy(newBuffer, self->buffer, self->size * self->objSize);
  self->capacity = newSize * 2;
  self->size = newSize;
  free(self->buffer);
  self->buffer = newBuffer;
}

Vector vct_DeepCopy(Vector self) {
  Vector new = malloc(sizeof(struct Vector_t));
  new->capacity = self->capacity;
  new->objSize = self->objSize;
  new->size = self->size;
  new->buffer = malloc(new->capacity * new->objSize);
  memcpy(new->buffer, self->buffer, self->size * self->objSize);
  return new;
}