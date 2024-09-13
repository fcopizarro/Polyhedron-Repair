<div align="center">
  
# **_PolyhedronRepair_**

Aplicación gráfica 3D creada para un proyecto de memoria.

Funciona tanto con mallas poliedricas como de polígonos, ofreciendo la mayoria de funcionalidades para las primeras.

Permite cargar y guardar mallas, repararlas de forma automática o manual según lo necesites y visualizar las métricas en tiempo real mientras trabajas.
</div>

**Nota:** Se puede ver la documentacion en docs/html/index.html

## Aclaración

De momento solo se trabajan con 5 metricas:

- Scaled Jacobian
- Element Normalized Scaled Jacobian
- Aspect Ratio
- Aspect Ratio Gamma
- Element Normalized Aspect Ratio

Y con los siguientes formatos:

- .vtk
- .obj

## Compatibilidad

Al momento de escrito esto el software funciona tanto en Windows (10 y 11) como en Linux (probado en el distro Ubuntu 24).

Adicionalmente, fue probado con una targeta gráfica integrada (Intel UHD Graphics), alcanzando unos 20-30 fps en un modelo de mas de 1.000.000 de poliedros.

## Instalacion

```
# 1. Instalar los prerequisitos (Ubuntu)
sudo apt install build-essential gdb
sudo apt install cmake
sudo apt install libsdl2-dev
sudo apt install libglew-dev
sudo apt-get install libglm-dev

# 2. Clonar el repositorio y sus dependencias directas
git clone --recursive -j8 https://github.com/fcopizarro/Polyhedron-Repair/ 

# 3. Configurar y Compilar
cd Polyhedron-Repair
mkdir build
cd build
cmake ..
make

# 4. Ejecutar
./MemoriaIci
```

## Controles

La mayoria de interacciones con la escena se pueden realizar a través de los menus.

Los controles de la camara son:

[Modo Centrado]
- 'Boton Central del Mouse`: Rotar la camara sobre el objeto
- `Click Izquierdo`: Seleccionar vertices en el modo edición


## Más archivos de prueba

 - [10M Hexaedros](https://usmcl-my.sharepoint.com/:u:/g/personal/francisco_pizarroj_usm_cl/ES7QYxDdYspOoNPkDgB4hX4BbzMAgsfkXGfNdojCqbmHOw?e=7SseZZ)
 - [1M Hexaedros](https://usmcl-my.sharepoint.com/:u:/g/personal/francisco_pizarroj_usm_cl/ESyldUEyK2ZAsORfYhUtJp4BxNfJJpyXTl6AtFCubbQ-Dg?e=hZQcGS)
 - [100K Hexaedros](https://usmcl-my.sharepoint.com/:u:/g/personal/francisco_pizarroj_usm_cl/EU4IAqiQ5eJFhUIDpcuywWgB6bayOzOuRih-OgP5YkbSqg?e=bfAvEr)
 - [10K Hexaedros](https://usmcl-my.sharepoint.com/:u:/g/personal/francisco_pizarroj_usm_cl/EXuLmDOZHxxFnVXumQtVIIEBfcQFhcJeN7l4qHturOpCFg?e=janelX)
 - [1K Hexaedros](https://usmcl-my.sharepoint.com/:u:/g/personal/francisco_pizarroj_usm_cl/EW0foyyqUA9BqnGZJQdNyxYBEkhgl8YC8YZmjbhwWj3Zgg?e=zbX3A1)
