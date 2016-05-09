CC=gcc
CFLAGS=-Wall -g -O0
server:server.c 
	$(CC) $(CFLAGS)  -o $@ $^ -lsqlite3 -ljson -lpthread
clean:
	$(RM) server .*.*.sw? test.db
