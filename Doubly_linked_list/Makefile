
# first build

CC = gcc -pthread
LIBRARY = ../Library
INCLUDES = -I../Library/Doubly_linked_list/

obj:main.c D_linked_list.o
	@$(CC) main.c D_linked_list.o -o main $(INCLUDES)
	@echo "\nmake done !\n"

D_linked_list.o:$(LIBRARY)/Doubly_linked_list/D_linked_list.h $(LIBRARY)/Doubly_linked_list/D_linked_list.c
	@$(CC) -c $(LIBRARY)/Doubly_linked_list/D_linked_list.c $(INCLUDES)
	@echo "Compile Doubly_linked_list.c done"


clean:
	rm -f main *.o
