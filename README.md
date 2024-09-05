<div align="center">
  
# **_PolyhedronRepair_**

Aplicación gráfica 3D creada para un proyecto de memoria.

Funcionaa tanto con mallas poliedricas como de polígonos, ofreciendo la mayoria de funcionalidades para las primeras.

Permite cargar y guardar mallas, repararlas de forma automática o manual según lo necesites y visualizar las métricas en tiempo real mientras trabajas.
</div>

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

Al momento de escrito esto el software funciona tanto en Windows como en Linux.

## Instalacion

```
# 1. Instalar los prerequisitos (Ubuntu)
sudo apt install build-essential gdb
sudo apt install cmake
sudo apt install libsdl2-dev
sudo apt install libglew-dev
sudo apt-get install libglm-dev

# 2. Clonar el repositorio y sus dependencias directas
git clone --recursive --j8 https://github.com/fcopizarro/Polyhedron-Repair/ 

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

`C`: cambiar entre el modo centrado y el modo libre.

[Modo Libre]
- `WASD`: Mover la camara por el espacio
- `Boton Central del Mouse`: Rotar la camara
- `Click Izquierdo`: Seleccionar vertices en el modo edición

[Modo Centrado]
- 'Boton Central del Mouse`: Rotar la camara sobre el objeto
- `Click Izquierdo`: Seleccionar vertices en el modo edición

  ## Diagramas

# TODO: Hacer un Struct o Class que permita ingresar metricas facilmente.
# TODO: Implementar .vtk nativo

## Clases
