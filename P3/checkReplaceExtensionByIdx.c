/*
// Created by roberto on 29/8/20.
*/
#include "tester.h"

void checkReplaceExtensionByIdx(const char * tableName,
                                const char * indexName)
/**
 * Tester method for replaceExtensionByIdx which replaces
 * any 3 character extension by "idx"
 * This function is not required in the assignment but
 * I find it useful. If you do not want to implement it
 * just skip this test
 *
 * @param tableName: original filename
 * @param indexName: the tested function should return this value
 */
{
    char * outFileName;
    outFileName = (char *) malloc(strlen(tableName) + 1);
    replaceExtensionByIdx(tableName, outFileName);
    if (strcmp(outFileName, indexName) != 0) {
        printf("call to replaceExtensionByIdx failed\n");
        printf("'%s' != '%s'\n", outFileName, indexName);
        exit(1);
    }
    free(outFileName);
    printf("* replaceExtensionByIdx: OK\n");
}

