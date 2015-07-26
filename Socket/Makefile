
# first build

CC = gcc -pthread
LIBRARY = ../Library/

main:main.c Network.o Link_list.o
	@$(CC) main.c Network.o Link_list.o -o main
	@echo "\nmake done !\n"

Network.o:$(LIBRARY)/Network/Network.h $(LIBRARY)/Network/Network.c
	@$(CC) -c $(LIBRARY)/Network/Network.c
	@echo "Compile Netowrk.c done"

Link_list.o:$(LIBRARY)/Link_list/Link_list.h $(LIBRARY)/Link_list/Link_list.c
	@$(CC) -c $(LIBRARY)/Link_list/Link_list.c
	@echo "Compile Link_list.c done"

clean:
	rm -f main *.o
