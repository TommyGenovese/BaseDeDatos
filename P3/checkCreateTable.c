/*
 Created by roberto on 29/8/20.
 */
#include "tester.h"

void checkCreateTable(const char * tableName)
/**
 *  Tester method for createTable which initializes
 *  a file to store data registers
 * @param tableName: file in which data will be stores
 * @param tableName: program structure that stores
 * a point to the tableName file
*/
{
    int num = 0;
    struct stat st;
    FILE * dataFileHandler;
    /* call createTable */
    createTable(tableName);

    /* check file tableName has been created */
    /* and first stored number if -1 */
    dataFileHandler = fopen(tableName, "r");
    fread(&num, sizeof(int), 1, dataFileHandler);
    if (num != -1){
        printf("call to createTable failed\n");
        printf("the first value stored in the file "
               "should be '-1' but is '%d'\n", num);
        exit(1);
    }
    /* check file size */
    stat(tableName, &st);
    if (st.st_size!=sizeof(int))
    {
        printf("call to createTable failed\n");
        printf("the size of file %s should by %ld\n", tableName, sizeof(int));
        exit(1);
    }
    printf("* checkcreateTable: OK\n");
    fclose(dataFileHandler);
}
