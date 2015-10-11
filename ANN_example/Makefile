
# first build

CC = gcc -pthread
LIBRARY = ../Library
INCLUDES = -I../Library/Artificail_neural_network/ -In

obj:main.c D_linked_list.o ANN.o
	@$(CC) main.c D_linked_list.o ANN.o -o main $(INCLUDES)
	@echo "\nmake done !\n"

D_linked_list.o:$(LIBRARY)/Artificail_neural_network/D_linked_list.h $(LIBRARY)/Artificail_neural_network/D_linked_list.c
	@$(CC) -c $(LIBRARY)/Artificail_neural_network/D_linked_list.c $(INCLUDES)
	@echo "Compile Doubly_linked_list.c done"


ANN.o:$(LIBRARY)/Artificail_neural_network/ANN.h $(LIBRARY)/Artificail_neural_network/ANN.c
	@$(CC) -c $(LIBRARY)/Artificail_neural_network/ANN.c $(INCLUDES)
	@echo "Compile ANN.c done"


clean:
	rm -f main *.o
