CC ?= gcc
SRCS = $(wildcard *.c src/*.c)
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

testcase:
	@$(CC) test/ota_publish.c -o ota_publish $(LFLAGS)
	@echo "Compiled ota_publish successfully!"

.PHONY: clean
clean:
	@rm -rf $(TARGET) $(OBJS) ota_publish
	@echo "Cleanup complete!"
