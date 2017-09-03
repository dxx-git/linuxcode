.PHONY:all
all:poll_server poll_client

poll_server:poll_server.c
	gcc -o $@ $^ -g
poll_client:poll_client.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f poll_server poll_client
