
# first build

CC = gcc -pthread
LIBRARY = ../Library
INCLUDES = -I../Library/Kalman_filter/ -lm

obj:main.c kalman.o
	@$(CC) main.c kalman.o -o main $(INCLUDES)
	@echo "\nmake done !\n"


kalman.o:$(LIBRARY)/Kalman_filter/kalman.h $(LIBRARY)/Kalman_filter/kalman.c
	@$(CC) -c $(LIBRARY)/Kalman_filter/kalman.c $(INCLUDES)
	@echo "Compile kalman.c done"

clean:
	rm -f main *.o
