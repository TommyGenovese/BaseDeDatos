/*
 * Created by roberto on 27/8/20.
 */

#ifndef CODE_V2_UTILS_H
#define CODE_V2_UTILS_H
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define NO_DELETED_REGISTERS -1

#define HEADER_SIZE 4
#define INDEX_HEADER_SIZE 8
#define DATA_HEADER_SIZE 4
#define PK_SIZE 4

#define BUFFLEN 512
/* Our table is going to contain a string (title) and
   an alphanumeric primary key (book_id)
*/
typedef struct book {
    char book_id[PK_SIZE]; /* primary key */
    size_t title_len; /* title length */
    char *title; /* string to be saved in the database */
} Book;

/* Note that in general a struct cannot be used to
   handle tables in databases since the table structure
   is unknown on compilation time.
*/

typedef struct node {
    char book_id[PK_SIZE];
    int left, right, parent, offset;
}Node;


/* Function prototypes.
   see function description in the following sections

   All function return true if success or false if failed
   except findKey which return true if register found
   and false otherwise.
   */

bool createTable(const char * tableName);
bool createIndex(const char * indexName);
bool findKey(const char * book_id, const char * indexName,
             int * nodeIDOrDataOffset);
bool addTableEntry(Book * book, const char * tableName,
                   const char * indexName);
bool addIndexEntry(char * book_id, int bookOffset, const char * indexName);

void printTree(size_t level, const char * indexName);

/* Other functions that you may find useful
 * change extension
 */
void replaceExtensionByIdx(const char * fileName, char * indexName);

#endif /* CODE_V2_UTILS_H */
