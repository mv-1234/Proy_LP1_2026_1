# Compresor y Descompresor de Datos (RLE + Huffman) en C

Este proyecto implementa dos de los algoritmos de compresión de datos más clásicos e importantes en las ciencias de la computación y las telecomunicaciones: **Run-Length Encoding (RLE)** y la **Codificación de Huffman**. El sistema está desarrollado completamente en **C** de forma modular y está diseñado para procesar y optimizar el almacenamiento de archivos de texto y archivos binarios.

## 📋 Descripción del Proyecto

El objetivo principal es ofrecer una herramienta por línea de comandos (CLI) eficiente que permita reducir el tamaño de diversos archivos mediante dos enfoques distintos:
1. **RLE (Run-Length Encoding):** Un algoritmo de compresión sin pérdidas ideal para datos que contienen secuencias repetitivas consecutivas.
2. **Codificación de Huffman:** Un algoritmo basado en la frecuencia de caracteres/bytes que utiliza un árbol binario óptimo (implementado mediante una cola de prioridad o Min-Heap) para asignar códigos de longitud variable.

El programa también incluye un módulo de analíticas que calcula de forma automática métricas críticas de rendimiento para comparar la eficiencia de ambos métodos.

## ✨ Características Clave

* **Compresión Multiformato:** Soporte completo para el procesamiento de archivos de texto plano (`.txt`) y archivos binarios.
* **Cola de Prioridad Eficiente:** Estructura de datos Min-Heap para la construcción óptima del árbol de Huffman.
* **Deserialización Automática:** Serialización del árbol de frecuencias directamente en la cabecera del archivo comprimido (`.huf`) para garantizar una descompresión autónoma.
* **Interfaz de Línea de Comandos (CLI):** Sintaxis limpia y estándar para facilitar la automatización y el uso en terminales Linux/Unix.
* **Módulo de Métricas:** Cálculo en tiempo real del tiempo de ejecución, tamaño original vs. comprimido, y el ratio exacto de compresión.

## 📂 Estructura del Proyecto

El código fuente se encuentra organizado de manera modular siguiendo las mejores prácticas de desarrollo en C:

```text
├── src/
│   ├── main.c           # Punto de entrada y gestión de la CLI
│   ├── huffman.c        # Lógica del árbol, min-heap y codificación de Huffman
│   ├── huffman.h        # Cabeceras y estructuras para Huffman
│   ├── rle.c            # Implementación del algoritmo RLE
│   └── rle.h            # Cabeceras y estructuras para RLE
├── tests/               # Archivos y scripts de prueba
├── Makefile             # Automatización de la compilación
└── README.md            # Documentación del proyecto
