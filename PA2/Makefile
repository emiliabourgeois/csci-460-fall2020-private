CC=gcc
CFLAGS=-W -Wall -std=c11 -ggdb -pthread -m32
LIBS=
SRCS= b2bsim.c
OBJS=$(SRCS:.c=.o)
TARGET=b2bsim 

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET) *~
