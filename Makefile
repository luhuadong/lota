CC ?= gcc
SRCS = $(wildcard *.c */*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
FLAG = -g
LFLAGS = -Wall -lcurl
NAME = $(wildcard *.c)
TARGET = lota

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(FLAG) $(LFLAGS)

%.o:%.c
	$(CC) -o $@ -c $< $(FLAG) $(LFLAGS)

clean:
	rm -rf $(TARGET) $(OBJS)