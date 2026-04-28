#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR 256
typedef struct Nodo {
    char caracter;
    int frecuencia;
    struct Nodo *izq, *der;
} Nodo;
Nodo* crearNodo(char c, int f) {
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->caracter = c;
    nodo->frecuencia = f;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}
int comparar(const void* a, const void* b) {
    Nodo* n1 = *(Nodo**)a;
    Nodo* n2 = *(Nodo**)b;
    return n1->frecuencia - n2->frecuencia;
}
Nodo* construirHuffman(char* texto) {
    int freq[MAX_CHAR] = {0};
    for (int i = 0; texto[i]; i++) {
        freq[texto[i]]++;
    }
    Nodo* lista[MAX_CHAR];
    int n = 0;
    for (int j = 0; j < MAX_CHAR; j++) {
        if (freq[j] > 0) {
            lista[n++] = crearNodo(j, freq[j]);
        }
    }
    while (n > 1) {
        qsort(lista, n, sizeof(Nodo*), comparar);
        Nodo* izq = lista[0];
        Nodo* der = lista[1];
        Nodo* nuevo = crearNodo('\0', izq->frecuencia + der->frecuencia);
        nuevo->izq = izq;
        nuevo->der = der;
        lista[0] = nuevo;
        lista[1] = lista[n - 1];
        n--;
    }

    return lista[0];
}
void imprimirCodigos(Nodo* raiz, char* codigo, int profundidad) {
    if (!raiz) return;
    if (!raiz->izq && !raiz->der) {
        codigo[profundidad] = '\0';
        printf("'%c': %s\n", raiz->caracter, codigo);
        return;
    }
    codigo[profundidad] = '0';
    imprimirCodigos(raiz->izq, codigo, profundidad + 1);
    codigo[profundidad] = '1';
    imprimirCodigos(raiz->der, codigo, profundidad + 1);
}

int main() {
    char texto[256];
    printf("Ingrese una linea: ");
    fgets(texto, sizeof(texto), stdin);
    texto[strcspn(texto, "\n")] = '\0';
    Nodo* raiz = construirHuffman(texto);
    char codigo[256];
    printf("\nCodigos Huffman:\n");
    imprimirCodigos(raiz, codigo, 0);
    return 0;
}
