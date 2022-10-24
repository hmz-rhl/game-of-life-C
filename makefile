CC=gcc
CFLAGS=-Wall -lncurses
DEPS = libGOL.h
OBJ = libGOL.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

GOL.exe: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	sudo rm *.o *.exe