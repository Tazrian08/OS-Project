CC = gcc
CFLAGS = -Wall -Wextra
SOURCES = src/main.c src/shell.c src/parser.c src/executor.c src/history.c src/signals.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = shell

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
