#include "ProductQuery.h"
#include "OrderQuery.h"
#include "CustomerQuery.h"

#include <stdio.h>
#include <stdlib.h>

static int ShowMainMenu();
static void ShowProductsMenu();
static int ShowProductsSubMenu();
static void ShowOrdersMenu();
static int ShowOrdersSubMenu();
static void ShowCustomersMenu();
static int ShowCustomersSubMenu();
static void ClearInputBuffer();

/**
 * @brief Entry point, shows and process main menu.
 */
int main(void)
{
    int nChoice = 0;
    printf("\n------------------------------------------\nREAD ME PLEASE\nYou are running the program with page switch enabled.\nProbably, the .sh tests wont work in this version.\nIf you wish to test the querys using them please use the other version instead\n------------------------------------------\n");
    do
    {
        nChoice = ShowMainMenu();
        switch (nChoice)
        {
        case 1:
            ShowProductsMenu();
            break;
        case 2:
            ShowOrdersMenu();
            break;
        case 3:
            ShowCustomersMenu();
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

    do
    {
        printf("\nSelect the menu you want to access: \n\n"
               " (1) Products\n"
               " (2) Orders\n"
               " (3) Customers\n"
               " (4) Exit\n\n"
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
 * @brief Shows and processes the products menu.
 */
void ShowProductsMenu()
{
    int nChoice = 0;
    int res = 0;
    do
    {
        nChoice = ShowProductsSubMenu();
        switch (nChoice)
        {
        case 1:
            res = StockQuery();
            ClearInputBuffer();
            break;
        case 2:
            res = FindProductQuery();
            ClearInputBuffer();
            break;
        case 3:
            printf("Returning to previous menu\n");

            break;
        }
        if (res == 1)
        {
            printf("An error has occurred while executing the query");
        }
    } while (nChoice != 3);
}

/**
 * @brief prints the products submenu and allows to select an option.
 *
 * @return selected option
 */
int ShowProductsSubMenu()
{
    int nSelected = 0;
    char buf[16];

    do
    {
        printf("\nSelect one of the following options: \n\n"
               " (1) Stock\n"
               " (2) Find\n"
               " (3) Back\n\n");
        printf("Enter a number that corresponds to your choice > ");
        if (!fgets(buf, sizeof(buf), stdin))
        {
            nSelected = 0;
        }
        else
        {
            nSelected = atoi(buf);
            if (nSelected < 1 || nSelected > 3)
            {
                printf("You have entered an invalid choice. Please try again\n\n");
            }
        }
        printf("\n");
    } while (nSelected < 1 || nSelected > 3);

    return nSelected;
}

/**
 * @brief Shows and processes the orders menu.
 */
void ShowOrdersMenu()
{
    int nChoice = 0;
    int res = 0;
    do
    {
        nChoice = ShowOrdersSubMenu();
        switch (nChoice)
        {
        case 1:
            res = OpenQuery();
            break;
        case 2:
            res = RangeQuery();
            break;
        case 3:
            res = DetailQuery();
            ClearInputBuffer();
            break;
        case 4:
            printf("Returning to previous menu\n");
            break;
        }
        if (res == 1)
        {
            printf("An error has occurred while executing the query");
        }
    } while (nChoice != 4);
}

/**
 * @brief prints the orders submenu and allows to select an option.
 *
 * @return selected option
 */
int ShowOrdersSubMenu()
{
    int nSelected = 0;
    char buf[16];

    do
    {
        printf("\nSelect one of the following options: \n\n"
               " (1) Open\n"
               " (2) Range\n"
               " (3) Detail\n"
               " (4) Back\n\n");
        printf("Enter a number that corresponds to your choice > ");
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
 * @brief Shows and processes the customers menu.
 */
void ShowCustomersMenu()
{
    int nChoice = 0;
    int res = 0;

    do
    {
        nChoice = ShowCustomersSubMenu();
        switch (nChoice)
        {
        case 1:
            res = FindCustomerQuery();
            ClearInputBuffer();
            break;
        case 2:
            res = ListProductsQuery();
            ClearInputBuffer();
            break;
        case 3:
            res = BalanceQuery();
            ClearInputBuffer();
            break;
        case 4:
            printf("Returning to previous menu\n");
            break;
        }
        if (res == 1)
        {
            printf("An error has occurred while executing the query");
        }
    } while (nChoice != 4);
}

/**
 * @brief prints the customers submenu and allows to select an option.
 *
 * @return selected option
 */
int ShowCustomersSubMenu()
{
    int nSelected = 0;
    char buf[16];

    do
    {
        printf("\nSelect one of the following options: \n\n"
               " (1) Find\n"
               " (2) List Products\n"
               " (3) Balance\n"
               " (4) Back\n\n");
        printf("Enter a number that corresponds to your choice > ");
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
    while ((c = getchar()) != '\n' && c != EOF);
}
