/*
// Created by roberto on 29/8/20.
*/

#ifndef CODE_V2_TESTER_H
#define CODE_V2_TESTER_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "utils.h"

void createTestIndexFile(const char * indexName);
void createTestDataFile(const char * dataName);
void createTestFiles(const char * tableName,
                     const  char * indexName);

void SwapIOB(FILE *A, FILE *B);
void fillTesterArray();
/* array 9 nodes for testing */
#define TEST_ARRAY_SIZE 11
#define SORTED_TEST_ARRAY_SIZE 9
extern Node a[TEST_ARRAY_SIZE];
extern char sort_a[SORTED_TEST_ARRAY_SIZE][PK_SIZE+1];
extern Book b[TEST_ARRAY_SIZE];
#endif /* CODE_V2_TESTER_H */
