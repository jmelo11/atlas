# Atlas: Libreria de pricing simplificada #

## Como compilar el proyecto ##


    mkdir build && cd build
    cmake .. -DCMAKE_CXX_STANDARD=20 -DCMAKE_INSTALL_PREFIX='C:\Users\bloomberg\Desktop\Desarrollo\builds' -DCMAKE_PREFIX_PATH='C:\Users\bloomberg\Desktop\Desarrollo\builds' -DBoost_INCLUDE_DIR='C:\Users\bloomberg\Desktop\Desarrollo\builds\boost' -DCMAKE_BUILD_TYPE=Release
    cmake --build . --target INSTALL --config Release -j 18

