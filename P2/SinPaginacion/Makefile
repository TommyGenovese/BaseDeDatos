CC = gcc -g
CFLAGS = -Wall -Wextra -pedantic #-ansi
LDLIBS = -lodbc

export PGDATABASE:=classicmodels
export PGUSER :=alumnodb
export PGPASSWORD :=alumnodb
export PGCLIENTENCODING:=LATIN9
export PGHOST:=localhost

DBNAME =$(PGDATABASE)
PSQL = psql
CREATEDB = createdb
DROPDB = dropdb --if-exists
PG_DUMP = pg_dump
PG_RESTORE = pg_restore
SPLINTDUMP = splint.log
SPLINT := $(shell splint -nullpass -type -predboolint -usedef -formattype -redef *.c *.h >$(SPLINTDUMP))

all: dropdb createdb restore
createdb:
	@echo Creando BBDD
	@$(CREATEDB)
dropdb:
	@echo Eliminando BBDD
	@$(DROPDB) $(DBNAME)
	rm -f *.log
dump:
	@echo creando dumpfile
	@$(PG_DUMP) > $(DBNAME).sql
restore:
	@echo restore data base
	@cat $(DBNAME).sql | $(PSQL)
shell:
	@echo create psql shell
	@$(PSQL)


EXE = menu

compile: clean $(EXE)

menu: menu.o ProductQuery.o OrderQuery.o CustomerQuery.o odbc.o
	$(CC) -o $@ $^ $(LDLIBS)

menu.o: menu.c ProductQuery.h OrderQuery.h CustomerQuery.h odbc.h
	$(CC) $(CFLAGS) -c $< -o $@

ProductQuery.o: ProductQuery.c ProductQuery.h
	$(CC) $(CFLAGS) -c $< -o $@

OrderQuery.o: OrderQuery.c OrderQuery.h
	$(CC) $(CFLAGS) -c $< -o $@

CustomerQuery.o: CustomerQuery.c CustomerQuery.h
	$(CC) $(CFLAGS) -c $< -o $@

odbc.o: odbc.c odbc.h
	$(CC) $(CFLAGS) -c $< -o $@


clean :
	rm -f *.o core $(EXE)

test_all: test_products test_orders test_customers

test_products:
	@echo -----------------------------------------------
	@echo PRODUCTS STOCK TEST
	@echo -----------------------------------------------
	@./products_stock.sh
	@echo -----------------------------------------------
	@echo PRODUCTS FIND TEST
	@echo -----------------------------------------------
	@./products_find.sh
	@echo -----------------------------------------------
	@echo Done
	@echo -----------------------------------------------

test_orders:
	@echo -----------------------------------------------
	@echo ORDERS OPEN TEST
	@echo -----------------------------------------------
	@./orders_open.sh
	@echo -----------------------------------------------
	@echo ORDERS RANGE TEST
	@echo -----------------------------------------------
	@./orders_range.sh
	@echo -----------------------------------------------
	@echo ORDERS DETAIL TEST
	@echo -----------------------------------------------
	@./orders_detail.sh
	@echo -----------------------------------------------
	@echo Done
	@echo -----------------------------------------------

test_customers:
	@echo -----------------------------------------------
	@echo CUSTOMERS FIND TEST
	@echo -----------------------------------------------
	@./customers_find.sh
	@echo -----------------------------------------------
	@echo CUSTOMERS LIST PRODUCTS TEST
	@echo -----------------------------------------------
	@./customers_list_products.sh
	@echo -----------------------------------------------
	@echo CUSTOMERS BALANCE TEST
	@echo -----------------------------------------------
	@./customers_balance.sh
	@echo -----------------------------------------------
	@echo Done
	@echo -----------------------------------------------

splint:
	@$(SPLINT)

give_permisions:
	@echo Making all .sh files executables
	@chmod +x *.sh
	@echo Done
