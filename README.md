# Atlas: Libreria Financiera

El objetivo de Atlas es convertirse en una alternativa real a las herramientas financieras existentes en el mercado, ofreciendo un mayor rendimiento, simplicidad y alcance.

- Performance: En cuanto al rendimiento, hasta el día de hoy, la única alternativa utilizable en sistemas productivos es QuantLib, que cuenta con un sinfín de herramientas para valorizar diversos instrumentos. Sin embargo, es importante mencionar que QuantLib no está pensada a ser rápida (no cuenta con multithreading por defecto, por ejemplo), sino que se enfoca en la flexibilidad. Creemos que hay un espacio en el mercado donde Atlas puede diferenciarse al reducir el alcance de herramientas disponibles y enfocarse en la velocidad, construyéndose desde un inicio con ese objetivo.

- Simplicidad: La simplicidad es otro aspecto clave de Atlas. Se busca tener una herramienta sencilla pero práctica, para que los usuarios puedan aprender a usarla fácilmente, y lo suficientemente flexible para implementar los instrumentos financieros más avanzados del mercado.

Algunas características técnicas que se buscan incorporar en Atlas son:

- Multithreading: contar con una estructura que permita utilizar la librería de forma thread-safe es crucial para poder sacarle el máximo de rendimiento.

- Diferenciación automática: AAD se ha convertido en un estándar para el cálculo de sensibilidades en el mundo financiero, habilitando la posibilidad de contar con riesgos de forma automática sin tener que recalcular todos nuestros instrumentos. Esta funcionalidad es crucial si, por ejemplo, se busca calcular XVAs u otras métricas.

## Documentación

Es posible encontrar la documentación disponible hasta hoy en <https://jmelo11.github.io/atlasdocs/>

## Roadmap

### Generales

- [x] Estructura básica de las clases (en desarrollo)
- [x] Diferenciacion automatica
- [x] Paralelización CPU
- [x] Port a Python
- [ ] Compresión de portafolio
- [x] Transformación a moneda local
- [x] Precisión monedas
- [ ] Paralelización GPU (depende de factibilidad)
- [ ] Customización instrumentos (incorporar parámetros QL)

### Funcionalidades

- [x] NPV
- [x] Sensibilidades (ej. Duracion)
- [x] Tasas Par
- [x] ZSpread

### Instrumentos

- [x] Productos lineales
  - [x] Loans
    - [x] Loans tasa fija (bullet)
    - [x] Loans tasa fija (cuotas iguales)
    - [x] Loans tasa fija (perfil de pagos custom)
    - [x] Loans tasa flotante (bullet)
    - [x] Loans tasa flotante (perfil de pagos custom)
    - [x] Loans tasa mixta
  - [ ] Derivados
    - [x] Forward
    - [ ] Swaps vanilla
    - [ ] Xccy
  - [ ] OIS
  - [ ] Renta Fija
    - [ ] Instrumentos con convenciones de mercados

- [ ] Productos no lineales

### Curvas

- [x] Interpolación con soporte AD
- [x] Curvas con soporte AD
- [ ] Bootstrap con soporte AD

### Modelos

- [x] Generación de tasas en base a curvas estáticas
- [ ] LGM para generación de trayectorias de mercado

### Requisitos previos

Antes de compilar el proyecto, es necesario instalar las siguientes bibliotecas:

- Boost: <https://www.boost.org/>
- QuantLib: <https://github.com/lballabio/quantlib>

El objetivo futuro es reemplazar estas dependencias por versiones internas del proyecto.

#### Estructura de directorios recomendada

Para facilitar el proceso de compilación, se sugiere organizar los directorios de la siguiente manera:

- builds
  - release
  - debug
- libs
  - QuantLib
  - boost

#### Compilación de QuantLib con CMake (Windows)

1. Dentro del directorio libs/QuantLib, cree una carpeta llamada build.
2. Abra una terminal y navegue hasta la carpeta build recién creada.
3. Ejecute los siguientes comandos, reemplazando <path/to/builds/config> y <your/boost/path> con las rutas apropiadas:

##### Debug

```cmd
    cmake -S .. -B . -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<path/to/builds/config>' -DBoost_INCLUDE_DIR='<your/boost/path>' -DQL_BUILD_BENCHMARK=OFF -DQL_BUILD_EXAMPLES=OFF -DQL_BUILD_TEST_SUITE=OFF -DCMAKE_CXX_FLAGS="/MDd /EHsc /MP" -DQL_TAGGED_LAYOUT=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebugDLL"

    cmake --build . --target INSTALL --config <config>
```

##### Release

```cmd
    cmake -S .. -B . -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<path/to/builds/config>' -DBoost_INCLUDE_DIR='<your/boost/path>' -DQL_BUILD_BENCHMARK=OFF -DQL_BUILD_EXAMPLES=OFF -DQL_BUILD_TEST_SUITE=OFF -DCMAKE_CXX_FLAGS="/MD /EHsc /MP" -DQL_TAGGED_LAYOUT=OFF -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDLL"

    cmake --build . --target INSTALL --config <config>
```

Donde config puede ser Debug o Release, dependiendo de la configuración que desee compilar.

#### Instalación de Boost

Boost incluye un instalador. Siga las instrucciones en <https://www.boost.org/doc/libs/1_82_0/more/getting_started/windows.html> para instalarlo en su sistema.

#### Compilación de Atlas

1. Dentro del directorio del proyecto Atlas, cree una carpeta llamada build.
2. Abra una terminal y navegue hasta la carpeta build recién creada.
3. Ejecute los siguientes comandos, reemplazando ```path/to/builds/config``` y ```your/boost/path``` con las rutas apropiadas:

```cmd
    cmake -S .. -B . -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<path/to/builds/config>' -DBoost_INCLUDE_DIR='<your/boost/path>'
    cmake --build . --target INSTALL --config <config>
```

#### Instalación modulo Python

En el caso de querer instalar el paquete desde este repositorio, es necesario configurar el archivo ```setup.py```. En este caso, sera necesario tambien contar con pybind11 instalado. Por consistencia se recomienda utilizar CMAKE para esto, ya que de esta forma funcionará el comando ```find_package```. Una vez instalada y seleccionada la opcion ```BUID_PYTHON=ON``` al configurar Atlas, bastara con:

1. Ir con la terminal a la carpeta ```python```
2. Ejectura el comando ```pip install .```

Tambien es posible instalar el paquete estandar a travez de PYPI:
```pip instal atlas-finance```
