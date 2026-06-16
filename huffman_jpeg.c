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

void comprimir_estilo_jpeg(int *datos_entrada, int tam, const char *archivo_salida) {
    printf(" Iniciando Compresion Estilo JPEG \n");

    printf("[1] Aplicando RLE pre-Huffman...\n");
    int *datos_rle = (int*)malloc(sizeof(int) * tam * 2);
    int indice_rle = 0;
    
    for (int i = 0; i < tam; i++) {
        int cuenta_ceros = 0;
        while (i < tam && datos_entrada[i] == 0 && cuenta_ceros < 15) {
            cuenta_ceros++;
            i++;
        }
        if (cuenta_ceros > 0) {
            datos_rle[indice_rle++] = 0;            
            datos_rle[indice_rle++] = cuenta_ceros; 
            if(i < tam) datos_rle[indice_rle++] = datos_entrada[i];
        } else {
            datos_rle[indice_rle++] = datos_entrada[i];
        }
    }

    int frecuencias[512] = {0}; 
    for (int i = 0; i < indice_rle; i++) {
        frecuencias[datos_rle[i] + 256]++;
    }

    printf("[2] Construyendo Arbol de Huffman...\n");
    NodoHuffman *nodos_heap[512];
    int tam_heap = 0;

    for (int i = 0; i < 512; i++) {
        if (frecuencias[i] > 0) {
            nodos_heap[tam_heap++] = nuevo_nodo(i - 256, frecuencias[i]);
        }
    }

    while (tam_heap > 1) {
        for (int i = 0; i < tam_heap - 1; i++) {
            for (int j = i + 1; j < tam_heap; j++) {
                if (nodos_heap[i]->frecuencia > nodos_heap[j]->frecuencia) {
                    NodoHuffman *temp = nodos_heap[i];
                    nodos_heap[i] = nodos_heap[j];
                    nodos_heap[j] = temp;
                }
            }
        }
        
        NodoHuffman *izq = nodos_heap[0];
        NodoHuffman *der = nodos_heap[1];
        
        NodoHuffman *padre = nuevo_nodo(999, izq->frecuencia + der->frecuencia); 
        padre->izq = izq;
        padre->der = der;

        nodos_heap[0] = padre;
        for (int i = 1; i < tam_heap - 1; i++) {
            nodos_heap[i] = nodos_heap[i+1];
        }
        tam_heap--;
    }
    
    NodoHuffman *raiz = nodos_heap[0];

    char tabla_codigos[512][256];
    char buffer_camino[256];
    generar_codigos(raiz, tabla_codigos, buffer_camino, 0);

    printf("[3] Escribiendo bitstream comprimido a disco...\n");
    BitWriter *bw = crear_bit_writer(archivo_salida);
    
    for (int i = 0; i < indice_rle; i++) {
        char *codigo = tabla_codigos[datos_rle[i] + 256];
        escribir_codigo(bw, codigo);
    }

    cerrar_bit_writer(bw);
    free(datos_rle);
    printf("Archivo '%s' generado con exito!\n\n", archivo_salida);
}
