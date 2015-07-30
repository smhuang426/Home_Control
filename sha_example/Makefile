
# first build

CC = gcc -pthread

LIBRARY = ../Library/

obj:main.c sha256.o
	@$(CC) main.c sha256.o -o main

sha256.o:$(LIBRARY)/Sha256/sha256.h $(LIBRARY)/Sha256/sha256.c
	@$(CC) -c $(LIBRARY)/Sha256/sha256.c
	@echo "Compile Sha256.c done"


clean:
	rm -f main *.o
