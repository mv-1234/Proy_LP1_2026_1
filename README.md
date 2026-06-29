# Compresor y Descompresor de Imágenes (Codificación Huffman)

Este proyecto implementa un compresor y descompresor de imágenes en el lenguaje C, inspirado en el funcionamiento básico del estándar JPEG. El programa divide la imagen en bloques de 8x8 píxeles y aplica compresión sin pérdida utilizando el algoritmo de **Codificación de Huffman**.

Proyecto desarrollado como parte del trabajo académico para la **Universidad Nacional de Ingeniería (UNI)**.

## 🚀 Características

* **Compresión basada en bloques:** Procesa la imagen dividiéndola en matrices de 8x8.
* **Codificación de Huffman:** Construye un árbol de frecuencias para reducir el tamaño de almacenamiento de los píxeles.
* **Soporte de formatos:** Capaz de leer múltiples formatos de entrada (gracias a `stb_image`) y exportar la imagen reconstruida en formato JPG (vía `stb_image_write`).
* **Gestión de memoria a nivel de bits:** Implementación de estructuras `BitWriter` y `BitReader` para manipular eficientemente la escritura y lectura en los archivos comprimidos (`.huf`).

## 📁 Estructura del Proyecto

* `main.c`: Punto de entrada del programa. Maneja los argumentos de la línea de comandos e invoca las funciones principales.
* `huffman_jpeg.c` / `huffman_jpeg.h`: Contiene la lógica del algoritmo de Huffman, división en bloques y manejo de lectura/escritura de bits.
* `stb_image.h`: Librería *header-only* utilizada para cargar las imágenes originales.
* `stb_image_write.h`: Librería *header-only* utilizada para guardar el resultado de la descompresión.
* `Makefile`: Script para compilar el proyecto fácilmente.

## 🛠️ Requisitos e Instalación

Para compilar el proyecto, necesitas tener instalado el compilador **GCC** y la herramienta **Make**.

1. Clona este repositorio:
   ```bash
   git clone <URL_DE_TU_REPOSITORIO>
   cd <NOMBRE_DE_LA_CARPETA>
