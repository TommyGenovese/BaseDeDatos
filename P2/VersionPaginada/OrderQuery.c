#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define BUFFER_LENGTH 512

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

    ret = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    ret = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    return EXIT_SUCCESS;
}

int RangeQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret, ret2;
    char dummy[BUFFER_LENGTH] = "\0";
    char inserteddate1[BUFFER_LENGTH] = "\0";
    char inserteddate2[BUFFER_LENGTH] = "\0";
    DATE_STRUCT orderdate;
    DATE_STRUCT shippeddate;
    int ordernumber = 0;
    int page_size = 10;
    int current_offset = 0;
    char user_input;
    int ret_rows;

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

    printf("Enter dates (YYYY-MM-DD - YYYY-MM-DD) > ");
    (void)fflush(stdout);
    (void)fgets(dummy, BUFFER_LENGTH, stdin);
    (void)sscanf(dummy, "%s - %s", inserteddate1, inserteddate2); // Obteniendo las dos fechas

    // loop for pages
    while (1)
    {
        ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT o.ordernumber, o.orderdate, o.shippeddate FROM orders o WHERE o.orderdate BETWEEN ? AND ? LIMIT ? OFFSET ?;", SQL_NTS);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

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

        // Binding both limit and offset to allow to change page via statements
        ret = SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &page_size, 0, NULL);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

        ret = SQLBindParameter(stmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &current_offset, 0, NULL);
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
        ret = SQLBindCol(stmt, 1, SQL_C_LONG, &ordernumber, 0, NULL);
        if (ret != SQL_SUCCESS)
        {
            printf("ERROR WHILE BINDING COLUMN 1\n");
            return EXIT_FAILURE;
        }

        ret = SQLBindCol(stmt, 2, SQL_C_TYPE_DATE, &orderdate, sizeof(orderdate), NULL);
        if (ret != SQL_SUCCESS)
        {
            printf("ERROR WHILE BINDING COLUMN 2\n");
            return EXIT_FAILURE;
        }

        ret = SQLBindCol(stmt, 3, SQL_C_TYPE_DATE, &shippeddate, sizeof(shippeddate), NULL);
        if (ret != SQL_SUCCESS)
        {
            printf("ERROR WHILE BINDING COLUMN 3\n");
            return EXIT_FAILURE;
        }

        // print results
        printf("Fetching results from query...\n");
        ret_rows = 0;
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
        {
            printf("%d %d-%02d-%02d %d-%02d-%02d\n", ordernumber, orderdate.year, orderdate.month, orderdate.day, shippeddate.year, shippeddate.month, shippeddate.day);
            ret_rows++;
        }

        if (ret_rows == 0)
        {
            printf("No more results.\n");
        }

        // prompt with instructions for the user
        printf("\nPress '+' to go to the next page, '-' to go to the previous page, or 'q' to quit.\n");
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

         //Reseting cursor
        ret2 = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret2))
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

int DetailQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret; /* ODBC API return status */
    int ordernumber = 0;
    int page_size = 10;  // Results per page
    int current_offset = 0;  // current page
    char user_input;
    int ret_rows = 0;  // Returned rows

    printf("Enter ordernumber > ");
    (void)fflush(stdout);
    (void)scanf("%d", &ordernumber);

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

    ret = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    ret = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

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

    ret = SQLCloseCursor(stmt);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    ret = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

    // THIRD QUERY (ONLY IMPLEMENT PAGES HERE)
    char productcode[BUFFER_LENGTH] = "\0";
    int quantityordered = 0, counter = 0;
    float priceeach = 0;
    while (1)
    {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT od.productcode, od.quantityordered, od.priceeach FROM orderdetails od WHERE od.ordernumber = ? ORDER BY od.orderlinenumber LIMIT ? OFFSET ?;", SQL_NTS);
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
        if (counter != 0)
        {
            printf("%d-%02d-%02d %s\n", orderdate.year, orderdate.month, orderdate.day, status);
            printf("%d\n", totalamount);
        }
        counter++;
        ret_rows=0;
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
        {
            printf("%s %d %.2f\n", productcode, quantityordered, priceeach);
            ret_rows++;
        }

        if (ret_rows == 0)
        {
            printf("No more results.\n");
        }

        // prompt with instructions for the user
        printf("\nPress '+' to go to the next page, '-' to go to the previous page, or 'q' to quit.\n");
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
        ret = SQLCloseCursor(stmt);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    }
    ret = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    ret = odbc_disconnect(env, dbc);
    if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
    return EXIT_SUCCESS;
}