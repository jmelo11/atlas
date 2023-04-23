# Atlas: Libreria Financiera (alpha version)

El objetivo de Atlas es convertirse en una alternativa real a las herramientas financieras existentes en el mercado, ofreciendo un mayor rendimiento, simplicidad y alcance.

- Performance: En cuanto al rendimiento, hasta el día de hoy, la única alternativa utilizable en sistemas productivos es QuantLib, que cuenta con un sinfín de herramientas para valorizar diversos instrumentos. Sin embargo, es importante mencionar que QuantLib no está pensada a ser rápida (no cuenta con multithreading por defecto, por ejemplo), sino que se enfoca en la flexibilidad. Creemos que hay un espacio en el mercado donde Atlas puede diferenciarse al reducir el alcance de herramientas disponibles y enfocarse en la velocidad, construyéndose desde un inicio con ese objetivo.

- Simplicidad: La simplicidad es otro aspecto clave de Atlas. Se busca tener una herramienta sencilla pero práctica, para que los usuarios puedan aprender a usarla fácilmente, y lo suficientemente flexible para implementar los instrumentos financieros más avanzados del mercado. 

Algunas características técnicas que se buscan incorporar en Atlas son:

- Multithreading: contar con una estructura que permita utilizar la librería de forma thread-safe es crucial para poder sacarle el máximo de rendimiento.
- Diferenciación automática: AAD se ha convertido en un estándar para el cálculo de sensibilidades en el mundo financiero, habilitando la posibilidad de contar con riesgos de forma automática sin tener que recalcular todos nuestros instrumentos. Esta funcionalidad es crucial si, por ejemplo, se busca calcular XVAs u otras métricas.


## Roadmap 

### Generales 

- [x] Estructura básica de las clases (en desarrollo)
- [x] Diferenciacion automatica
- [x] Paralelización CPU
- [ ] Port a Python
- [ ] Compresión de portafolio
- [ ] Precisión monedas
- [ ] Paralelización GPU (depende de factibilidad)
- [ ] Calendarios avanzados (incorporar parámetros QL)

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
        - [ ] Forward
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


## Como compilar el proyecto 

Para poder compilar el proyecto es necesario tener una version de Boost y QuantLib descargadas y compiladas, ya que esta libreria se apalacan aun en algunas herramientas ya desarrolladas en estas dos librerias. El plan a futuro es ir remplazando por versiones internas consistentes con los objetivos del proyecto. Algunas cosas son:

- Date
- DayCounter, Calendar, Frequency
- DiscountCurve, FlatForward

Tambien, es necesario contar con CMAKE. A continuacion se presenta un script de ejemplo para poder compilar:
 
```
    mkdir build && cd build
    cmake .. -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<your/quantlib/install/path>' -DBoost_INCLUDE_DIR='<your/boost/path>'
    cmake --build . --target INSTALL --config Release
```
