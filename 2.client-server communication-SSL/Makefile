all: server client

server: server.c
	gcc -Wall -o server server.c -L/usr/lib -lssl -lcrypto

client: client.c
	gcc -Wall -o client  client.c -L/usr/lib -lssl -lcrypto
