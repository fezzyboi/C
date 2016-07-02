CC = gcc
CFLAGS = -Wall
DEPS = Player.h Game.h UnixBoard.h
OBJ = Player.o Game.o rpg.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

rpg: $(OBJ)
	gcc $(CFLAGS) -o $@ $^
	gcc $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o rpg