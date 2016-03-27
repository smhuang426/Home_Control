
# first build

CC = gcc -pthread
LIBRARY = ../Library
INCLUDES = -I../Library/Fuzzy/ -lm

obj:main.c Fuzzy.o D_linked_list.o
	@$(CC) main.c Fuzzy.o D_linked_list.o -o main $(INCLUDES)
	@echo "\nmake done !\n"


D_linked_list.o:$(LIBRARY)/Artificail_neural_network/D_linked_list.h $(LIBRARY)/Artificail_neural_network/D_linked_list.c
	@$(CC) -c $(LIBRARY)/Artificail_neural_network/D_linked_list.c $(INCLUDES)
	@echo "Compile Doubly_linked_list.c done"


Fuzzy.o:$(LIBRARY)/Fuzzy/Fuzzy.h $(LIBRARY)/Fuzzy/Fuzzy.c
	@$(CC) -c $(LIBRARY)/Fuzzy/Fuzzy.c $(INCLUDES)
	@echo "Compile Fuzzy.c done"

clean:
	rm -f main *.o
