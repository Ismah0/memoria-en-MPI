# Práctica: Programación Paralela Híbrida (MPI + OpenMP)
**Equipo:** César Longoria, Moisés Molina e Ismael Pantoja

## Descripción
Programa desarrollado en C++ que utiliza clases para llenar un arreglo dinámico con valores aleatorios y muestra su porcentaje de avance de 10% en 10%. Se implementó un modelo híbrido:
- **MPI:** Para distribuir la ejecución entre procesos.
- **OpenMP:** Para paralelizar el procesamiento de los hilos dentro de cada proceso.
- **Gestión de Memoria:** Uso estricto de arreglos dinámicos mediante `new` y `delete`, sin utilizar contenedores estándar como `std::vector`.

## Requisitos
- Compilador de C++ (MinGW).
- Entorno MS-MPI (Microsoft MPI) instalado.
- Soporte para OpenMP (`-fopenmp`).

## Instrucciones de Ejecución
Abrir la consola (CMD) en la carpeta donde se encuentra el archivo compilado `.exe` y correr los siguientes comandos:

**1. Primera ejecución (Tamaño aleatorio entre 20 y 50 elementos con 4 procesos):**
```bash
mpiexec -n 4 GeneradorArreglo.exe