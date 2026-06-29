#ifndef HUFFMAN_JPEG_H
#define HUFFMAN_JPEG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar un nodo en el arbol de Huffman
typedef struct NodoHuffman {
    int valor; // El valor a comprimir (pixel)
    int frecuencia; // Cantidad de veces que aparece este valor
    struct NodoHuffman *izq, *der; 
} NodoHuffman;

// Estructura para escribir datos bit a bit en un archivo
typedef struct {
    FILE *archivo; 
    unsigned char buffer; 
    int bits_en_buffer; 
} BitWriter;

// [NUEVO] Estructura para leer datos bit a bit desde el archivo comprimido
typedef struct {
    FILE *archivo;
    unsigned char buffer;
    int bits_restantes; // Cuantos bits quedan por leer en el buffer actual
} BitReader;

// Declaraciones de funciones de Compresion
BitWriter* crear_bit_writer(const char *nombre_archivo);
void escribir_bit(BitWriter *bw, int bit);
void escribir_codigo(BitWriter *bw, const char *codigo);
void cerrar_bit_writer(BitWriter *bw);
void comprimir_estilo_jpeg(unsigned char *pixeles, int ancho, int alto, int canales, const char *archivo_salida);

// [NUEVO] Declaraciones de funciones de Descompresion
BitReader* crear_bit_reader(const char *nombre_archivo);
int leer_bit(BitReader *br);
void cerrar_bit_reader(BitReader *br);
void decodificar_estilo_jpeg(const char *archivo_comprimido, const char *archivo_salida_imagen);

#endif