.PHONY:clean all

TARGET=svr cli
CC=gcc -lpthread

all:$(TARGET)

svr:chat_svr.o 
	$(CC) -o $@ $^
cli:chat_cli.o 
	$(CC) -o $@ $^

%.o : %.c
	$(CC) -c $^

clean:
	rm -rf *.o $(TARGET)
