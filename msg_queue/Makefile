OBJ1 = msg_send.c
OBJ2 = msg_recv.c
OBJ3 = msg_proc.c
CC = gcc
CFLAGS = -Wall -O -g

send: $(OBJ1)	
	$(CC) -g $(OBJ1) -o send

recv: $(OBJ2)
	$(CC) -g $(OBJ2) -o recv

proc: $(OBJ3)
	$(CC) -g $(OBJ3) -o proc

clean:
	rm -rf *.o core* send recv proc

