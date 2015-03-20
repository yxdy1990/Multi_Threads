OBJ1 = msg_send.c
OBJ2 = msg_recv.c
CC = gcc
CFLAGS = -Wall -O -g

send: $(OBJ1) $(OBJ2)
	$(CC) -g $(OBJ1) -o send
	$(CC) -g $(OBJ2) -o recv

clean:
	rm -rf *.o send recv

