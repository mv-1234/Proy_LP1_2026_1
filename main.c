#include "huffman_jpeg.h"

int main(int argc, char *argv[]) {
    if (argc < 5) {
        printf("Uso: %s -m huffman <input.txt> <output.huf>\n", argv[0]);
        return 1;
    }


    int bloque_coeficientes_dct[64] = {
        52, -3,  2,  0,  0,  0,  0,  0,
        -2,  1,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0,
         0,  0,  0,  0,  0,  0,  0,  0
    };

    if (strcmp(argv[1], "-m") == 0 && strcmp(argv[2], "huffman") == 0) {
        comprimir_estilo_jpeg(bloque_coeficientes_dct, 64, argv[4]);
    } else {
        printf("Modo no soportado.\n");
    }

    return 0;
}
