/* Autor: Roberto Marabini */
/* This file implements a set of unitary and functional tests
 * (not necessarily complete) designed to test the
 * methods that you are required to implement in this assignment
 *
 * We recommend you to follow a Test-driven development (TDD)
 * that is, execute the first test, which will fail, then create the code
 * to fix the errors and rerun the tests. Once the first test is
 * satisfied and the requirements for the first method are met
 * uncomment the second test and repeat the process
 *
 * IMPORTANT: do NOT modify the code in this file
 * but you are welcomed to add new tests
 */

#include "tester.h"

void checkReplaceExtensionByIdx(const char * tableName,
                                const char * indexName);
void checkCreateIndex(const char * indexName);
void checkCreateTable(const char * tableName);

void checkPrint(const char * indexName);
void createTestIndexFile(const char * indexName);
void checkFindKey(const char * indexName);
void checkAddIndexEntry(const char * indexName);

Node a[TEST_ARRAY_SIZE];
char sort_a[SORTED_TEST_ARRAY_SIZE][PK_SIZE+1];
Book b[TEST_ARRAY_SIZE];


int main() {
    const char *tableName = "myDataBase.dat";
    const char *indexName = "myDataBase.idx";

    /* remove file if exists */
    remove(tableName);
    remove(indexName);

    /* Start testing */

    checkReplaceExtensionByIdx(tableName, indexName);
    checkCreateIndex(indexName);
    checkCreateTable(tableName);
    createTestFiles(tableName, indexName);
    checkPrint(indexName);
    checkFindKey(indexName);
    checkAddIndexEntry(indexName);


#ifdef NEVERDEFINED
#endif
    return (0);
}

