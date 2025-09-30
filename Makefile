CC = gcc
CFLAGS = -Wall

rush: rush.c
	$(CC) $(CFLAGS) -o rush rush.c

.PHONY: clean
clean:
	rm rush.exe
