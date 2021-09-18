#include "Test.h"
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

uint64_t func_Time() {
  struct timespec _t;
  clock_gettime(CLOCK_REALTIME, &_t);
  return _t.tv_sec * 1000ULL + lround(_t.tv_nsec / 1.0e6);
}

void test_Hash_GetNumber() {
  Hash hash = hash_Init();
  const size_t sze = 500000;
  for(size_t i = 0; i < sze; i++) {
    char *randomStr = createRandomString(rand() % 50 + 1);
    hash_GetNumber(&hash, randomStr, strlen(randomStr));
    free(randomStr);
  }
  for(size_t i = 0; i < sze; i++) {
    char *randomStr = createRandomString(rand() % 50 + 1);
    size_t totalSize = strlen(randomStr);
    size_t firstNr = hash_GetNumber(&hash, randomStr, totalSize);
    size_t secondNr = hash_GetNumber(&hash, randomStr, totalSize);
    if(firstNr != secondNr) {
      printf("Wrong data for %s is %I64d and %s is %I64d!", randomStr,
             firstNr, randomStr, secondNr);
      exit(0);
    }
    free(randomStr);
  }
  hash_Delete(hash);
  printf("Test test_Hash_GetNumber passed!\n");
}

void test_Hash_Save() {
  Hash hash = hash_Init();
  const size_t sze = 515500;
  for(size_t i = 0; i < sze; i++) {
    char *randomStr = createRandomString(rand() % 50 + 1);
    char *rds = createRandomString(rand() % 50 + 1);
    const size_t f = strlen(randomStr);
    const size_t g = strlen(rds);
    hash_Save(&hash, randomStr, f, rds, g);
    char crt[512] = {0};
    memcpy(crt, hash_GetValue(hash, randomStr, f), g);
    if(strcmp(crt, rds)) {
      printf("Hash is different for %s and %s at index %I64d", crt, rds, sze);
      exit(0);
    }
    free(randomStr);
  }
  hash_Delete(hash);
  printf("Test test_Hash_Save passed!\n");
}

void test_Hash_SaveBigNumber() {
  Hash hash = hash_Init();
  const size_t sze = 500000;
  for(size_t i = 0; i < sze; i++) {
    uint64_t randomKey = (uint64_t)rand() * (uint64_t)rand() * (uint64_t)rand();
    uint64_t randomValue = (uint64_t)rand() * (uint64_t)rand() * (uint64_t)rand();
    hash_Save(&hash, &randomKey, sizeof(uint64_t), &randomValue, sizeof(uint64_t));
    uint64_t *respBuffer = hash_GetValue(hash, &randomKey, sizeof(uint64_t));
    uint64_t respNumber = 0;
    if(respBuffer) {
      respNumber = *respBuffer;
    }
    if(respNumber != randomValue) {
      printf("Hash is different for %I64d and %I64d at %I64d and pointer at %p", respNumber, randomValue, i, respBuffer);
      exit(0);
    }
  }
  hash_Delete(hash);
  printf("Test test_Hash_SaveBigNumber passed!\n");
}

void test_Hash_Timer() {
  Hash hash = hash_Init();
  const size_t sze = 500000;
  uint64_t startTime = func_Time();
  for(size_t i = 0; i < sze; i++) {
    uint64_t randomKey = (uint64_t)rand() * (uint64_t)rand() * (uint64_t)rand();
    uint64_t randomValue = (uint64_t)rand() * (uint64_t)rand() * (uint64_t)rand();
    hash_Save(&hash, &randomKey, sizeof(uint64_t), &randomValue, sizeof(uint64_t));
    uint64_t *respBuffer = hash_GetValue(hash, &randomKey, sizeof(uint64_t));
    uint64_t respNumber = 0;
    if(respBuffer) {
      respNumber = *respBuffer;
    }
    if(respNumber != randomValue) {
      printf("Hash is different for %I64d and %I64d at %I64d and pointer at %p", respNumber, randomValue, i, respBuffer);
      exit(0);
    }
  }
  printf("It took %I64d miliseconds for %I64d tests with 8 bytes key and 8 bytes value\n", func_Time() - startTime, sze);
  hash_Delete(hash);
  printf("Test test_Hash_Timer passed!\n");
}


char *createRandomString(size_t size) {
  char *response = malloc((size + 1) * sizeof(char));
  memset(response, 0, (size + 1) * sizeof(char));
  for(size_t i = 0; i < size; i++) {
    response[i] = rand() % 25 + 'a';
  }
  return response;
}