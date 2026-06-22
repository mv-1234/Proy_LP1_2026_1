#ifndef HUFFMAN_JPEG_H
#define HUFFMAN_JPEG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar un nodo en el arbol de Huffman
typedef struct NodoHuffman {
    int valor; // El valor a comprimir
    int frecuencia; // Cantidad de veces que aparece este valor (peso del nodo)
    struct NodoHuffman *izq, *der; // Punteros a los hijos izquierdo y derecho
} NodoHuffman;

// Estructura para escribir datos bit a bit en un archivo,
// ya que las funciones estandar de C escriben byte a byte
typedef struct {
    FILE *archivo; // Puntero al archivo de salida
    unsigned char buffer; // Acumulador de bits (hasta llegar a 8)
    int bits_en_buffer; // Contador de cuantos bits hay actualmente en el buffer
} BitWriter;

// Declaraciones de funciones
BitWriter* crear_bit_writer(const char *nombre_archivo);
void escribir_bit(BitWriter *bw, int bit);
void escribir_codigo(BitWriter *bw, const char *codigo);
void cerrar_bit_writer(BitWriter *bw);
void comprimir_estilo_jpeg(unsigned char *pixeles, int ancho, int alto, int canales, const char *archivo_salida);

#endif