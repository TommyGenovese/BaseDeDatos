#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include <stdio.h>

#define BUFFER_LENGHT 512

/*
StockQuery func
Obtains quantity in stock of a product via its productcode
*/
int StockQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret;        /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    char productcode[BUFFER_LENGHT] = "\0";
    int quantityinstock = 0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT quantityinstock FROM products WHERE productcode = ?;", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    printf("Enter productcode > ");
    (void)fflush(stdout);
    (void)scanf("%s", productcode);
    (void)SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_C_CHAR, 0, 0, productcode, 0, NULL);
    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
    }
    if (SQLBindCol(stmt, 1, SQL_C_SSHORT, &quantityinstock, BUFFER_LENGHT, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMNS\n");
    }

    // UNCOMMENT NEXT LINE TO PRINT THE QUERY
    // printf("%s\n", query);

    printf("Fetching results from query...\n");
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("quantityinstock = %d\n", quantityinstock);
    }

    // IMPORTANTE
    ret2 = SQLCloseCursor(stmt);

    if (!SQL_SUCCEEDED(ret2))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }
    printf("\n");

    /* free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/*
FindQuery func
Obtains name and code of a product with a shared string
*/
int FindProductQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    int ret;        /* odbc.c */
    SQLRETURN ret2; /* ODBC API return status */
    char producttosearch[BUFFER_LENGHT] = "\0";
    char productname[BUFFER_LENGHT] = "\0";
    char productcode[BUFFER_LENGHT] = "\0";
    int page_size = 10;  // Results per page
    int current_offset = 0;  // current page
    char user_input;
    int ret_rows;  // Returned rows

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    printf("Enter productname > ");
    (void)fflush(stdout);
    (void)scanf("%s", producttosearch);
    char searchTerm[BUFFER_LENGHT];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    (void)snprintf(searchTerm, sizeof(searchTerm), "%%%s%%", producttosearch); // Adding '%' so the query does not explode
#pragma GCC diagnostic pop

    // loop for pages
    while (1)
    {
        // Binding both limit and offset to allow to change page via statements
        ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT productname, productcode FROM products WHERE productname LIKE ? ORDER BY productcode LIMIT ? OFFSET ?;", SQL_NTS);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_ENV);
            return ret;
        }

        // Vincular el parámetro de searchTerm (product name search)
        ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, searchTerm, (SQLINTEGER)strlen(searchTerm), NULL);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

        // Binding limit to restrict the results per page
        ret = SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &page_size, 0, NULL);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

        // Binding offset witn the number of rows to jump
        ret = SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &current_offset, 0, NULL);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

        // Executing query
        ret = SQLExecute(stmt);
        if (ret != SQL_SUCCESS)
        {
            printf("ERROR EXECUTING QUERY\n");
            return EXIT_FAILURE;
        }

        // Binding returned vals
        ret = SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR *)productname, BUFFER_LENGHT, NULL);
        if (ret != SQL_SUCCESS)
        {
            printf("ERROR WHILE BINDING COLUMN 1\n");
            return EXIT_FAILURE;
        }

        ret = SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR *)productcode, BUFFER_LENGHT, NULL);
        if (ret != SQL_SUCCESS)
        {
            printf("ERROR WHILE BINDING COLUMN 2\n");
            return EXIT_FAILURE;
        }

        // print results
        printf("Fetching results from query...\n");
        ret_rows = 0;
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
        {
            printf("productname = %s | productcode = %s\n", productname, productcode);
            ret_rows++;
        }

        if (ret_rows == 0)
        {
            printf("No more results.\n");
        }

        // prompt with instructions for the user
        printf("\nPress '+' to go to the next page, '-' to go to the previous page, or 'q' to quit.\n");
        user_input = getchar();
        user_input = getchar();

        if (user_input == '+')
        {
            current_offset += page_size; 
        }
        else if (user_input == '-')
        {
            if (current_offset > 0)
            {
                current_offset -= page_size; 
            }
            else
            {
                printf("You are already at the first page.\n");
            }
        }
        else if (user_input == 'q')
        {
            break;  
        }
        else
        {
            printf("Invalid input. Please try again.\n");
        }

         //Reseting cursor
        ret = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    }

    /* Free up statement handle */
    ret2 = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret2))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    /* DISCONNECT */
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
