<h1 align="center">
  <strong>COMPRESOR HUFFMAN JPEG</strong>
</h1>

<p align="center">
  Compresor y descompresor de imágenes basado en bloques 8x8
</p>

<p align="center">
  <img src="foto.jpg" width="250" alt="Ejemplo de imagen a comprimir">
</p>

El presente proyecto describe el diseño e implementación de un compresor y descompresor de imágenes en lenguaje C. Utiliza una división en bloques de 8x8 píxeles y aplica el algoritmo de compresión sin pérdida de Huffman, operando mediante la manipulación a nivel de bits para la lectura y escritura de archivos.

## 👥 Roles
* *Integrante 1:* Contreras Calizaya, Alejandro
* *Integrante 2:* Vela Rosales, Mauricio Gonzalo
* *Integrante 3:* Cordova Meza, Diego Angel
* *Integrante 4:* Vega Vasquez, Cesar Manuel

*(Nota: Puedes cambiar "Integrante X" por los roles específicos que hayan asumido en el desarrollo, como QA, Coder, etc., según prefieran).*

## 📎 Recursos
* *Repositorio GitHub:* [Enlace al Repositorio](https://github.com/mv-1234/Proy_LP1_2026_1.git) 
* *Librerías externas:* [stb_image y stb_image_write](https://github.com/nothings/stb)

## 📂 Estructura del Repositorio

```text
Compresor_Huffman/
├── foto.jpg                    # Imagen de prueba original
├── README.md                   # Documentación principal del proyecto
├── Makefile                    # Script de automatización de compilación
├── main.c                      # Punto de entrada y manejo de argumentos (CLI)
├── huffman_jpeg.c              # Lógica de compresión, bloques y lectura/escritura de bits
├── huffman_jpeg.h              # Cabeceras y estructuras (NodoHuffman, BitReader, BitWriter)
├── stb_image.h                 # Librería externa para la carga de imágenes
└── stb_image_write.h           # Librería externa para exportar el resultado decodificado
