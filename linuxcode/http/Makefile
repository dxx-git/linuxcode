cc=gcc 
.PHONY:all
all:httpd

httpd:$(shell ls *.c)
	$(cc) -o  $@ $^ -std=c99 -g
.PHONY:clean
clean:
	rm -f httpd
