CC=g++
CFLAGS=-c -g
OFLAGS=-o

VPATH=./src
TARGET=PressTest
OBJS=$(patsubst %.cpp, %.o, $(wildcard ./src/*.cpp))


$(TARGET):$(OBJS)
	@echo $(OBJS)
	$(CC) $(OFLAGS) $@ $^

%.o:%.cpp
	$(CC) $(CFLAGS) $@

%.o:%.c
	$(CC) $(CFLAGS) $@

.PHONY:clean

clean:
	rm $(TARGET)
	rm -rf *.o
	cd ./src
	rm -rf *.o

