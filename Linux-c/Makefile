SERVER_BIN=server
CLIENT_BIN=client

CFLAGS=-g -O3


all:
	gcc -o $(CLIENT_BIN) client.c socket_util.c socket_handler.c $(CFLAGS) 
	gcc -o $(SERVER_BIN) server.c socket_util.c socket_handler.c $(CFLAGS)

.PHONY:
clean:
	-rm -rf $(SERVER_BIN) $(CLIENT_BIN)
