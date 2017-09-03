.PHONY:all
all:tcp_server1 tcp_client tcp_server2 tcp_server3

tcp_server1:tcp_server1.c
	gcc -o $@ $^ -g
tcp_client:tcp_client.c
	gcc -o $@ $^
tcp_server2:tcp_server2.c
	gcc -o $@ $^
tcp_server3:tcp_server3.c
	gcc -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f tcp_server1 tcp_client tcp_server2 tcp_server3
