built:
	g++ server.c -o server
	g++ client.c -o client
run-server:
	./server
run-client:
	./client
clean:
	rm -r server client
