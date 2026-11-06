# Simulador de Máquina de Turing - Regla 110

Este proyecto implementa una **Máquina de Turing (MT)** que simula la evolución del autómata celular de **Regla 110** (Elementary Cellular Automaton Rule 110) a lo largo de $K$ pasos de tiempo especificados en formato binario. 

La aplicación cuenta con aceleración mediante procesamiento en paralelo con **OpenMP** para el cálculo del estado de las celdas y soporta tanto visualizaciones resumidas como **trazas micro-paso a paso** del estado interno del autómata y la posición del cabezal.

---

## 📋 Características Principales

* **Modelo Formal de Máquina de Turing:** Implementación explícita de estados ($q_{START}$, $q_{DEC\_K}$, $q_{SCAN}$, $q_{EVAL\_R110}$, $q_{HALT}$) y control de posición del cabezal.
* **Soporte Paralelepipédico / OpenMP:** Evaluación paralela multihilo del vecindario de 3 celdas de la Regla 110 mediante `#pragma omp parallel for`.
* **Modos de Ejecución:**
  * **Modo Resumen:** Muestra la evolución generación por generación ($Gen\ 0 \dots Gen\ K$).
  * **Modo Traza Detallada:** Imprime el movimiento celda por celda del cabezal en la cinta binaria, mostrando la función de transición $\delta$ y los cambios de estado.
* **Procesamiento Por Lotes:** Capacidad de cargar múltiples casos de prueba desde archivos de texto (`.txt`).
* **Interfaz de Menú Interactivo:** Menú CLI sencillo para seleccionar hileras manuales, archivos o ejemplos prefijados.

---

## 🛠️ Requisitos del Sistema

* **Compilador C++:** `g++` con soporte para el estándar **C++17** (o superior).
* **OpenMP:** Biblioteca instalada en el sistema para permitir la paralelizacion multihilo (`-fopenmp`).
* **Herramienta Make:** `make` para utilizar el archivo de construcción incluido.

En sistemas basados en Linux/Ubuntu:
```bash
sudo apt update
sudo apt install build-essential libomp-dev
```

---

## ⚙️ Compilación e Instalación

El proyecto incluye un `Makefile` optimizado (`-O3`, `-Wall`, `-Wextra`, `-fopenmp`).

Para compilar el proyecto ejecuta:

```bash
make
```

Esto generará el ejecutable binario llamado `sim_regla110`.

Para limpiar los objetos generados (`.o`) y el ejecutable:

```bash
make clean
```

---

## 🚀 Uso del Programa

### 1. Modo Interactivo (Menú)
Puedes iniciar la aplicación directamente sin argumentos:

```bash
./sim_regla110
```

Se desplegará el menú principal con las siguientes opciones:
1. **Ingresar cadena manualmente (Modo Resumen):** Muestra los estados finales de cada generación.
2. **Ingresar cadena manualmente (Modo Traza Detallada):** Muestra el desplazamiento del cabezal paso por paso sobre la cinta.
3. **Cargar casos de prueba desde archivo:** Procesa un archivo `.txt` que contiene múltiples hileras.
4. **Ejecutar ejemplos integrados:** Caso de prueba por defecto de la consigna.
5. **Salir.**

### 2. Modo Directo por Archivo
Puedes pasar como parámetro la ruta de un archivo de texto con casos de prueba al ejecutar el programa:

```bash
./sim_regla110 casos.txt
```

---

## 📥 Formato de la Hilera de Entrada

La hilera formateada en la cinta debe cumplir la siguiente sintaxis:

$$\text{K\_binario\#cinta\_inicial\#}$$

* **`K_binario`**: Representación binaria del número de iteraciones a ejecutar (ej. `100` equivale a 4 iteraciones en decimal).
* **Primer `#`**: Delimitador que separa el contador $K$ de la cinta del autómata.
* **`cinta_inicial`**: Cadena binaria de longitud $N$ ($N \ge 8$) compuesta por `0` y `1`. Las condiciones de frontera son circulares.
* **Segundo `#`**: Delimitador final de la cinta.

### Ejemplo de entrada:
```text
100#000000010000000#
```

---

## 📐 Regla 110 (Mapeo de Vecindades)

La Regla 110 evalúa cada celda según su estado actual y el de sus vecinos adyacentes a la izquierda y derecha ($L, C, R$):

| Vecindad ($LCR$) | Nuevo Estado |
| :---: | :---: |
| `111` | `0` |
| `110` | `1` |
| `101` | `1` |
| `100` | `0` |
| `011` | `1` |
| `010` | `1` |
| `001` | `1` |
| `000` | `0` |

---

## 📂 Estructura del Código

```text
.
├── Makefile             # Instrucciones de compilación
├── Rule110.h / .cpp     # Lógica pura de evaluación del autómata celular
├── TuringMachine.h / .cpp# Clase principal con la máquina de estados y cinta
└── main.cpp             # Entrada del programa, menú CLI y manejo de archivos
```

* **`Rule110`**: Clase utilitaria estática para mapear la vecindad de 3 bits a la respuesta binaria de la Regla 110.
* **`TuringMachineRule110`**: Clase contenedora de la lógica del autómata formal: administración de la cinta completa, decremento en binario del parámetro $K$, posición del cabezal `^` y paralelización de estados con OpenMP.
* **`main`**: Control de flujo, manejo del Menú interactivo y carga de entradas desde terminal o disco.