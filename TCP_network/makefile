.PHONY:clean all

TARGET=svr cli
CC=gcc -lpthread

all:$(TARGET)

svr:tcp_server.o public.o
	$(CC) -o $@ $^
cli:tcp_client.o public.o
	$(CC) -o $@ $^

%.o : %.c
	$(CC) -c $^

clean:
	rm -rf *.o $(TARGET)
