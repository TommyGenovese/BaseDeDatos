/*
// Created by roberto on 29/8/20.
*/

#include "tester.h"
void fillTesterArray2()
/** Fill test array with the following index
 * Binary tree
 * Structure file:
   MAR2 (0): 423
   | l MAR0 (2): 51
   |    | l BAR0 (5): 89
   |    |   | r CAR5 (9): 232
   |    | r MAR1 (6): 33
   | r VAR1 (3): 29
        | l PAR2 (7): 90
        |   | r RAR2 (10): 250
        | r WAR3 (1): 47
 */
{
    Node *node;
    Book *book;
    size_t length = -1;
    char bookid[5];
    char title[64];
    size_t offset = 1 * sizeof(int);

    /* node 0 */
    node = &(a[0]);
    book = &(b[0]);
    strcpy(bookid, "BAR1");
    strcpy(title, "Zalacain el aventurero");
    memcpy(node->book_id, bookid, PK_SIZE);
    length = strlen(title);
    node->left = -1;
    node->right = -1;
    node->parent = 2;
    node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = length;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 1 */
    node = &(a[1]);
    book = &(b[1]);
    strcpy(bookid, "GAR1");
    strcpy(title, "Poema del cante jondo");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left = -1;
    node->right = -1;
    node->parent = 2;
    node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 2 */
    node = &(a[2]);
    book = &(b[2]);
    strcpy(bookid, "BAR2");
    strcpy(title, "La busca");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left = 0;
    node->right = 1;
    node->parent = -1;
    node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;
}

void fillTesterArray()
/** Fill test array with the following index
 * Binary tree
 * Structure file:
   MAR2 (0): 423
   | l MAR0 (2): 51
   |    | l BAR0 (5): 89
   |    |   | r CAR5 (9): 232
   |    | r MAR1 (6): 33
   | r VAR1 (3): 29
        | l PAR2 (7): 90
        |   | r RAR2 (10): 250
        | r WAR3 (1): 47
 */
{   Node * node;
    Book * book;
    size_t length = -1;
    char bookid[5];
    char title[64];
    size_t offset = 1 * sizeof(int);

    /* node 0 */
    node = &(a[0]); book = &(b[0]);
    strcpy(bookid, "MAR2");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_zero");
    memcpy(node->book_id, bookid, PK_SIZE);
    length = strlen(title);
    node->left =  2; node->right =  3; node->parent = -1;  node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = length;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 1 */
    node = &(a[1]); book = &(b[1]);
    strcpy(bookid, "WAR3");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_one");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left = -1; node->right = -1; node->parent =  3;  node->offset =  offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 2 */
    node = &(a[2]); book = &(b[2]);
    strcpy(bookid, "MAR0");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_two");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  5; node->right =  6; node->parent =  0;  node->offset =  offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 3 */
    node = &(a[3]); book = &(b[3]);
    strcpy(bookid, "VAR1");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_three");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  7; node->right =  1; node->parent =  0;  node->offset =  offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* node 4 is deleted, 8 next deleted node, 32 size deleted register */
    node = &(a[4]); book = &(b[4]);
    strcpy(bookid, "    ");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_four");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  8; node->right =  -1; node->parent =  -1;  node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 5 */
    node = &(a[5]); book = &(b[5]);
    strcpy(bookid, "BAR0");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_five");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  -1; node->right =  9; node->parent =  2;  node->offset =   offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 6 */
    node = &(a[6]); book = &(b[6]);
    strcpy(bookid, "MAR1");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_six");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  -1; node->right =  -1; node->parent =  2;  node->offset =  offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 7 */
    node = &(a[7]); book = &(b[7]);
    strcpy(bookid, "PAR2");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_seven");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  -1; node->right =  10; node->parent =  3;  node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 8 deleted, 40 size deleted register */
    node = &(a[8]); book = &(b[8]);
    strcpy(bookid, "    ");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_eigth");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  -1; node->right =  -1; node->parent =  -1;  node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 9 */
    node = &(a[9]); book = &(b[9]);
    strcpy(bookid, "CAR5");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_nine");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  -1; node->right =  -1; node->parent =  5;  node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    /* write node 10 */
    node = &(a[10]); book = &(b[10]);
    strcpy(bookid, "RAR2");
    strcpy(title, bookid); strcat(title + PK_SIZE, "_ten");
    memcpy(node->book_id, bookid, PK_SIZE);
    node->left =  -1; node->right =  -1; node->parent =  7;  node->offset = offset;
    memcpy(book->book_id, bookid, PK_SIZE);
    book->title_len = strlen(title);
    length = book->title_len;
    book->title = (char *) malloc(length);
    memcpy(book->title, title, length);
    offset += PK_SIZE + sizeof(int) + length;

    strcpy(sort_a[0], "MAR2");
    strcpy(sort_a[1], "MAR0");
    strcpy(sort_a[2], "BAR0");
    strcpy(sort_a[3], "CAR5");
    strcpy(sort_a[4], "MAR1");
    strcpy(sort_a[5], "VAR1");
    strcpy(sort_a[6], "PAR2");
    strcpy(sort_a[7], "RAR2");
    strcpy(sort_a[8], "WAR3");

}

