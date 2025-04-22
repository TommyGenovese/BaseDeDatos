/*
// Created by roberto on 30/8/20.
*/
#include "tester.h"

void checkAddIndexEntry(const char * indexName)
/**
 *  check AddIndexEntry
 */
{
    bool result;
    int nodeIDOrDataOffset;
    char bookId[4]="   ";
    int tmp=-1;
    FILE * fp;
    int offset;
    int deleted;
    createTestIndexFile(indexName);

    /* 1) add existing node should complain*/
    result = addIndexEntry(a[0].book_id, a[0].offset, indexName);
    if (result){
        fprintf(stderr,
                "Error in addIndexEntry, inserted existing key %s",
                a[0].book_id);
        exit(EXIT_FAILURE);
    }
    /* add a new node using a existing deleted node */
    printf("-----Original tree-------\n");
    printTree(4, indexName);
    memcpy(bookId, "VAR2", 4);
    offset = 123;
    deleted = 8;
    result = addIndexEntry(bookId, 123, indexName);

    findKey(bookId, indexName, &nodeIDOrDataOffset);

    printf("-----after adding VAR2-------\n");
    printTree(4, indexName);
    if (nodeIDOrDataOffset!=offset){
        fprintf(stderr,
                "Error in addIndexEntry, inserted offset should be %d"
                " but I get %d", offset, nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /* get first to integer shuld be 0 (root) and 8 (first deleted) */
    fp = fopen(indexName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=0){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be 0");
        exit(EXIT_FAILURE);
    }
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be %d"
                " but ir is %d", deleted, tmp);
        exit(EXIT_FAILURE);
    }

    /* add a new node using the last  existing deleted node */
    memcpy(bookId, "VAR3", 4);
    offset = 321;
    deleted = -1;
    result = addIndexEntry(bookId, offset, indexName);
    findKey(bookId, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR3-------\n");
    printTree(4, indexName);
    if (nodeIDOrDataOffset!=offset){
        fprintf(stderr,
                "Error in addIndexEntry, inserted offset should be %d"
                " but I get %d", offset, nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /* get first to integer shuld be 0 (root) and -1 (first deleted) */
    fp = fopen(indexName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=0){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be 0");
        exit(EXIT_FAILURE);
    }
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be %d"
                " but ir is %d", deleted, tmp);
        exit(EXIT_FAILURE);
    }

    /* add a new node by expanding file */
    memcpy(bookId, "VAR4", 4);
    offset = 678;
    deleted = -1;
    result = addIndexEntry(bookId, offset, indexName);
    findKey(bookId, indexName, &nodeIDOrDataOffset);
    printf("-----after adding VAR4-------\n");
    printTree(5, indexName);
    if (nodeIDOrDataOffset!=offset){
        fprintf(stderr,
                "Error in addIndexEntry, inserted offset should be %d"
                " but I get %d", offset, nodeIDOrDataOffset);
        exit(EXIT_FAILURE);
    }
    /* get first to integer shuld be 0 (root) and -1 (first deleted) */
    fp = fopen(indexName, "r");
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=0){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be 0");
        exit(EXIT_FAILURE);
    }
    fread(&tmp, sizeof(int), 1, fp);
    if (tmp!=deleted){
        fprintf(stderr,
                "Error in addIndexEntry, first int in file should be %d"
                " but ir is %d", deleted, tmp);
        exit(EXIT_FAILURE);
    }
    printf("* checkAddIndexEntry: OK\n");
}

