#include "huffman_jpeg.h"

BitWriter* crear_bit_writer(const char *nombre_archivo) {
    BitWriter *bw = (BitWriter*)malloc(sizeof(BitWriter));
    bw->archivo = fopen(nombre_archivo, "wb");
    bw->buffer = 0;
    bw->bits_en_buffer = 0;
    return bw;
}

void escribir_bit(BitWriter *bw, int bit) {
    bw->buffer <<= 1;          
    if (bit) bw->buffer |= 1;  
    bw->bits_en_buffer++;

    if (bw->bits_en_buffer == 8) {
        fputc(bw->buffer, bw->archivo);
        bw->buffer = 0;
        bw->bits_en_buffer = 0;
    }
}

void escribir_codigo(BitWriter *bw, const char *codigo) {
    for (int i = 0; codigo[i] != '\0'; i++) {
        escribir_bit(bw, codigo[i] == '1');
    }
}

void cerrar_bit_writer(BitWriter *bw) {
    if (bw->bits_en_buffer > 0) {
        bw->buffer <<= (8 - bw->bits_en_buffer); 
        fputc(bw->buffer, bw->archivo);
    }
    fclose(bw->archivo);
    free(bw); 
}

NodoHuffman* nuevo_nodo(int valor, int frec) {
    NodoHuffman* nodo = (NodoHuffman*)malloc(sizeof(NodoHuffman));
    nodo->valor = valor;
    nodo->frecuencia = frec;
    nodo->izq = nodo->der = NULL;
    return nodo;
}

void generar_codigos(NodoHuffman *raiz, char codigos[512][256], char *actual, int prof) {
    if (!raiz) return;

    if (!raiz->izq && !raiz->der) {
        actual[prof] = '\0';
        strcpy(codigos[raiz->valor + 256], actual);
        return;
    }

    actual[prof] = '0';
    generar_codigos(raiz->izq, codigos, actual, prof + 1);
    
    actual[prof] = '1';
    generar_codigos(raiz->der, codigos, actual, prof + 1);
}


void comprimir_estilo_jpeg(unsigned char *pixeles, int ancho, int alto, int canales, const char *archivo_salida) {
    printf(" Iniciando Compresion con Datos Reales \n");

   
    for (int y = 0; y < alto; y += 8) {
        for (int x = 0; x < ancho; x += 8) {
            
            int bloque_actual[64];
            int sub_indice = 0;

            
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 8; i++) {
                    int pixel_x = x + i;
                    int pixel_y = y + j;

                
                    if (pixel_x >= ancho) pixel_x = ancho - 1;
                    if (pixel_y >= alto) pixel_y = alto - 1;

              
                    int indice_pixel = (pixel_y * ancho + pixel_x) * canales;
                
                    bloque_actual[sub_indice++] = pixeles[indice_pixel]; 
                }
            }

           
        }
    }
    
  
}
