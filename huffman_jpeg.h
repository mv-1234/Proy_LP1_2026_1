#ifndef HUFFMAN_JPEG_H
#define HUFFMAN_JPEG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NodoHuffman {
    int valor;                 
    int frecuencia;
    struct NodoHuffman *izq, *der;
} NodoHuffman;

typedef struct {
    FILE *archivo;
    unsigned char buffer;
    int bits_en_buffer;
} BitWriter;

BitWriter* crear_bit_writer(const char *nombre_archivo);
void escribir_bit(BitWriter *bw, int bit);
void escribir_codigo(BitWriter *bw, const char *codigo);
void cerrar_bit_writer(BitWriter *bw);
void comprimir_estilo_jpeg(int *datos_entrada, int tam, const char *archivo_salida);

#endif
