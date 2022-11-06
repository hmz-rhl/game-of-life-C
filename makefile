CC=gcc
CFLAGS=-Wall -lncurses
DEPS = libGOL.h
OBJ = libGOL.o main.o 

%.o: %.c $(DEPS)
	sudo $(CC) -c -o $@ $< $(CFLAGS)

GOL.exe: $(OBJ)
	sudo $(CC) -o $@ $^ $(CFLAGS)

clean:
	sudo rm *.o
