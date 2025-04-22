#include "utils.h"

int no_deleted_registers = NO_DELETED_REGISTERS;

void printnode(size_t _level, size_t level, FILE *indexFileHandler, int node_id, char side);

/*Fichero usa extension db*/
bool createTable(const char *tablename)
{
    FILE *binary_file;
    int size = -1;
    char ret[BUFFLEN] = "DEFAULT";
    char dummy[BUFFLEN] = "DEFAULT";
    int len = 0;
    len = strlen(tablename);

    if (len >= BUFFLEN)
    {
        return false;
    }

    strcpy(dummy, tablename);
    strcpy(ret, tablename);

    if ((ret[len - 3] != 'd') || (ret[len - 2] != 'a') || (ret[len - 1] != 't'))
    {
        return false;
    }

    binary_file = fopen(dummy, "ab+");

    if (binary_file == NULL)
    {
        return false;
    }

    fseek(binary_file, 0, SEEK_END);
    size = ftell(binary_file);

    if (size == 0)
    {
        if (fwrite(&no_deleted_registers, HEADER_SIZE, 1, binary_file) != 1)
        {
            fclose(binary_file);
            return false;
        }
    }

    if (createIndex(dummy) == false)
    {
        fclose(binary_file);
        return false;
    }

    fclose(binary_file);
    return true;
}

bool createIndex(const char *indexName)
{
    FILE *binary_file;
    int size = -1;
    char ret[BUFFLEN] = "DEFAULT";
    int len = 0;
    len = strlen(indexName);

    if (len >= BUFFLEN)
    {
        return false;
    }

    strcpy(ret, indexName);

    if ((ret[len - 3] != 'i') || (ret[len - 2] != 'd') || (ret[len - 1] != 'x'))
    {
        replaceExtensionByIdx(indexName, ret);
    }

    binary_file = fopen(ret, "ab+");

    if (binary_file != NULL)
    {
        fseek(binary_file, 0, SEEK_END);
        size = ftell(binary_file);
        if (size == 0)
        {
            size_t write_result = fwrite(&no_deleted_registers, sizeof(no_deleted_registers), 1, binary_file);
            if (write_result != 1)
            {
                fclose(binary_file);
                return false;
            }

            write_result = fwrite(&no_deleted_registers, sizeof(no_deleted_registers), 1, binary_file);
            if (write_result != 1)
            {
                fclose(binary_file);
                return false;
            }
        }
    }
    else
    {
        return false;
    }

    fclose(binary_file);
    return true;
}

void replaceExtensionByIdx(const char *fileName, char *indexName)
{
    /*
    strcpy(dummy,indexName);
    ret = strcat(dummy, ".idx");

    */
    int len = 0;
    len = strlen(fileName);
    strcpy(indexName, fileName);
    indexName[len - 3] = 'i';
    indexName[len - 2] = 'd';
    indexName[len - 1] = 'x';
    return;
}

/*
Here is a rough implementation of a bst search
we can not really test it yet as we lack the
functions to really insert data into our files
*/
/*Check imputs before menu call*/
void printTree(size_t level, const char *indexName)
{
    FILE *binary_file;
    char ret[BUFFLEN] = "DEFAULT";
    int len = 0, root = -1;
    Node node;
    /*Openning our index file*/

    strcpy(ret, indexName);
    len = strlen(indexName);
    if (len >= BUFFLEN)
    {
        return;
    }

    if ((ret[len - 3] != 'i') || (ret[len - 2] != 'd') || (ret[len - 1] != 'x'))
    {
        printf("The file extension is not the index one\n");
        return;
    }

    binary_file = fopen(ret, "rb+");
    if (binary_file == NULL)
    {
        printf("Error opening index file\n");
        return;
    }
    /*Going to file start to search for first node*/

    /*Format > MAR2 (0): 4*/
    /*We seek where the root (first pointer on the file) is located*/
    fseek(binary_file, 0, SEEK_SET);
    fread(&root, sizeof(int), 1, binary_file);
    if (root == -1)
    {
        printf("There is no root node, returning");
        return;
    }
    /*With the root position we can search that node*/
    fseek(binary_file, 8, SEEK_SET);
    fseek(binary_file, root * ((4 * sizeof(int)) + 4), SEEK_CUR);
    fread(&node, sizeof(Node), 1, binary_file);

    printf("%.4s (%d): %d\n", node.book_id, root, node.offset);

    if (level > 0)
    {
        if (node.left != -1)
        {
            printnode(1, level, binary_file, node.left, 'l');
        }
        if (node.right != -1)
        {
            printnode(1, level, binary_file, node.right, 'r');
        }
    }
    fclose(binary_file);
    return;
}

