all: server client server-tcp client-tcp

client: client.c
	$(CC) client.c -o client
server: server.c
	$(CC) server.c -o server

client-tcp: client-tcp.c
	$(CC) client-tcp.c -o client-tcp
server-tcp: server-tcp.c
	$(CC) server-tcp.c -o server-tcp

clean:
	rm -f client server client-tcp server-tcp