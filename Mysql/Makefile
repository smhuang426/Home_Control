#make file

CC = gcc -pthread
INCLUDES = -I/usr/include/mysql -DBIG_JOINS=1  -fno-strict-aliasing   -g#mysql_config --cflags
LIBS = -L/usr/lib/mysql -lmysqlclient -lm

main: main.c
	$(CC) main.c -o main $(INCLUDES) $(LIBS)

clean:
	rm -f main
