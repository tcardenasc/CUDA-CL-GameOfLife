# CUDA-CL Game Of Life
<!-- Este proyecto es un _template_ que les permitirá guiarse sobre como trabajar con CUDA y OpenCL en CMake. Se incluye código básico para la implementación de un proyecto de suma de vectores, la idea es crear los ejecutables para CPU, CUDA y OpenCL, de tal forma que estos se puedan manipular lo suficiente como para despues ser usados mediante Python para crear distintos experimentos. En este caso, cada programa guarda los datos de interés en un CSV. Puede revisar la carpeta `experiment` para ver como utilizarlos en conjunto.

Puede hacer uso de este _template_ haciendo un fork. -->
El presente proyecto contiene implementaciones del juego de la vida de Conway en CPU, CUDA y OpenCL, con el propósito de evaluar las diferencias y ventajas/desventajas de cada plataforma.

Puede correr y observar las simulaciones desde la terminal indicando `1` en el argumento de `debug` al ejecutar cada versión.

## Makefile
Hay un Makefile para trabajar más fácil con los siguientes comandos:
- all: Construye los ejecutables para CUDA, OpenCL y CPU.
- init: Inicializa el directorio de `build` utilizando CMake.
- cuda: Construye el ejecutable para CUDA.
- cl: Construye el ejecutable para OpenCL.
- cpu: Construye el ejecutable para CPU.
- test: Ejecuta las pruebas utilizando CTest.
- clean: Elimina los artefactos de construcción y los directorios de pruebas.
- watch: Monitorea los archivos fuente en busca de cambios y desencadena una construcción cuando se detecta un cambio.

## Ejecución
Desde el mismo directorio `CUDA-CL-GameOfLife`.
### Versión CPU
`build/src/MyProjectCPU <world width> <world height> <iterations> <output_file> <0|1 (debug)> <0|1 (if)>`
#### Ejemplo
`build/src/MyProjectCPU 32 16 500 out.txt 1 0`
```
Iteration: 500
□□□□□□□□□□□□□□□□□□□□□■□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□■□□□□□□□□□□
□□□□□□□■■■□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□■■■□□□■■■□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□■□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□■□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□■□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□■■□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□■■□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□■□□□□□□□□□□
RESULTS: 
Time to create data: 8 microseconds
Time to execute kernel: 1198305 microseconds
Time to execute the whole program: 1198313 microseconds
Data written to out.txt
```
### Versión Open CL
`build/src/cl/MyProjectCL <world width> <world height> <block size> <iterations> <output file> <0|1 (debug)> <0|1 (if)>`
#### Ejemplo
`build/src/cl/MyProjectCL 32 16 128 500 out.txt 1 0`
```
Iteration: 500
□□□□■□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
■□□□□□□□■■□□□□□□□□□□□□□□□□□□□□□■
□■□□□□□■□□■□□□□□■■□□□□□□□□□□□□■□
■□□□□□□□■■□□□□□■□□■□□□□□□□□□□□□■
□□□□□□□□□□□□□□□□■■□□□□□□□□□□□□□□
□□□□■□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□■□■□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□■□■□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□■□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□■■□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□■■□□
□□□□■□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□■□■□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□■□■□□□□□□□□□□□□□□□□□□□□□□□□□□
RESULTS: 
Time to create data: 8 microseconds
Time to execute kernel: 1308667 microseconds
Time to execute the whole program: 1308821 microseconds
Data written to out.txt
```
### Versión CUDA
`build/src/cuda/MyProjectCUDA <world width> <world height> <block size> <iterations> <output file> <0|1 (debug)> <0|1 (if)>`
#### Ejemplo
`build/src/cuda/MyProjectCUDA 32 16 128 500 out.txt 1 0`
```
Iteration: 500
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□■□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□■□■□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□■■□□□■■□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□■■□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□■■□□□□□□□□□□□□□□□□□□□■■
□□□□■■□□□■■□□□□□□□□□□□□□□□□□□■□■
□□□□■■□□□□□□□□□□□□□□□□□□□□□□□□■□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□□
□□□□□□□□□□□□□□□□□□□□□□□□□■■□□□□□
■■■□□□□□□□□□□□□□□□□□□□□□□■■□□□□□
RESULTS: 
Time to create data: 8 microseconds
Time to copy data to device: 18 microseconds
Time to execute kernel: 1371543 microseconds
Time to copy data to host: 44 microseconds
Time to execute the whole program: 1371613 microseconds
Data written to out.txt
```