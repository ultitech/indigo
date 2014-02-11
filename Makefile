TARGET = indigo

SOURCES = main.c render.c math.c game.c
OBJS = $(SOURCES:%.c=%.o)

CC = gcc
CFLAGS = -g -Wall
LIBS = $(shell sdl2-config --libs) -lGL -lGLU

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(TARGET)
