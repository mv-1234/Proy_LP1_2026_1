
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "huffman_jpeg.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <imagen_entrada.jpg/png/bmp> <output.huf>\n", argv[0]);
        return 1;
    }

    int ancho, alto, canales;
    unsigned char *pixeles = stbi_load(argv[1], &ancho, &alto, &canales, 0);
    if (!pixeles) {
        printf("Error al cargar la imagen.\n");
        return 1;
    }

    comprimir_estilo_jpeg(pixeles, ancho, alto, canales, argv[2]);

    stbi_image_free(pixeles);
    return 0;
}
