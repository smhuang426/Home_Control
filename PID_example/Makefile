
# first build

CC = gcc -pthread
LIBRARY = ../Library
INCLUDES = -I../Library/PID_Controller/ -lm

obj:main.c PID.o
	@$(CC) main.c PID.o -o main $(INCLUDES)
	@echo "\nmake done !\n"


PID.o:$(LIBRARY)/PID_Controller/PID.h $(LIBRARY)/PID_Controller/PID.c
	@$(CC) -c $(LIBRARY)/PID_Controller/PID.c $(INCLUDES)
	@echo "Compile PID.c done"

clean:
	rm -f main *.o
