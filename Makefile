PROJECT=demo_lcd1x9
CC=gcc
CFLAGS=-I.
DEPS =
OBJ = lcd1x9.o demo_lcd1x9.o
EXTRA_LIBS=-lwiringPi -lm -lrt -lcrypt -lpthread

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(EXTRA_LIBS)

.PHONY: clean

clean:
	rm -f $(PROJECT) $(OBJ)
