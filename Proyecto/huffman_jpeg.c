#include "huffman_jpeg.h"

// --- [ LIBRERÍA STB PARA ESCRIBIR IMÁGENES ] ---
// Usamos stb_image_write para exportar el resultado decodificado
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Función auxiliar para qsort
int comparar_nodos(const void* a, const void* b) {
    NodoHuffman* n1 = *(NodoHuffman**)a;
    NodoHuffman* n2 = *(NodoHuffman**)b;
    return n1->frecuencia - n2->frecuencia;
}

// Libera la memoria del árbol de Huffman recursivamente
void liberar_arbol(NodoHuffman* raiz) {
    if (!raiz) return;
    liberar_arbol(raiz->izq);
    liberar_arbol(raiz->der);
    free(raiz);
}

// Estructura de bits

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
    if (bw->archivo) fclose(bw->archivo);
    free(bw); 
}

// Lectura de bits

// Prepara la lectura binaria
BitReader* crear_bit_reader(const char *nombre_archivo) {
    BitReader *br = (BitReader*)malloc(sizeof(BitReader));
    br->archivo = fopen(nombre_archivo, "rb");
    br->buffer = 0;
    br->bits_restantes = 0;
    return br;
}

// Extrae un solo bit del archivo comprimido
int leer_bit(BitReader *br) {
    // Si consumimos todos los bits del búfer actual, leemos el siguiente byte
    if (br->bits_restantes == 0) {
        if (fread(&br->buffer, 1, 1, br->archivo) < 1) {
            return -1; // Fin de archivo o error
        }
        br->bits_restantes = 8;
    }
    
    // Extraer el bit más significativo actual 
    int bit = (br->buffer >> (br->bits_restantes - 1)) & 1;
    br->bits_restantes--;
    return bit;
}

// Cierra y libera el lector
void cerrar_bit_reader(BitReader *br) {
    if (br->archivo) fclose(br->archivo);
    free(br);
}

// Construccion del arbol de Huffman

NodoHuffman* nuevo_nodo(int valor, int frec) {
    NodoHuffman* nodo = (NodoHuffman*)malloc(sizeof(NodoHuffman));
    // Validación de puntero para mayor estabilidad
    if (!nodo) {
        printf("Error fatal: No se pudo asignar memoria para un nodo de Huffman.\n");
        exit(1);
    }
    nodo->valor = valor;
    nodo->frecuencia = frec;
    nodo->izq = nodo->der = NULL;
    return nodo;
}

void generar_codigos(NodoHuffman *raiz, char codigos[512][256], char *actual, int prof) {
    if (!raiz) return;
    if (!raiz->izq && !raiz->der) {
        actual[prof] = '\0';
        // El char tiene signo: de [-256, 256] se debe pasar a [0, 512]
        strcpy(codigos[raiz->valor + 256], actual);
        return;
    }
    actual[prof] = '0';
    generar_codigos(raiz->izq, codigos, actual, prof + 1);
    
    actual[prof] = '1';
    generar_codigos(raiz->der, codigos, actual, prof + 1);
}

NodoHuffman* construir_huffman(int frecuencias[512]) {
    NodoHuffman* lista[512];
    int n = 0;
    for (int i = 0; i < 512; i++) {
        if (frecuencias[i] > 0) {
            // Revertir el índice para guardar el valor real del píxel en el nodo
            lista[n++] = nuevo_nodo(i - 256, frecuencias[i]);
        }
    }
    if (n == 0) return NULL;
    while (n > 1) {
        qsort(lista, n, sizeof(NodoHuffman*), comparar_nodos);
        NodoHuffman* izq = lista[0];
        NodoHuffman* der = lista[1];
        // Crear nodo padre con valor nulo (debe ser unsigned)
        NodoHuffman* nuevo = nuevo_nodo(-256, izq->frecuencia + der->frecuencia);
        nuevo->izq = izq;
        nuevo->der = der;
        lista[0] = nuevo;
        lista[1] = lista[n - 1];
        n--;
    }
    return lista[0];
}

// Metodos esenciales: manejo de la imagen como tal

