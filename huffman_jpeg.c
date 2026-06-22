#include "huffman_jpeg.h"

// Inicializa la estructura BitWriter y abre el archivo en modo escritura binaria
BitWriter* crear_bit_writer(const char *nombre_archivo) {
    BitWriter *bw = (BitWriter*)malloc(sizeof(BitWriter));
    bw->archivo = fopen(nombre_archivo, "wb");
    bw->buffer = 0;
    bw->bits_en_buffer = 0;
    return bw;
}

// Escribe un unico bit en el archivo
void escribir_bit(BitWriter *bw, int bit) {
    // Desplaza los bits existentes una posicion a la izquierda para hacer espacio
    bw->buffer <<= 1;          
    // Si el bit a escribir es 1, aplicamos una operacion OR para encender el ultimo bit
    if (bit) bw->buffer |= 1;  
    bw->bits_en_buffer++;

    // Si ya acumulamos 8 bits (1 byte), lo escribimos en el archivo y reiniciamos el buffer
    if (bw->bits_en_buffer == 8) {
        fputc(bw->buffer, bw->archivo);
        bw->buffer = 0;
        bw->bits_en_buffer = 0;
    }
}

// Escribe una cadena de caracteres formada por '0's y '1's bit a bit
void escribir_codigo(BitWriter *bw, const char *codigo) {
    for (int i = 0; codigo[i] != '\0'; i++) {
        // Pasa un 1 a escribir_bit si el caracter es '1', de lo contrario pasa 0
        escribir_bit(bw, codigo[i] == '1');
    }
}

// Cierra el BitWriter, asegurandose de escribir cualquier bit sobrante
void cerrar_bit_writer(BitWriter *bw) {
    // Si quedaron bits colgando (menos de 8), los alineamos a la izquierda y escribimos el byte
    if (bw->bits_en_buffer > 0) {
        bw->buffer <<= (8 - bw->bits_en_buffer); 
        fputc(bw->buffer, bw->archivo);
    }
    // Cerramos el archivo y liberamos la memoria
    fclose(bw->archivo);
    free(bw); 
}

// Crea y retorna un nuevo nodo hoja o interno para el arbol de Huffman
NodoHuffman* nuevo_nodo(int valor, int frec) {
    NodoHuffman* nodo = (NodoHuffman*)malloc(sizeof(NodoHuffman));
    nodo->valor = valor;
    nodo->frecuencia = frec;
    nodo->izq = nodo->der = NULL;
    return nodo;
}

// Recorre el arbol de Huffman recursivamente para generar los codigos binarios
void generar_codigos(NodoHuffman *raiz, char codigos[512][256], char *actual, int prof) {
    // Caso base: si el nodo es nulo, terminamos
    if (!raiz) return;

    // Si es un nodo hoja (sin hijos), hemos llegado a un valor que necesita un codigo
    if (!raiz->izq && !raiz->der) {
        actual[prof] = '\0'; // Terminamos la cadena de caracteres del codigo actual
        // Se suma 256 al valor para evitar indices negativos
        // (comun si el valor representa coeficientes de frecuencia que van de -256 a 255)
        strcpy(codigos[raiz->valor + 256], actual);
        return;
    }

    // Recorremos la rama izquierda: aniadimos un '0' al codigo
    actual[prof] = '0';
    generar_codigos(raiz->izq, codigos, actual, prof + 1);
    
    // Recorremos la rama derecha: aniadimos un '1' al codigo
    actual[prof] = '1';
    generar_codigos(raiz->der, codigos, actual, prof + 1);
}

// Funcion principal que divide la imagen en bloques de 8x8
void comprimir_estilo_jpeg(unsigned char *pixeles, int ancho, int alto, int canales, const char *archivo_salida) {
    printf(" Iniciando Compresion con Datos Reales \n");

    // Iteramos sobre la imagen en pasos de 8 pixeles (para formar bloques de 8x8)
    for (int y = 0; y < alto; y += 8) {
        for (int x = 0; x < ancho; x += 8) {
            
            int bloque_actual[64]; // Arreglo para almacenar los 64 pixeles del bloque actual
            int sub_indice = 0;

            // Iteramos dentro del bloque de 8x8 actual
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 8; i++) {
                    int pixel_x = x + i;
                    int pixel_y = y + j;

                    // Clamping (sujecion): Si el bloque se sale de los bordes de la imagen
                    // (porque el ancho/alto no son multiplos de 8), repetimos el ultimo pixel valido
                    if (pixel_x >= ancho) pixel_x = ancho - 1;
                    if (pixel_y >= alto) pixel_y = alto - 1;

                    // Calculamos la posicion en el arreglo unidimensional de la imagen
                    // Multiplicamos por 'canales' para saltar al pixel correcto
                    int indice_pixel = (pixel_y * ancho + pixel_x) * canales;
                
                    // Guardamos el primer canal (usualmente Red o un valor en escala de grises) en nuestro bloque
                    bloque_actual[sub_indice++] = pixeles[indice_pixel]; 
                }
            }
        }
    }
}