
# first build

CC = gcc -pthread
LIBRARY = ../Library
INCLUDES = -I../Library/FIR_filter/ -lm

obj:main.c FIR.o
	@$(CC) main.c FIR.o -o main $(INCLUDES)
	@echo "\nmake done !\n"


FIR.o:$(LIBRARY)/FIR_filter/FIR.h $(LIBRARY)/FIR_filter/FIR.c
	@$(CC) -c $(LIBRARY)/FIR_filter/FIR.c $(INCLUDES)
	@echo "Compile FIR.c done"


clean:
	rm -f main *.o
