all: server charge
server: 
	gcc -o server testserver.c -L. -lesl -I./ -lpthread
charge: testcharge.c
	gcc -o charge testcharge.c -L. -lesl -I./ -lpthread