void printnode(size_t _level, size_t level, FILE *indexFileHandler, int node_id, char side)
{
    Node node;
    size_t i = 0;
    if (_level > level)
    {
        return;
    }
    fseek(indexFileHandler, 8, SEEK_SET);
    fseek(indexFileHandler, node_id * ((4 * sizeof(int)) + 4), SEEK_CUR);
    fread(&node, sizeof(Node), 1, indexFileHandler);

    for (; i < _level; i++)
    {
        printf("   ");
    }
    printf("%c %.4s (%d): %d\n", side, node.book_id, node_id, node.offset);
    if (node.left != -1)
    {
        printnode(_level + 1, level, indexFileHandler, node.left, 'l');
    }

    if (node.right != -1)
    {
        printnode(_level + 1, level, indexFileHandler, node.right, 'r');
    }

    return;
}

bool findKey(const char *book_id, const char *indexName, int *nodeIDOrDataOffset)
{
    FILE *binary_file = NULL;
    int root = -1;
    int ret;
    Node node;
    size_t offset = 0, r_size = 0;

    binary_file = fopen(indexName, "rb+");

    /*Reading root*/
    fseek(binary_file, 0, SEEK_SET);
    fread(&root, sizeof(int), 1, binary_file);
    if (root == -1)
    {
        (*nodeIDOrDataOffset) = root;
        return false;
    }
    r_size = sizeof(Node);
    *nodeIDOrDataOffset = root;

    while (true)
    {
        offset = (*nodeIDOrDataOffset) * r_size + INDEX_HEADER_SIZE;
        fseek(binary_file, offset, SEEK_SET);
        fread(&node, sizeof(Node), 1, binary_file);
        /*Check if we found the requested book*/
        ret = strncmp(book_id, node.book_id, PK_SIZE);
        if (ret == 0)
        {
            (*nodeIDOrDataOffset) = node.offset;
            fclose(binary_file);
            return true;
        }
        else if (ret < 0)
        {
            if (node.left == -1)
            {
                fclose(binary_file);
                return false;
            }
            else
            {
                (*nodeIDOrDataOffset) = node.left;
            }
        }
        else
        {
            if (node.right == -1)
            {
                fclose(binary_file);
                return false;
            }
            else
            {
                (*nodeIDOrDataOffset) = node.right;
            }
        }
    }
    return true;
}

/*For errors on the add functions, not in prev because it was implemented the last day*/
void handle_error(const char* msg, FILE* file) {
    if (file) fclose(file);
    printf("%s\n", msg);
}

bool addTableEntry(Book* book, const char* dataName, const char* indexName) {
    FILE* binary_file = fopen(dataName, "rb+");
    if (!binary_file) {
        perror("Error opening file");
        return false;
    }

    int current_offset = 0, next_offset = 0, prev_offset = 0, new_deleted_offset = 0;
    size_t deleted_size = 0;
    bool found_deleted = false;

    if (findKey(book->book_id, indexName, &current_offset)) {
        handle_error("The book id is already registered", binary_file);
        return false;
    }

    fseek(binary_file, 0, SEEK_SET);
    if (fread(&current_offset, sizeof(int), 1, binary_file) != 1) {
        handle_error("Error reading the initial offset.", binary_file);
        return false;
    }

    while (current_offset != NO_DELETED_REGISTERS && !found_deleted) {
        fseek(binary_file, current_offset, SEEK_SET);
        if (fread(&next_offset, sizeof(int), 1, binary_file) != 1 || fread(&deleted_size, sizeof(int), 1, binary_file) != 1) {
            handle_error("Error reading deleted record information.", binary_file);
            return false;
        }

        if (deleted_size >= book->title_len + PK_SIZE + sizeof(int)) {
            found_deleted = true;
        } else {
            prev_offset = current_offset;
            current_offset = next_offset;
        }
    }

    if (!found_deleted) {
        fseek(binary_file, 0, SEEK_END);
    } else {
        fseek(binary_file, current_offset, SEEK_SET);
    }

    current_offset = ftell(binary_file);

    if (fwrite(book->book_id, PK_SIZE, 1, binary_file) != 1 || fwrite(&book->title_len, sizeof(int), 1, binary_file) != 1 || fwrite(book->title, book->title_len, 1, binary_file) != 1) {
        handle_error("Error writing the book entry to the file.", binary_file);
        return false;
    }

    if (found_deleted) {
        new_deleted_offset = ftell(binary_file);
        size_t new_deleted_size = deleted_size - book->title_len - (PK_SIZE + sizeof(int));

        if (new_deleted_size > 0) {
            if (fwrite(&next_offset, sizeof(int), 1, binary_file) != 1 || fwrite(&new_deleted_size, sizeof(int), 1, binary_file) != 1) {
                handle_error("Error updating deleted entry information.", binary_file);
                return false;
            }
            fseek(binary_file, prev_offset, SEEK_SET);
            if (fwrite(&new_deleted_offset, sizeof(int), 1, binary_file) != 1) {
                handle_error("Error writing updated deleted entry information.", binary_file);
                return false;
            }
        } else {
            fseek(binary_file, prev_offset, SEEK_SET);
            if (fwrite(&next_offset, sizeof(int), 1, binary_file) != 1) {
                handle_error("Error writing updated deleted entry information.", binary_file);
                return false;
            }
        }
    }

    if (!addIndexEntry(book->book_id, current_offset, indexName)) {
        handle_error("Error updating the index file.", binary_file);
        return false;
    }

    fclose(binary_file);
    return true;
}


