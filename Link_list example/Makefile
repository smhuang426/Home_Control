
# first build

CC = gcc -pthread
LIBRARY = ../Library/
INCLUDES = -I../Library/Link_list/

obj:main.c Link_list.o
	@$(CC) main.c Link_list.o -o main $(INCLUDES)
	@echo "\nmake done !\n"

Link_list.o:$(LIBRARY)/Link_list/Link_list.h $(LIBRARY)/Link_list/Link_list.c
	@$(CC) -c $(LIBRARY)/Link_list/Link_list.c $(INCLUDES)
	@echo "Compile Link_list.c done"


clean:
	rm -f main *.o
