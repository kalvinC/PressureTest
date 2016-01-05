CC=g++
CFLAGS=-c -g
OFLAGS=-o

TARGET=PressTest
OBJS=

$(TARGET):$(OBJS)
	$(CC) $(OFLAGS) $@ $^

%.o:%.cpp
	$(CC) $(CFLAGS) $@

%.o:%.c
	$(CC) $(CFLAGS) $@

.PHONY:clean

clean:
	rm -rf *.o
