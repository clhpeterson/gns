CC = gcc

CFLAGS = -g3 -std=c99 -pedantic -Wall

SOURCES = gns.c queue.c

OBJECTS = $(SOURCES:.c=.o) 

TARGET = gns



$(TARGET): $(OBJECTS)
	${CC} ${CFLAGS} -o $@ $^



%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
