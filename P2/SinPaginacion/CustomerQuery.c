#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define BUFFER_LENGTH 512

int FindCustomerQuery(void);
int ListProductsQuery(void);
int BalanceQuery(void);

int FindCustomerQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    char insertedname[BUFFER_LENGTH] = "\0";
    int customernumber = 0;
    char customername[BUFFER_LENGTH] = "\0";
    char customerfirstname[BUFFER_LENGTH] = "\0";
    char customerlastname[BUFFER_LENGTH] = "\0";

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT c.customernumber, c.customername, c.contactfirstname, c.contactlastname FROM customers c WHERE (c.contactfirstname LIKE ?) OR (c.contactlastname LIKE ?)", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    printf("Enter customer name > ");
    (void)fflush(stdout);
    (void)scanf("%s", insertedname);
    char searchTerm[BUFFER_LENGTH];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    (void)snprintf(searchTerm, sizeof(searchTerm), "%%%s%%", insertedname); // Adding '%' so the query does not explode
#pragma GCC diagnostic pop
    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, searchTerm, (SQLINTEGER)strlen(searchTerm), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }
    ret = SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, searchTerm, (SQLINTEGER)strlen(searchTerm), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
    }

    if (SQLBindCol(stmt, 1, SQL_C_SSHORT, &customernumber, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
    }
    if (SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR *)customername, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 2\n");
    }
    if (SQLBindCol(stmt, 3, SQL_C_CHAR, (SQLCHAR *)customerfirstname, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 3\n");
    }
    if (SQLBindCol(stmt, 4, SQL_C_CHAR, (SQLCHAR *)customerlastname, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 4\n");
    }

    printf("Fetching results from query...\n");
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("%d %s %s %s\n", customernumber, customername, customerfirstname, customerlastname);
    }

    (void)SQLCloseCursor(stmt);
    (void)SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    (void)odbc_disconnect(env, dbc);
    return EXIT_SUCCESS;
}

int ListProductsQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    int customernumber = 0;
    char productname[BUFFER_LENGTH] = "\0";
    char totalamount[BUFFER_LENGTH] = "\0";

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
        odbc_extract_error("", dbc, SQL_HANDLE_DBC);
        return ret;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT p.productname, SUM(od.quantityordered) AS total_amount FROM orderdetails od JOIN products p ON p.productcode = od.productcode JOIN orders o ON od.ordernumber = o.ordernumber WHERE o.customernumber = ? GROUP BY p.productcode ORDER BY p.productcode ", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    printf("Enter customer number > ");
    (void)fflush(stdout);
    (void)scanf("%d", &customernumber);

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &customernumber, sizeof(customernumber), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    if (SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR *)productname, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
    }
    if (SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR *)totalamount, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 2\n");
    }

    printf("Fetching results from query...\n");
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("%s %s\n", productname, totalamount);
    }

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        printf("ERROR FETCHING RESULTS\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    (void)SQLCloseCursor(stmt);
    (void)SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    (void)odbc_disconnect(env, dbc);
    return EXIT_SUCCESS;
}

int BalanceQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    int customernumber = 0;
    float balance = 0;

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
        odbc_extract_error("", dbc, SQL_HANDLE_DBC);
        return ret;
    }

    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT (SELECT SUM(p.amount) FROM payments p WHERE p.customernumber = ?) - (SELECT SUM(od.priceeach * od.quantityordered) FROM orders o JOIN orderdetails od ON o.ordernumber = od.ordernumber WHERE o.customernumber = ?) AS balance;", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    printf("Enter customer number > ");
    (void)fflush(stdout);
    (void)scanf("%d", &customernumber);

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &customernumber, sizeof(customernumber), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }
    ret = SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &customernumber, sizeof(customernumber), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return EXIT_FAILURE;
    }

    if (SQLBindCol(stmt, 1, SQL_C_FLOAT, &balance, sizeof(balance), NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
    }

    printf("Fetching results from query...\n");
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("Balance = %.2f\n", balance);
    }

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        printf("ERROR FETCHING RESULTS\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    (void)SQLCloseCursor(stmt);
    (void)SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    (void)odbc_disconnect(env, dbc);
    return EXIT_SUCCESS;
}