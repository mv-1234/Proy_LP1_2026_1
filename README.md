<h1 align="center">
  <strong>P09 — Compresor/descompresor RLE + Huffman</strong>
</h1>

<p align="center">
  Algoritmos clásicos de compresión de datos en C
</p>

<p align="center">
  <img src="metrics_example.png" width="450" alt="Ejemplo de salida de métricas en consola">
</p>

El presente proyecto describe el diseño e implementación de una herramienta por línea de comandos (CLI) capaz de comprimir y descomprimir archivos de texto y binarios. Actúa como un entorno práctico para analizar teórica y empíricamente la eficiencia algorítmica de **Run-Length Encoding (RLE)** y la **Codificación de Huffman**, utilizando serialización de árboles y colas de prioridad (Min-Heap).

## 👥 Roles
* *Integrante 1:* [Apellidos y Nombres] - Perfil en el proyecto (ej. Arquitectura del árbol y CLI)
* *Integrante 2:* [Apellidos y Nombres] - Perfil en el proyecto (ej. Implementación de Huffman y bits)
* *Integrante 3:* [Apellidos y Nombres] - Perfil en el proyecto (ej. Implementación de RLE y Métricas)

## 📎 Recursos
* *Análisis y Diseño (Entregable 1):* [Enlace al documento de requerimientos y casos de uso](#)
* *Documento de Arquitectura:* [Enlace a la tabla comparativa y pruebas reales](#)

## 📂 Estructura del Repositorio

```text
Proyecto_P09/
├── .gitignore                  # Archivos y carpetas ignorados por Git
├── metrics_example.png         # Captura de pantalla de los ratios de compresión obtenidos
├── README.md                   # Documentación principal del proyecto
├── docs/                       # Entregables 1 y 2 (Diseño, arquitectura y tablas comparativas)
├── test_files/                 # Archivos de prueba (texto y binarios/imágenes)
└── src/                        # Código fuente principal en C
    ├── huffman.c               # Lógica del árbol de Huffman, Min-Heap y operaciones bit a bit
    ├── huffman.h               # Cabeceras y estructuras del algoritmo de Huffman
    ├── rle.c                   # Lógica de compresión y descompresión del algoritmo RLE
    ├── rle.h                   # Cabeceras para el módulo RLE
    ├── cli.c                   # Punto de entrada (Main), parseo de argumentos y cálculo de métricas
    ├── stb_image.h             # Librería auxiliar para carga de imágenes en pruebas (opcional)
    ├── stb_image_write.h       # Librería auxiliar para exportar imágenes reconstruidas (opcional)
    └── Makefile                # Script de automatización para compilación (gcc) y limpieza
