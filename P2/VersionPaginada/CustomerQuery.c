#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sql.h>
#include <sqlext.h>
#include "odbc.h"

#define BUFFER_LENGTH 512

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
    int page_size = 10;  // Results per page
    int current_offset = 0;  // current page
    char user_input;
    int ret_rows = 0;  // Returned rows

    /* CONNECT */
    ret = odbc_connect(&env, &dbc);
    if (!SQL_SUCCEEDED(ret))
    {
        return EXIT_FAILURE;
    }

    printf("Enter customer name > ");
    (void)fflush(stdout);
    (void)scanf("%s", insertedname);
    char searchTerm[BUFFER_LENGTH];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    (void)snprintf(searchTerm, sizeof(searchTerm), "%%%s%%", insertedname); // Adding '%' so the query does not explode
#pragma GCC diagnostic pop


    while (1)
    {
            /* Allocate a statement handle */
        ret = SQLAllocHandle(SQL_HANDLE_STMT, dbc, &stmt);
        ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT c.customernumber, c.customername, c.contactfirstname, c.contactlastname FROM customers c WHERE (c.contactfirstname LIKE ?) OR (c.contactlastname LIKE ?) LIMIT ? OFFSET ?;", SQL_NTS);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_ENV);
            return ret;
        }
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
        // Binding limit to restrict the results per page
        ret = SQLBindParameter(stmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &page_size, 0, NULL);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }

        // Binding offset witn the number of rows to jump
        ret = SQLBindParameter(stmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_C_LONG, 0, 0, &current_offset, 0, NULL);
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
        ret_rows=0;
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
        {
            printf("%d %s %s %s\n", customernumber, customername, customerfirstname, customerlastname);
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

         //Reseting cursor
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

int ListProductsQuery(void)
{
    SQLHENV env = NULL;
    SQLHDBC dbc = NULL;
    SQLHSTMT stmt = NULL;
    SQLRETURN ret;
    int customernumber = 0;
    char productname[BUFFER_LENGTH] = "\0";
    char totalamount[BUFFER_LENGTH] = "\0";
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
        odbc_extract_error("", dbc, SQL_HANDLE_DBC);
        return ret;
    }

    

    printf("Enter customer number > ");
    (void)fflush(stdout);
    (void)scanf("%d", &customernumber);

    while(1)
    {
        ret = SQLPrepare(stmt, (SQLCHAR *)"SELECT p.productname, SUM(od.quantityordered) AS total_amount FROM orderdetails od JOIN products p ON p.productcode = od.productcode JOIN orders o ON od.ordernumber = o.ordernumber WHERE o.customernumber = ? GROUP BY p.productcode ORDER BY p.productcode LIMIT ? OFFSET ?", SQL_NTS);
        if (!SQL_SUCCEEDED(ret))
        {
            odbc_extract_error("", stmt, SQL_HANDLE_STMT);
            return ret;
        }
        ret = SQLBindParameter(stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &customernumber, sizeof(customernumber), NULL);
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
        ret_rows=0;
        while (SQL_SUCCEEDED(ret = SQLFetch(stmt)))
        {
            printf("%s %s\n", productname, totalamount);
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

         //Reseting cursor
        /* free up statement handle */
        ret = SQLFreeHandle(SQL_HANDLE_STMT, stmt);
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