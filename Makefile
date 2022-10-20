CC := gcc
CFLAGS += -g -Wall
CFLAGS += -I./ipc_stub/
LDLIBS += -lpthread

TARGET := client server


all: $(TARGET)

client: common_client/client.c fifo/fifo.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

server: common_server/server.c fifo/fifo.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm $(TARGET)
