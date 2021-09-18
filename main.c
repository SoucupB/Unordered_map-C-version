#include <stdio.h>
#include "Hash.h"
#include <string.h>
#include "Test.h"

void runTests() {
  test_Hash_GetNumber();
  test_Hash_Save();
  test_Hash_SaveBigNumber();
  test_Hash_Timer();
}

void runExample() {
  Hash hash = hash_Init();
  uint32_t key = 15;
  uint32_t value = 114;
  hash_Save(&hash, &key, sizeof(uint32_t), &value, sizeof(uint32_t)); // saving key -> value in hashmap; int to int
  uint32_t *ret = hash_GetValue(hash, &key, sizeof(uint32_t)); // returns a pointer to the value structure
  if(ret) {
    printf("Value for key %d is %d\n", key, ret[0]);
  }
  else {
    printf("Value for key %d not found\n", key);
  }
  key = 16;
  ret = hash_GetValue(hash, &key, sizeof(uint32_t));
  if(ret) {
    printf("Value for key %d is %d\n", key, ret[0]);
  }
  else {
    printf("Value for key %d not found\n", key);
  }
  uint32_t fkey = 25;
  float fvalue = 1.54226;
  hash_Save(&hash, &fkey, sizeof(uint32_t), &fvalue, sizeof(float)); // saving key -> value in hashmap; int -> float
  float *fret = hash_GetValue(hash, &fkey, sizeof(uint32_t)); // returns a pointer to the value structure
  if(fret) {
    printf("Value for key %d is %f\n", fkey, fret[0]);
  }
  else {
    printf("Value for key %d not found\n", fkey);
  }
  fkey = 53;
  fret = hash_GetValue(hash, &fkey, sizeof(uint32_t));
  if(fret) {
    printf("Value for key %d is %f\n", fkey, fret[0]);
  }
  else {
    printf("Value for key %d not found\n", fkey);
  }
  char *cKey = "Some hash key";
  char *cValue = "Some hash value";
  size_t keySize = strlen(cKey) * sizeof(char);
  size_t valueSize = strlen(cValue) * sizeof(char);
  hash_Save(&hash, cKey, keySize, cValue, valueSize); // saving key -> value in hashmap char array -> char array
  char *cRet = hash_GetValue(hash, cKey, keySize); // careful here since this value byte stream is not null terminated for the char array.
  char nullTermString[256] = {0};
  if(cRet) {
    memcpy(nullTermString, cRet, valueSize);
    printf("Value for key '%s' is '%s'\n", cKey, nullTermString);
  }
  else {
    printf("Value for key '%s' not found\n", cKey);
  }
  cKey = "AAF";
  keySize = strlen(cKey) * sizeof(char);
  cRet = hash_GetValue(hash, cKey, keySize);
  if(cRet) {
    memcpy(nullTermString, cRet, valueSize);
    printf("Value for key '%s' is '%s'\n", cKey, nullTermString);
  }
  else {
    printf("Value for key '%s' not found\n", cKey);
  }
}

void iterationTest() {
  for(Iter i = NULL; i != NULL; i++) {

  }
}

int main(int argc, char *argv[]) {
  if(argc == 2 && strcmp(argv[1], "-r=tests") == 0) {
    runTests();
  }
  else {
    runExample();
  }
}