bool addIndexEntry(char *book_id, int bookOffset, const char *indexName) {
    int root, current, parent, cmp, ret, del, delaux, loop = 0, right = 2;
    FILE *indexFile = fopen(indexName, "rb+");
    Node newNode = {0};
    Node currentNode;

    if (!indexFile) {
        printf("Error opening index file");
        return false;
    }

    fseek(indexFile, 0, SEEK_SET);
    fread(&root, sizeof(int), 1, indexFile);
    strncpy(newNode.book_id, book_id, PK_SIZE);
    newNode.left = -1;
    newNode.right = -1;
    newNode.parent = -1;
    newNode.offset = bookOffset;

    if (root == -1) {
        root = 0;
        fseek(indexFile, 0, SEEK_SET);
        fwrite(&root, sizeof(int), 1, indexFile);
        fseek(indexFile, INDEX_HEADER_SIZE, SEEK_SET);
        fwrite(&newNode, sizeof(Node), 1, indexFile);
    } else {
        current = root;
        parent = -1;

        do {
            fseek(indexFile, current * sizeof(Node) + INDEX_HEADER_SIZE, SEEK_SET);
            fread(&currentNode, sizeof(Node), 1, indexFile);

            parent = current;

            cmp = strncmp(book_id, currentNode.book_id, PK_SIZE);
            if (cmp == 0) {
                /*Already exists a node with same id*/
                fclose(indexFile);
                return false;
            } else if (cmp < 0) {
                /*izquierda*/
                if (currentNode.left == -1) {
                    ret = ftell(indexFile) / sizeof(Node);
                    currentNode.left = ret;
                    newNode.parent = parent;
                    loop = 1;
                    right = 0;
                }
                if (loop == 0) {
                    current = currentNode.left;
                }
            } else if (cmp > 0) {
                /*derecha*/
                if (currentNode.right == -1) {
                    ret = ftell(indexFile) / sizeof(Node);
                    currentNode.right = ret;
                    newNode.parent = parent;
                    loop = 1;
                    right = 1;
                }
                if (loop == 0) {
                    current = currentNode.right;
                }
            }
        } while (loop == 0);

        /*We will seek a deleted entry to write on*/

        fseek(indexFile, 4, SEEK_SET);
        fread(&del, sizeof(int), 1, indexFile);

        if (del != -1) {
            fseek(indexFile, del * sizeof(Node) + INDEX_HEADER_SIZE, SEEK_SET);
            fseek(indexFile, HEADER_SIZE, SEEK_CUR);
            fread(&delaux, sizeof(int), 1, indexFile);

            /*Write on deleted location*/
            fseek(indexFile, del * sizeof(Node) + INDEX_HEADER_SIZE, SEEK_SET);
            fwrite(&newNode, sizeof(Node), 1, indexFile);

            /*Changing del pointer to next (or 0 if not)*/
            fseek(indexFile, 4, SEEK_SET);
            fwrite(&delaux, sizeof(int), 1, indexFile);
        } else {
            fseek(indexFile, 0, SEEK_END);
            del = ftell(indexFile) / sizeof(Node);
            fwrite(&newNode, sizeof(Node), 1, indexFile);
        }

        if (right == 1) {
            currentNode.right = del;
            fseek(indexFile, (current * sizeof(Node)) + INDEX_HEADER_SIZE, SEEK_SET);
            fwrite(&currentNode, sizeof(Node), 1, indexFile);
        } else if (right == 0) {
            currentNode.left = del;
            fseek(indexFile, (current * sizeof(Node)) + INDEX_HEADER_SIZE, SEEK_SET);
            fwrite(&currentNode, sizeof(Node), 1, indexFile);
        } else {
            return false;
        }

        fseek(indexFile, del * sizeof(Node) + INDEX_HEADER_SIZE, SEEK_SET);
        fread(&newNode, sizeof(Node), 1, indexFile);
    }

    fclose(indexFile);
    return true;
}
