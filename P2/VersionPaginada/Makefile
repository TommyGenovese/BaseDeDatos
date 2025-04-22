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

splint:
	@$(SPLINT)