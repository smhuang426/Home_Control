#make file

CC = gcc -pthread
INCLUDES = -I/usr/include/mysql -I../Library/Mysql -DBIG_JOINS=1  -fno-strict-aliasing -g#mysql_config --cflags
LIBS = -L/usr/lib/mysql -lmysqlclient -lm
LIBRARY = ../Library/

main: main.c Ex_mysql.o
	$(CC) main.c Ex_mysql.o -o main $(INCLUDES) $(LIBS)

Ex_mysql.o: $(LIBRARY)/Mysql/Ex_mysql.h $(LIBRARY)/Mysql/Ex_mysql.c
	@$(CC) -c $(LIBRARY)/Mysql/Ex_mysql.c $(INCLUDES) $(LIBS)
	@echo "Compile Ex_mysql.c done"

clean:
	rm -f main *.o
