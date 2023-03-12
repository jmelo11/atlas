# Atlas: Libreria Financiera (alpha version)

El objetivo de esta libreria es convertirse en una alternativa real a las herramientas que ya existen en el mercado, diferenciandose en performance, simplicidad y scope. En cuanto a esto, podemos mencionar:

- Performance:
    Hasta el dia de hoy, la unica alternativa utilizable en sistemas productivos es QuantLib, la cual cuenta con un sinfin de herramientas elaboradas para valorizar diversos instrumentos. Sin embargo, es importante mencionar que esta libreria no esta orientada a ser rapida (no cuenta con multithreading por default, por ejemplo), si no que a ser una herramienta flexible. Creo que aqui hay un espacio donde Atlas puede diferenciarse, al reducir el scope de herramientas disponibles, pero con una performance mucho mayor al estar construida desde un inicio pensando en performance.

- Simplicidad:
    En linea con lo anterior, el objetivo es tener una herramienta simple, con la suficiente flexibilidad para poder implementar los instrumentos mas comunes del mercado (instrumentos vanilla), de manera que los usuarios pueden aprender de forma facil a usarla. Idealmente, la libreria deberia tener tambien la suficiente flexibilidad para aquellos usuarios mas expertos que buscan cosas especificas.


Algunas caracteristicas tecnicas que se buscan incorporar por defecto son:

- Multithreading:
    Contar con una estructura que permitar utilizar la libreria de una forma thread-safe es crucial para poder usar el maximo de performance.

- Diferenciacion automatica:
    AAD se ha convertido en un estandar para el calculo de sensibilidades en el mundo financiero, habilitando la posibilidad de contar con riesgos de forma automatica sin tener que recalcular (bump'n revalue) todos nuestros instrumentos. Esta funcionalidad es crucial si, por ejemplo, uno busca calcular XVAs o otras metricas.


## Roadmap 

### Funcionalidades 

- [ ] Estructura basica de las clases (en desarrollo)
- [ ] Diferenciacion automatica
- [x] Valorizacion (NPV)
- [x] Calculo sensibilidad productos (duracion)
- [x] Calculo tasas par

### Instrumentos

- [x] Instrumentos tasa fija (bullet)
- [x] Instrumentos tasa fija (cuotas iguales)
- [x] Instrumentos tasa fija (perfil de pagos custom)
- [x] Instrumentos tasa flotante (bullet)
- [x] Instrumentos tasa flotante (perfil de pagos custom)
- [ ] Derivados
- [ ] Otros renta fija

### Otros de mercado

- [ ] Curvas (con AAD)

## Como compilar el proyecto 

Para poder compilar el proyecto es necesario tener una version de Boost y QuantLib descargadas y compiladas, ya que esta libreria se apalacan aun en algunas herramientas ya desarrolladas en estas dos librerias. El plan a futuro es ir remplazando por versiones internas consistentes con los objetivos del proyecto. Algunas cosas son:

- Fechas
- DayCounters, Calendarios, Frequencias
- Curvas

Tambien, es necesario contar con CMAKE. A continuacion se presenta un script de ejemplo para poder compilar:
 
```
    mkdir build && cd build
    cmake .. -DCMAKE_CXX_STANDARD=20 -DCMAKE_PREFIX_PATH='<your/quantlib/install/path>' -DBoost_INCLUDE_DIR='<your/boost/path>'
    cmake --build . --target INSTALL --config Release
```
