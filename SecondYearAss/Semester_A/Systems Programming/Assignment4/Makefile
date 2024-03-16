CC = gcc
CFLAGS = -Wall -Wextra -std=c99
SRCS = encodemain.c compress.c
OBJS = $(SRCS:.c=.o)
TARGET = digitcompress


all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
		$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
		$(RM) $(OBJS) $(TARGET)