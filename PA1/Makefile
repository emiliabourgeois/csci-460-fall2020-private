CC      =  gcc
CFLAGS  = -Wall -pedantic -std=c11 -ggdb 
LDFLAGS =
SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)
DEPS    = $(wildcard *.h)
TARGET  = album

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET) *~
	rm -f index.html