/*
// Created by roberto on 29/8/20.
*/

#include "tester.h"

void checkCreateIndex(const char * indexName)
/**
 *  Tester method for createIndex which initializes
 *  a index over a file to store data registers
 * @param tableName: file in which data will be stores
 * @param indexName: program structure that stores
 * a point to the tableName file
*/
{
    int num = 0;
    int i = 0;
    struct stat st;
    FILE * indexFileHandler;
    /* call createTable which calls createIndex*/
    createIndex(indexName);
    /* check file index has been created */
    /* and first TWO stored number are -1 */
    indexFileHandler = fopen(indexName, "r");
    for (i = 0; i < 2; i++) {
        fread(&num, sizeof(int), 1, indexFileHandler);
        if (num != -1) {
            printf("call to checkcreateTablecreateIndex failed\n");
            printf("the %d value stored in the file "
                   "should be '-1' but is '%d'\n", i, num);
            exit(1);
        }
        num = 0;
    }
    /* check file size */
    stat(indexName, &st);
    if (st.st_size!= (2*sizeof(int)))
    {
        printf("call to createTable failed\n");
        printf("the size of file %s should by %ld but it is %ld\n",
               indexName, 2 * sizeof(int), st.st_size);
        exit(1);
    }
    printf("* checkcreateTablecreateIndex: OK\n");
    fclose(indexFileHandler);
}


