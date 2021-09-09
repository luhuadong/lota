CC ?= gcc
SRCS = $(wildcard *.c */*.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))
FLAG = -g -I./inc -I./src -L./lib
LFLAGS = -Wall -lpthread -lcurl -lpaho-mqtt3c -lpaho-mqtt3a
SFLAGS = -static libpaho-mqtt3a.a libpaho-mqtt3c.a
NAME = $(wildcard *.c)
TARGET = lota

$(TARGET):$(OBJS)
	@$(CC) -o $@ $^ $(FLAG) $(LFLAGS)
	@echo "Compiled "$@" successfully!"

%.o:%.c
	@$(CC) -o $@ -c $< $(FLAG) $(LFLAGS)
	@echo "Compiled "$<" successfully!"

.PHONY: clean
clean:
	@rm -rf $(TARGET) $(OBJS)
	@echo "Cleanup complete!"
