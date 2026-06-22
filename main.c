// Define la implementacion de stb_image para que el compilador cree el codigo fuente de la libreria aqui
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "huffman_jpeg.h"

int main(int argc, char *argv[]) {
    // Verificamos que el usuario haya proporcionado la imagen de entrada y el nombre de salida
    if (argc < 3) {
        printf("Uso: %s <imagen_entrada.jpg/png/bmp> <output.huf>\n", argv[0]);
        return 1; // Salida con error
    }

    int ancho, alto, canales;
    
    // Carga la imagen utilizando stb_image. 
    // Devuelve un puntero al arreglo de pixeles y llena ancho, alto y canales
    // El '0' al final significa que queremos cargar los canales tal como estan en el archivo
    unsigned char *pixeles = stbi_load(argv[1], &ancho, &alto, &canales, 0);
    
    // Verificacion de errores por si la imagen no existe o esta corrupta
    if (!pixeles) {
        printf("Error al cargar la imagen.\n");
        return 1;
    }

    // Llamamos a la funcion de compresion
    comprimir_estilo_jpeg(pixeles, ancho, alto, canales, argv[2]);

    // Liberamos la memoria asignada por stb_image para los pixeles
    stbi_image_free(pixeles);
    
    return 0;
}