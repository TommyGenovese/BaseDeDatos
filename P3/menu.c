#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

static int ShowMainMenu();
static void ClearInputBuffer();
static void UseFunc();
static void InsertFunc();
static void PrintFunc();

int USE_USED = 0;
char TABLE_TO_USE[BUFFLEN] = "";

/**
 * @brief Entry point, shows and process main menu.
 */
int main(void)
{
    int nChoice = 0;
    printf("Welcome to the database manager\n");
    do
    {
        nChoice = ShowMainMenu();
        switch (nChoice)
        {
        case 1:
            UseFunc();
            ClearInputBuffer();
            break;
        case 2:
            InsertFunc();
            ClearInputBuffer();
            break;
        case 3:
            PrintFunc();
            ClearInputBuffer();
            break;
        case 4:
            printf("Exiting the Program...\n\n");
            break;
        }
    } while (nChoice != 4);
    return 0;
}

/**
 * @brief prints main menu and allows to select an option.
 *
 * @return selected entry in the menu
 */
int ShowMainMenu()
{
    int nSelected = 0;
    char buf[16];
    char idx[BUFFLEN] = "DEFAULT";
    do
    {
        if (USE_USED == 0)
        {
            printf("\n\nRight now you have not selected a database\nPlease use the 'use' command for that\n");
        }
        else
        {
            replaceExtensionByIdx(TABLE_TO_USE, idx);
            printf("\n\nRight now you have selected the '%s' data and '%s' index files.\nUse again the 'use' command in order to change it for another one\n", TABLE_TO_USE, idx);
        }
        printf("\nSelect the routine you want to use: \n\n"
               " (1) use\n"
               " (2) insert\n"
               " (3) print\n"
               " (4) exit\n\n"
               "Enter a number that corresponds to your choice > ");
        if (!fgets(buf, sizeof(buf), stdin))
        {
            nSelected = 0;
        }
        else
        {
            nSelected = atoi(buf);
            if (nSelected < 1 || nSelected > 4)
            {
                printf("You have entered an invalid choice. Please try again\n\n");
            }
        }
        printf("\n");
    } while (nSelected < 1 || nSelected > 4);

    return nSelected;
}

/**
 * @brief Clears the input buffer.
 */
void ClearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void UseFunc()
{
    char insertedname[BUFFLEN] = "DEFAULT";
    int len = 0, opt = 0;
    printf("You have chosen the use command\nIntroduce the desired table name:\n");
    (void)fflush(stdout);
    (void)scanf("%s", insertedname);
    len = strlen(insertedname);
    if (len >= BUFFLEN)
    {
        return;
    }

    if ((insertedname[len - 4] != '.') || (insertedname[len - 3] != 'd') || (insertedname[len - 2] != 'a') || (insertedname[len - 1] != 't'))
    {
        do
        {
            printf("The name given does not contain the .dat extension, ¿would you like to add it?\n");
            printf(" (1) Yes, add it\n (2) No, return me to the previous menu\n");
            (void)fflush(stdout);
            (void)scanf("%d", &opt);
            if (opt < 1 || opt > 2)
            {
                printf("You have entered an invalid choice. You will be returned to the previous menu\n\n");
                return;
            }
            if (opt == 1)
            {
                if ((len + 4) < BUFFLEN)
                {
                    printf("Edited the name '%s' ", insertedname);
                    insertedname[len] = '.';
                    insertedname[len + 1] = 'd';
                    insertedname[len + 2] = 'a';
                    insertedname[len + 3] = 't';
                    printf("into '%s'\n", insertedname);
                }
                else
                {
                    printf("The requested name would be too big with the extension included,\nplease try again with a shorter one\n");
                    return;
                }
            }
            else if (opt == 2)
            {
                printf("Returning to previous menu\n\n");
                return;
            }
        } while (opt < 1 || opt > 2);
    }
    if (createTable(insertedname) != true)
    {
        printf("There has been an error creating either the table or the index\n");
        return;
    }
    printf("You have succesfully selected the %s table\n", insertedname);
    strcpy(TABLE_TO_USE, insertedname);
    USE_USED = 1;
    return;
}

void InsertFunc()
{
    char insertedkey[BUFFLEN] = "DEFAULT";
    char insertedtitle[BUFFLEN] = "DEFAULT";
    char indexname[BUFFLEN] = "DEFAULT";
    Book book;

    if (USE_USED == 0)
    {
        printf("No tables selected\nPlease select a table with the 'use' command before inserting elements\n");
        return;
    }

    printf("You have chosen the insert command\nIntroduce the name of the key you want to store:\n");
    (void)fflush(stdout);

    (void)scanf("%s", insertedkey);
    while (getchar() != '\n');

    if (strlen(insertedkey) > 4) {
        printf("Error: Book ID cannot be longer than 4 characters.\n");
        return;
    }

    printf("Ok, you have selected the key by the name %s,\nIntroduce the desired title to store:\n", insertedkey);
    (void)fflush(stdout);
    
    (void)fgets(insertedtitle, BUFFLEN, stdin);

    size_t len = strlen(insertedtitle);
    if (len > 0 && insertedtitle[len - 1] == '\n') {
        insertedtitle[len - 1] = '\0';
    }

    replaceExtensionByIdx(TABLE_TO_USE, indexname);

    book.title_len = strlen(insertedtitle);
    book.title = (char*)malloc((book.title_len + 1) * sizeof(char));
    if (book.title == NULL) {
        printf("Memory allocation failed for book.title\n");
        return;
    }  

    strcpy(book.book_id, insertedkey);
    strcpy(book.title, insertedtitle);
    book.title_len = strlen(insertedtitle);

    printf("Inserting new book %s with title %s (with a title length of %ld) into the database\n", 
           book.book_id, book.title, book.title_len);

    if (addTableEntry(&book, TABLE_TO_USE, indexname) == false)
    {
        printf("There has been an error inserting the new entry\n");
    }
    else
    {
        printf("Entry inserted successfully\n");
    }

    return;
}


void PrintFunc()
{
    int insertedlevel;
    char indexname[BUFFLEN] = "DEFAULT";
    if (USE_USED == 0)
    {
        printf("No tables selected\nPlease select a table with the 'use' command before printing the index\n");
        return;
    }
    printf("You have chosen the print command.\nIntroduce the desired level:\n");
    (void)fflush(stdout);
    (void)scanf("%d", &insertedlevel);
    if (insertedlevel < 0)
    {
        printf("You have introduced an invalid number.\nYour selected level must not be a negative\n");
        return;
    }
    replaceExtensionByIdx(TABLE_TO_USE, indexname);
    printf("\n\n");
    printTree(insertedlevel, indexname);
    return;
}