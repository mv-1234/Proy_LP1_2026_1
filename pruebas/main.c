#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "huffman_jpeg.h"

int main(int argc, char *argv[]) {
    // Requerimos una bandera de operacion y archivos in/out
    if (argc < 4) {
        printf("Uso para comprimir:   %s -c <imagen.jpg> <archivo.huf>\n", argv[0]);
        printf("Uso para descomprimir: %s -d <archivo.huf> <salida.jpg>\n", argv[0]);
        return 1;
    }

    const char *modo = argv[1];
    const char *input = argv[2];
    const char *output = argv[3];

    if (strcmp(modo, "-c") == 0) {
        int ancho, alto, canales;
        unsigned char *pixeles = stbi_load(input, &ancho, &alto, &canales, 0);
        
        if (!pixeles) {
            printf("Error al cargar la imagen.\n");
            return 1;
        }

        comprimir_estilo_jpeg(pixeles, ancho, alto, canales, output);
        stbi_image_free(pixeles);

    } else if (strcmp(modo, "-d") == 0) {
        // La logica de stb_image_write ya esta dentro de la funcion en huffman_jpeg.c
        decodificar_estilo_jpeg(input, output);
        
    } else {
        printf("Modo no reconocido. Use -c (comprimir) o -d (descomprimir).\n");
        return 1;
    }

    return 0;
}