/*
// Created by roberto on 29/8/20.
*/

#include "tester.h"

void checkFindKey(char * indexName)
/** test FindKey protocol
 * first create a indexfile and the search in it.
 *
 * @param indexFileHandler, file handler to indexFileHandler
 */
{
    int nodeIDOrDataOffset;
    int i=0;
    int result=-1;
    char book_id[5];
    book_id[4] = '\0';
    /* create test index file */
    createTestIndexFile(indexName);
    /* create index file */
    for(i=0; i<TEST_ARRAY_SIZE; i++)
    {
        if(strncmp(a[i].book_id, "    ", PK_SIZE)==0)
            continue;
        result = findKey(a[i].book_id, indexName, &nodeIDOrDataOffset);
        if (a[i].offset != nodeIDOrDataOffset) {
            fprintf(stderr, "checkFindKey failed offset %d !=%d\n",
                    a[i].offset, nodeIDOrDataOffset);
            exit(1);
        }
        if (result != 1){
            fprintf(stderr, "checkFindKey failed, result !=1\n");
            exit(1);
        }
    }
    /* MAR3 -- offset 7 */
    memcpy(book_id, "MAR3", PK_SIZE);
    result = findKey(book_id, indexName, &nodeIDOrDataOffset);
    if (7 != nodeIDOrDataOffset)
    {
        printf("checkFindKey failed parent node %d !=%d",
               7, nodeIDOrDataOffset);
        exit(1);
    }
    /* QQQQ -- nodo 10 */
    memcpy(book_id, "QQQQ", PK_SIZE);
    result = findKey(book_id, indexName, &nodeIDOrDataOffset);
    if (10 != nodeIDOrDataOffset)
    {
        printf("checkFindKey failed parent node %d !=%d",
               10, nodeIDOrDataOffset);
        exit(1);
    }
    printf("* checkFindKey: OK\n");
}