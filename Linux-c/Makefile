SERVER_BIN=server
CLIENT_BIN=client

SERVER_SRC=server.c socket_util.c socket_handler.c
CLIENT_SRC=client.c socket_util.c socket_handler.c

CFLAGS=-g -O3 -Wall


all:
	gcc -o $(CLIENT_BIN) $(CLIENT_SRC) $(CFLAGS) 
	gcc -o $(SERVER_BIN) $(SERVER_SRC) $(CFLAGS)

.PHONY:
clean:
	-rm -rf $(SERVER_BIN) $(CLIENT_BIN)