void comprimir_estilo_jpeg(unsigned char *pixeles, int ancho, int alto, int canales, const char *archivo_salida) {
    printf("Iniciando Compresión Estilo JPEG...\n");
    int frecuencias[512] = {0};
    
    // Conteo de frecuencias respetando el recorrido espacial por bloques de 8x8
    // Asegura que la estadística sea coherente con el orden espacial del bitstream.
    for (int y = 0; y < alto; y += 8) {
        for (int x = 0; x < ancho; x += 8) {
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 8; i++) {
                    // Lógica original de 'clamping': repetir píxeles de los bordes para rellenar el bloque
                    int pixel_x = (x + i >= ancho) ? ancho - 1 : x + i;
                    int pixel_y = (y + j >= alto) ? alto - 1 : y + j;
                    int indice_pixel = (pixel_y * ancho + pixel_x) * canales;
                    // Contar frecuencias para cada canal
                    for (int c = 0; c < canales; c++) {
                        frecuencias[pixeles[indice_pixel + c] + 256]++;
                    }
                }
            }
        }
    }

    // Construcción del árbol y generación de códigos
    NodoHuffman* raiz = construir_huffman(frecuencias);
    char codigos[512][256];
    char ruta_actual[256];
    for (int i = 0; i < 512; i++) codigos[i][0] = '\0'; 
    if (raiz) generar_codigos(raiz, codigos, ruta_actual, 0);

    BitWriter *bw = crear_bit_writer(archivo_salida);
    if (!bw || !bw->archivo) return;

    // Escribir la cabecera, crítico para el decompressor
    // Fwrite para guardar los datos binarios tal cual están en memoria
    fwrite(&ancho, sizeof(int), 1, bw->archivo);
    fwrite(&alto, sizeof(int), 1, bw->archivo);
    fwrite(&canales, sizeof(int), 1, bw->archivo);
    fwrite(frecuencias, sizeof(int), 512, bw->archivo);

    // Escritura del bitstream respetando el recorrido espacial
    printf("Generando flujo binario...\n");
    for (int y = 0; y < alto; y += 8) {
        for (int x = 0; x < ancho; x += 8) {
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 8; i++) {
                    int pixel_x = (x + i >= ancho) ? ancho - 1 : x + i;
                    int pixel_y = (y + j >= alto) ? alto - 1 : y + j;
                    int indice = (pixel_y * ancho + pixel_x) * canales;
                    // Escribir el código binario para cada canal del píxel
                    for (int c = 0; c < canales; c++) {
                        escribir_codigo(bw, codigos[pixeles[indice + c] + 256]);
                    }
                }
            }
        }
    }
    cerrar_bit_writer(bw);
    if (raiz) liberar_arbol(raiz);
    printf("Compresión finalizada. Archivo guardado: %s\n", archivo_salida);
}

// Lógica corregida para invertir el proceso de compresión
void decodificar_estilo_jpeg(const char *archivo_comprimido, const char *archivo_salida_imagen) {
    printf("Iniciando Descompresión Estilo JPEG...\n");
    BitReader *br = crear_bit_reader(archivo_comprimido);
    if (!br || !br->archivo) {
        printf("Error fatal: No se pudo abrir el archivo comprimido para lectura.\n");
        return;
    }

    int ancho, alto, canales;
    int frecuencias[512];

    // 1. Leer la metadata de la cabecera
    printf("Leyendo metadata y cabecera...\n");
    if (fread(&ancho, sizeof(int), 1, br->archivo) < 1) {
        printf("Error: Archivo comprimido corrupto o incompleto.\n");
        cerrar_bit_reader(br);
        return;
    }
    fread(&alto, sizeof(int), 1, br->archivo);
    fread(&canales, sizeof(int), 1, br->archivo);
    if (fread(frecuencias, sizeof(int), 512, br->archivo) < 512) {
         printf("Error: Tabla de frecuencias incompleta.\n");
         cerrar_bit_reader(br);
         return;
    }

    // 2. Reconstruir el Árbol de Huffman
    NodoHuffman* raiz = construir_huffman(frecuencias);
    if (!raiz) {
        printf("Error fatal: No se encontraron píxeles válidos para reconstruir el árbol.\n");
        cerrar_bit_reader(br);
        return;
    }

    // 3. Decodificación por Bloques de 8x8 píxeles
    // El orden de los bucles debe ser IDÉNTICO a la compresión.
    // Esto asegura que extraigamos la cantidad correcta de códigos del bitstream.
    printf("Reconstruyendo imagen espacial por bloques de 8x8...\n");
    int total_pixeles = ancho * alto * canales;
    unsigned char *pixeles_output = (unsigned char*)calloc(total_pixeles, 1);
    // Validación de asignación de memoria
    if (!pixeles_output) {
        printf("Error fatal: No se pudo asignar memoria para la imagen de salida (%dx%dx%d).\n", ancho, alto, canales);
        liberar_arbol(raiz);
        cerrar_bit_reader(br);
        return;
    }
    
    for (int y = 0; y < alto; y += 8) {
        for (int x = 0; x < ancho; x += 8) {
            // Procesar cada uno de los 64 elementos que forman este bloque
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 8; i++) {
                    
                    // Decodificación de los códigos de Huffman para cada canal
                    for (int c = 0; c < canales; c++) {
                        NodoHuffman *node = raiz;
                        while (node->izq != NULL && node->der != NULL) {
                            int bit = leer_bit(br);
                            if (bit == -1) {
                                printf("Error inesperado: Fin de archivo alcanzado antes de terminar un código.\n");
                                break; 
                            }
                            if (bit == 0) node = node->izq;
                            else node = node->der;
                        }

                        // Se ha alcanzado un nodo hoja, hemos encontrado el valor del canal.
                        // Lógica de 'clamping' inversa: solo guardamos los píxeles que están dentro de la imagen real.
                        // El compresor escribió códigos de más para rellenar los bloques en los bordes.
                        if (x + i < ancho && y + j < alto) {
                            int coord_x = x + i;
                            int coord_y = y + j;
                            int indice_final = (coord_y * ancho + coord_x) * canales + c;
                            pixeles_output[indice_final] = (unsigned char)node->valor;
                        }
                    }
                }
            }
        }
    }

    cerrar_bit_reader(br);
    liberar_arbol(raiz);

    // 4. Exportar la imagen reconstruida
    printf("Generando archivo JPG final...\n");
    stbi_write_jpg(archivo_salida_imagen, ancho, alto, canales, pixeles_output, 100);
    free(pixeles_output);
    
    printf("Descompresión finalizada con éxito. Imagen guardada como: %s\n", archivo_salida_imagen);
}