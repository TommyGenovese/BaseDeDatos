#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define BUFFER_LENGTH 512

int OpenQuery(void);
int RangeQuery(void);
int DetailQuery(void);

int OpenQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    int ordernumber = 0;

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT o.ordernumber FROM orders o WHERE o.shippeddate IS null ORDER BY o.ordernumber", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
    }

    if (SQLBindCol(stmt, 1, SQL_C_LONG, &ordernumber, 0, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMNS\n");
    }

    printf("Fetching results from query...\n");
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("ordernumber = %d\n", ordernumber);
    }

    (void)SQLCloseCursor(stmt);
    (void)SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    (void)odbc_disconnect(env, dbc);
    return EXIT_SUCCESS;
}

int RangeQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    char dummy[BUFFER_LENGTH] = "\0";
    char inserteddate1[BUFFER_LENGTH] = "\0";
    char inserteddate2[BUFFER_LENGTH] = "\0";
    DATE_STRUCT orderdate;
    DATE_STRUCT shippeddate;
    int ordernumber = 0;

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

    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT o.ordernumber, o.orderdate, o.shippeddate FROM orders o WHERE o.orderdate BETWEEN ? AND ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void)fflush(stdout);
    (void)fgets(dummy, BUFFER_LENGTH, stdin);
    (void)sscanf(dummy, "%s - %s", inserteddate1, inserteddate2); // Here im obtaining both dates with sscanf, it could also be done with one char* and strtok

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, inserteddate1, (SQLINTEGER)strlen(inserteddate1), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    ret = SQLBindParameter(stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_VARCHAR, 0, 0, inserteddate2, (SQLINTEGER)strlen(inserteddate2), NULL);
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

    if (SQLBindCol(stmt, 1, SQL_C_LONG, &ordernumber, 0, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
        return EXIT_FAILURE;
    }
    if (SQLBindCol(stmt, 2, SQL_C_TYPE_DATE, &orderdate, sizeof(orderdate), NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 2\n");
        return EXIT_FAILURE;
    }
    if (SQLBindCol(stmt, 3, SQL_C_TYPE_DATE, &shippeddate, sizeof(shippeddate), NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 3\n");
        return EXIT_FAILURE;
    }

    printf("Fetching results from query...\n");
    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("%d %d-%02d-%02d %d-%02d-%02d\n", ordernumber, orderdate.year, orderdate.month, orderdate.day, shippeddate.year, shippeddate.month, shippeddate.day);
    }

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        printf("ERROR FETCHING RESULTS\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    odbc_disconnect(env, dbc);
    return EXIT_SUCCESS;
}

int DetailQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    int ordernumber = 0;

    printf("Enter ordernumber > ");
    fflush(stdout);
    scanf("%d", &ordernumber);

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    DATE_STRUCT orderdate;
    char status[BUFFER_LENGTH] = "\0";

    /* Allocate a statement handle */
    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT o.orderdate, o.status FROM orders o WHERE o.ordernumber = ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ordernumber, sizeof(ordernumber), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
    }

    if (SQLBindCol(stmt, 1, SQL_C_DATE, &orderdate, sizeof(orderdate), NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
    }
    if (SQLBindCol(stmt, 2, SQL_C_CHAR, (SQLCHAR *)status, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 2\n");
    }

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("%d-%02d-%02d %s\n", orderdate.year, orderdate.month, orderdate.day, status);
    }

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        printf("ERROR FETCHING RESULTS\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    // SECOND QUERY
    int totalamount = 0;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT SUM(od.priceeach * od.quantityordered) AS total_amount FROM orderdetails od WHERE od.ordernumber = ?", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ordernumber, sizeof(ordernumber), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
    }

    if (SQLBindCol(stmt, 1, SQL_C_LONG, &totalamount, sizeof(totalamount), NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
    }

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("%d\n", totalamount);
    }

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        printf("ERROR FETCHING RESULTS\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);

    // THIRD QUERY
    char productcode[BUFFER_LENGTH] = "\0";
    int quantityordered = 0;
    float priceeach = 0;

    ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
    ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT od.productcode, od.quantityordered, od.priceeach FROM orderdetails od WHERE od.ordernumber = ? ORDER BY od.orderlinenumber ", SQL_NTS);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_ENV);
        return ret;
    }

    ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &ordernumber, sizeof(ordernumber), NULL);
    if (!SQL_SUCCEEDED(ret))
    {
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
        return ret;
    }

    if (SQLExecute(stmt) != SQL_SUCCESS)
    {
        printf("ERROR EXECUTING QUERY\n");
    }

    if (SQLBindCol(stmt, 1, SQL_C_CHAR, (SQLCHAR *)productcode, BUFFER_LENGTH, NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 1\n");
    }
    if (SQLBindCol(stmt, 2, SQL_C_LONG, &quantityordered, sizeof(quantityordered), NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 2\n");
    }
    if (SQLBindCol(stmt, 3, SQL_C_FLOAT, &priceeach, sizeof(priceeach), NULL) != SQL_SUCCESS)
    {
        printf("ERROR WHILE BINDING COLUMN 3\n");
    }

    while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
    {
        printf("%s %d %.2f\n", productcode, quantityordered, priceeach);
    }

    if (ret != SQL_NO_DATA && !SQL_SUCCEEDED(ret))
    {
        printf("ERROR FETCHING RESULTS\n");
        odbc_extract_error("", stmt, SQL_HANDLE_STMT);
    }

    SQLCloseCursor(stmt);
    SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    odbc_disconnect(env, dbc);
    return EXIT_SUCCESS;
}