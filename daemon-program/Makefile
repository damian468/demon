CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lrt
SRC = src/main.c src/daemon.c src/file_scanner.c src/signal_handler.c src/logger.c
OBJ = $(SRC:.c=.o)
TARGET = daemon_program

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean