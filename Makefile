CC=gcc
CFLAGS=-g -O2

DEPS = dataset.h models.h
OBJ = dataset.o models.o main.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

main: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf *.o
	rm main
