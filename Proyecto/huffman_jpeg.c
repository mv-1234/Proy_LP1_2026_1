#include "huffman_jpeg.h"

// Funcion auxiliar para qsort que compara las frecuencias de dos nodos
int comparar_nodos(const void* a, const void* b) {
    NodoHuffman* n1 = *(NodoHuffman**)a;
    NodoHuffman* n2 = *(NodoHuffman**)b;
    return n1->frecuencia - n2->frecuencia;
}

// Libera la memoria del arbol de Huffman de forma recursiva
void liberar_arbol(NodoHuffman* raiz) {
    if (!raiz) return;
    liberar_arbol(raiz->izq);
    liberar_arbol(raiz->der);
    free(raiz);
}

// Prepara la estructura para escribir bits en el archivo binario
BitWriter* crear_bit_writer(const char *nombre_archivo) {
    BitWriter *bw = (BitWriter*)malloc(sizeof(BitWriter));
    bw->archivo = fopen(nombre_archivo, "wb");
    bw->buffer = 0;
    bw->bits_en_buffer = 0;
    return bw;
}

// Inserta un 0 o un 1 en el buffer y lo guarda en el archivo cuando se llena un byte
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

// Lee una cadena de texto de 0s y 1s para mandarla al escritor de bits
void escribir_codigo(BitWriter *bw, const char *codigo) {
    for (int i = 0; codigo[i] != '\0'; i++) {
        escribir_bit(bw, codigo[i] == '1');
    }
}

// Empuja los bits sobrantes alineados a la izquierda y cierra el archivo
void cerrar_bit_writer(BitWriter *bw) {
    if (bw->bits_en_buffer > 0) {
        bw->buffer <<= (8 - bw->bits_en_buffer); 
        fputc(bw->buffer, bw->archivo);
    }
    fclose(bw->archivo);
    free(bw); 
}

// Reserva memoria para un nuevo elemento del arbol
NodoHuffman* nuevo_nodo(int valor, int frec) {
    NodoHuffman* nodo = (NodoHuffman*)malloc(sizeof(NodoHuffman));
    nodo->valor = valor;
    nodo->frecuencia = frec;
    nodo->izq = nodo->der = NULL;
    return nodo;
}

// Recorre las ramas para asignar la ruta binaria a cada valor
void generar_codigos(NodoHuffman *raiz, char codigos[512][256], char *actual, int prof) {
    if (!raiz) return;

    // Si llega a una hoja guarda el codigo y le suma 256 para evitar indices negativos
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

// Agrupa los valores de menor a mayor frecuencia hasta formar el arbol completo
NodoHuffman* construir_huffman(int frecuencias[512]) {
    NodoHuffman* lista[512];
    int n = 0;

    for (int i = 0; i < 512; i++) {
        if (frecuencias[i] > 0) {
            lista[n++] = nuevo_nodo(i - 256, frecuencias[i]);
        }
    }

    if (n == 0) return NULL;

    while (n > 1) {
        qsort(lista, n, sizeof(NodoHuffman*), comparar_nodos);
        
        NodoHuffman* izq = lista[0];
        NodoHuffman* der = lista[1];
        
        // Crea un nodo interno temporal que suma las dos frecuencias menores
        NodoHuffman* nuevo = nuevo_nodo(-256, izq->frecuencia + der->frecuencia);
        nuevo->izq = izq;
        nuevo->der = der;
        
        lista[0] = nuevo;
        lista[1] = lista[n - 1];
        n--;
    }

    return lista[0];
}

// Funcion principal que simula los bloques de procesamiento de una imagen
void comprimir_estilo_jpeg(unsigned char *pixeles, int ancho, int alto, int canales, const char *archivo_salida) {
    printf(" Iniciando Compresion con Datos Reales \n");

    // Crea un arreglo para contar las apariciones de cada valor en toda la imagen
    int frecuencias[512] = {0};
    
    for (int y = 0; y < alto; y += 8) {
        for (int x = 0; x < ancho; x += 8) {
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 8; i++) {
                    int pixel_x = x + i;
                    int pixel_y = y + j;

                    // Repite el ultimo pixel si el bloque queda fuera de los limites de la imagen
                    if (pixel_x >= ancho) pixel_x = ancho - 1;
                    if (pixel_y >= alto) pixel_y = alto - 1;

                    int indice_pixel = (pixel_y * ancho + pixel_x) * canales;
                    int valor_pixel = pixeles[indice_pixel];
                    
                    frecuencias[valor_pixel + 256]++;
                }
            }
        }
    }

    // Forma la estructura de arbol y prepara el diccionario de prefijos
    NodoHuffman* raiz = construir_huffman(frecuencias);
    
    char codigos[512][256];
    char ruta_actual[256];
    for (int i = 0; i < 512; i++) codigos[i][0] = '\0'; 
    
    if (raiz) {
        generar_codigos(raiz, codigos, ruta_actual, 0);
    }

    // Abre el archivo resultante para empezar a guardar la informacion comprimida
    BitWriter *bw = crear_bit_writer(archivo_salida);
    if (!bw || !bw->archivo) {
        printf(" Error No se pudo crear el archivo %s\n", archivo_salida);
        if (raiz) liberar_arbol(raiz);
        return;
    }

    // Recorre nuevamente la imagen por bloques de 8x8
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

            // Busca el codigo de cada pixel en el diccionario y lo manda a escribir en binario
            for (int k = 0; k < 64; k++) {
                int valor = bloque_actual[k];
                escribir_codigo(bw, codigos[valor + 256]);
            }
        }
    }
    
    // Finaliza el flujo de bits y limpia la memoria
    cerrar_bit_writer(bw);
    if (raiz) liberar_arbol(raiz);

    printf(" Compresion finalizada con exito Archivo '%s' guardado\n", archivo_salida);
}