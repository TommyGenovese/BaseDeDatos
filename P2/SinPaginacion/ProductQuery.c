#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int StockQuery(void);
int FindProductQuery(void);

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
    scanf("%s", productcode);
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

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT productcode, productname FROM products WHERE productname LIKE ? ORDER BY productcode;", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    printf("Enter productname > ");
    (void)fflush(stdout);
    scanf("%s", producttosearch);
    char searchTerm[BUFFER_LENGHT];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(searchTerm, sizeof(searchTerm), "%%%s%%", producttosearch); // Adding '%' so the query does not explode
#pragma GCC diagnostic pop

    // Now Ill bind the parameter with the % included
    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, searchTerm, (SQLINTEGER)strlen(searchTerm), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
    }

    if (SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR *)productname, BUFFER_LENGHT, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
    }
    if (SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR *)productcode, BUFFER_LENGHT, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 2\n");
    }

    printf("Fetching results from query...\n");
    /* Loop through the rows in the result-set */
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("productname = %s | productcode = %s\n", productname, productcode);
    }

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        printf("ERROR FETCHING RESULTS\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    ret2 = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret2))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }
    printf("\n");

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
