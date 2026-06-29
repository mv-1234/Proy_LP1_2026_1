CC = gcc
CFLAGS = -Wall -g -lm

all: compresor

compresor: main.o huffman_jpeg.o
	$(CC) $(CFLAGS) main.o huffman_jpeg.o -o compresor

main.o: main.c huffman_jpeg.h
	$(CC) $(CFLAGS) -c main.c

huffman_jpeg.o: huffman_jpeg.c huffman_jpeg.h
	$(CC) $(CFLAGS) -c huffman_jpeg.c

clean:
	rm -f *.o compresor
