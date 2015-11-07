
# first build

CC = gcc -pthread
LIBRARY = ../Library
INCLUDES = -I../Library/FIR_filter/ -lm

obj:main.c FIR.o D_linked_list.o
	@$(CC) main.c FIR.o D_linked_list.o -o main $(INCLUDES)
	@echo "\nmake done !\n"


FIR.o:$(LIBRARY)/FIR_filter/FIR.h $(LIBRARY)/FIR_filter/FIR.c
	@$(CC) -c $(LIBRARY)/FIR_filter/FIR.c $(INCLUDES)
	@echo "Compile FIR.c done"


D_linked_list.o:$(LIBRARY)/FIR_filter/D_linked_list.h $(LIBRARY)/FIR_filter/D_linked_list.c
	@$(CC) -c $(LIBRARY)/FIR_filter/D_linked_list.c $(INCLUDES)
	@echo "Compile D_linked_list.c done"


clean:
	rm -f main *.o
