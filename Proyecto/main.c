#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "huffman_jpeg.h"

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <bandera> <archivo_in> <archivo_out>\n", argv[0]);
        printf("  -c : Comprimir (ej. -c foto.bmp salida.huf)\n");
        printf("  -d : Descomprimir (ej. -d salida.huf foto_reconstruida.jpg)\n");
        return 1;
    }

    const char *modo = argv[1];
    const char *input = argv[2];
    const char *output = argv[3];

    if (strcmp(modo, "-c") == 0) {
        int ancho, alto, canales;
        unsigned char *pixeles = stbi_load(input, &ancho, &alto, &canales, 0);
        
        if (!pixeles) {
            fprintf(stderr, "Error: No se pudo cargar la imagen '%s'. Verifica la ruta.\n", input);
            return 1;
        }

        comprimir_estilo_jpeg(pixeles, ancho, alto, canales, output);
        stbi_image_free(pixeles);
        printf("Proceso de compresion finalizado.\n");

    } else if (strcmp(modo, "-d") == 0) {
        // La descompresión reconstruye el archivo JPG usando la metadata interna
        decodificar_estilo_jpeg(input, output);
        printf("Proceso de descompresion finalizado.\n");
        
    } else {
        fprintf(stderr, "Error: Modo '%s' no reconocido. Use -c o -d.\n", modo);
        return 1;
    }

    return 0;
}

/*comandos: gcc -Wall -g main.c huffman_jpeg.c -o compresor.exe   (compilar)
    .\compresor.exe -c foto.jpg foto_corregida.huf  (comprimir)
    .\compresor.exe -d foto_corregida.huf foto_reconstruida_corregida.jpg (descomprimir)
*/