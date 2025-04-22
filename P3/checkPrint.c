/*
// Created by roberto on 29/8/20.
*/

#include "tester.h"

void checkPrint(const char * indexName)
/**
 * check print method
 *
 * @param indexName
 */
{
    FILE * fp;
    int i =0;
    char buffer[128];
    char pk[PK_SIZE+1];
    /* create test index file */
    createTestIndexFile(indexName);
    /*printf("\n");*/
    /* redirect stdout to file
     * we want to catch printTree output
     * so we may validate it
     * */
    fp = fopen("file.txt", "w");
    SwapIOB(stdout, fp);
    printTree(4, indexName);
    /* stop redirection */
    SwapIOB(stdout, fp);
    fclose(fp);
    fp = fopen("file.txt", "r");
    for (i=0; i<SORTED_TEST_ARRAY_SIZE; i++) {
        /* check if deleted node */
        fgets(buffer, sizeof(buffer), fp);
        if (strstr(buffer, sort_a[i]) == NULL){
            printf("checkPrint line '%s' does not contain prinary key %s\n",
                   buffer, pk);
            exit(1);
        }
    }
    fclose(fp);
    printf("* checkcreateTablecreateIndex: OK\n");
}
