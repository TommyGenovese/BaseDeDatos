#ifndef AUX_H
#define AUX_H

#include <sql.h>
#include <sqlext.h>

/* CONNECTION PARAMETERS, ADAPT TO YOUR SCENARIO */
/* Check that the driver is the same before running */
#define CONNECTION_PARS "driver={PostgreSQL ANSI};uid=alumnodb;" \
        "pwd=alumnodb;server=localhost;database=classicmodels"

/* report most recient error */
void odbc_extract_error(char *fn, SQLHANDLE handle, SQLSMALLINT type);

/* STANDARD CONNECTION PROCEDURE */
int odbc_connect(SQLHENV* env, SQLHDBC* dbc);

/* STANDARD DISCONNECTION PROCEDURE */
int odbc_disconnect(SQLHENV env, SQLHDBC dbc);

/* check errors */
#define CHECK_ERROR(e, s, h, t) {\
            if (e!=SQL_SUCCESS && e != SQL_SUCCESS_WITH_INFO) {odbc_extract_error(s, h, t); goto exit;} \
}
#endif
