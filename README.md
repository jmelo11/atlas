# Atlas: Libreria de pricing simplificada #

## Como compilar el proyecto ##


    mkdir build && cd build
    cmake .. -DCMAKE_CXX_STANDARD=20 -DCMAKE_INSTALL_PREFIX='C:\Users\bloomberg\Desktop\Desarrollo\builds\atlas' -DCMAKE_PREFIX_PATH='C:\Users\bloomberg\Desktop\Desarrollo\builds' -DBoost_INCLUDE_DIR='C:\Users\bloomberg\Desktop\Desarrollo\builds\boost' 
    cmake --build . --target INSTALL --config Release

