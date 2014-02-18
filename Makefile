TARGET = indigo

SOURCES = main.c render.c math.c game.c
OBJS = $(SOURCES:%.c=%.o)

OS := $(shell uname -s)

CC = gcc
CFLAGS = -g -Wall
ifeq ($(OS), Darwin)
	LIBS = $(shell sdl2-config --libs) -framework OpenGl
else
	LIBS = $(shell sdl2-config --libs) -lGL -lGLU
endif

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(TARGET)