void createTestIndexFile(const char * indexName)
/**
 * create test index ile
 * save test nodes a in a file
 * @param indexName: name of created file
 *
 */
{
    FILE * indexFileHandler;
    int node_id=0;
    int i=0;
    int chainDeletedNodes = 4;
    indexFileHandler = fopen(indexName, "w+");
    /* write header */
    fwrite(&node_id, sizeof(int), 1, indexFileHandler);
    fwrite(&chainDeletedNodes, sizeof(int), 1, indexFileHandler);
    /* init array of nodes with tree */
    fillTesterArray();
    for(i=0; i< TEST_ARRAY_SIZE; i++)
        fwrite(&(a[i]), sizeof(Node), 1, indexFileHandler);
    fclose(indexFileHandler);
}

void createTestDataFile(const char * dataName)
/**
 * create test index ile
 * save test nodes a in a file
 * @param indexName: name of created file
 *
 */
{
    FILE * indexFileHandler;
    int i=0;
    int chainDeletedNodes = -1;
    int endChain = -1;
    indexFileHandler = fopen(dataName, "w+");
    /* write header */
    /* init array of nodes with tree */
    fillTesterArray();
    chainDeletedNodes = a[4].offset;
    fwrite(&chainDeletedNodes, sizeof(int), 1, indexFileHandler);
    for(i=0; i< TEST_ARRAY_SIZE; i++) {
        if (i==4) /* offset to next deleted node */
            fwrite(&(a[8].offset), sizeof(int), 1, indexFileHandler);
        else if (i==8) /* offset to next deleted node */
            fwrite(&(endChain), sizeof(int), 1, indexFileHandler);
        else
            fwrite(&(b[i].book_id), sizeof(int), 1, indexFileHandler);
        fwrite(&(b[i].title_len), sizeof(int), 1, indexFileHandler);
        fwrite(b[i].title, b[i].title_len, 1, indexFileHandler);
    }
    fclose(indexFileHandler);
}

void createTestFiles(const char * tableName,
                     const  char * indexName)
 /**
 *  create data and index test file
 * @param tableName name of data file
 * @param indexName name od index file
 */
{
    createTestDataFile(tableName);
    createTestIndexFile(indexName);
}

void SwapIOB(FILE *A, FILE *B)
/**
 * swap file descriptors.
 * USeful to redirect stdout to a file
 * Be careful since this function is unreliable and
 * may not be portable.
 * @param A first pointer to file
 * @param B second pointer to file
 */
{
    FILE temp;
    /* make a copy of IOB A (usually this is "stdout") */
    memcpy(&temp, A, sizeof(FILE));
    /* copy IOB B to A's location, now any output
       sent to A is redirected thru B's IOB. */
    memcpy(A, B, sizeof(FILE));
    /* copy A into B, the swap is complete */
    memcpy(B, &temp, sizeof(FILE));
}  /* end SwapIOB; */

