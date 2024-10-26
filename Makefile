CC = gcc
CCFLAGS = -Wall -O3
PROG = main
LIBS = -lglut -lGLU -lGL -lm -lSDL2 -lSDL2_mixer

all: $(PROG)

$(PROG): $(PROG).o
	$(CC) $(CCFLAGS) $(PROG).o -o $(PROG) $(LIBS)

%.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f *.o $(PROG)

