.PHONY:all 
all:select_server tcp_client

select_server:select_server.c
	gcc -o $@ $^
tcp_client:tcp_client.c
	gcc -o $@ $^

.PHONY:clean
clean:
	rm -f select_server tcp_client

