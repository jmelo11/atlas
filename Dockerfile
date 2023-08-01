# for the latest Amazon Linux 2023 & python 3.10
FROM amazonlinux:latest as base

RUN yum update -y && \
    yum install -y gcc openssl-devel bzip2-devel libffi-devel wget tar gzip zlib zlib-devel gcc-c++ make git

#################### Install Cmake
FROM base as buildcmake
RUN wget https://cmake.org/files/v3.21/cmake-3.21.3.tar.gz

RUN tar -xf cmake-3.21.3.tar.gz && \
    cd cmake-3.21.3 && \
    ./bootstrap && \
    make && \
    make install

RUN rm -rf cmake-3.21.3 cmake-3.21.3.tar.gz
####################

#################### Install Python 3.10 
# Extract and install Python
FROM buildcmake as buildpython
RUN yum install -y python3 python3-devel

# Download Python source
RUN wget https://www.python.org/ftp/python/3.10.4/Python-3.10.4.tgz
RUN tar -xf Python-3.10.4.tgz && \
    cd Python-3.10.4 && \
    ./configure --enable-optimizations && \
    make altinstall

# Clean up Python source
RUN rm -rf Python-3.10.4 Python-3.10.4.tgz
RUN python3.10 -m pip install --upgrade pip
RUN python3.10 -m pip install setuptools wheel pybind11
####################

#################### Install boost 1.82
FROM buildpython as buildboost
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz

RUN tar -xf boost_1_82_0.tar.gz && \
    cd boost_1_82_0 && \
    ./bootstrap.sh && \
    ./b2 install

RUN rm -rf boost_1_82_0.tar.gz boost_1_82_0
####################


#################### Build libs
# Set the build type and install directory as build arguments
FROM buildboost as buildlibs

ARG BUILD_TYPE=Release
ARG INSTALL_DIR='/builds'

# Copy the source code to the container
COPY . /app

# Add the install directory to the path
ENV LIBRARY_PATH=${INSTALL_DIR}/lib:$LIBRARY_PATH
ENV LD_LIBRARY_PATH=${INSTALL_DIR}/lib:$LD_LIBRARY_PATH

# Set the working directory
WORKDIR /app

RUN git submodule update --init --recursive

# Build and install pybind11
WORKDIR /app/repos/pybind11/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
    -DBUILD_TESTING=OFF &&\
    cmake --build . --config ${BUILD_TYPE} --target install -j 12


# Build and install QuantLib
WORKDIR /app/repos/QuantLib/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DQL_BUILD_BENCHMARK=OFF \
    -DQL_BUILD_EXAMPLES=OFF \
    -DQL_BUILD_TEST_SUITE=OFF \
    -DQL_TAGGED_LAYOUT=OFF \
    -DBOOST_ROOT=/usr \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR} && \
    cmake --build . --config ${BUILD_TYPE} --target install -j 12

# Build and install nanobench
WORKDIR /app/repos/nanobench/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR} && \
    cmake --build . --config ${BUILD_TYPE} --target install -j 12

# Build Atlas
WORKDIR /app/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
    -DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
    -DBUILD_BENCHMARK=ON \              
    -DBUILD_EXAMPLES=OFF && \
    cmake --build . --config ${BUILD_TYPE} --target install -j 12

# # Set the working directory for testing
WORKDIR /app/build/test

# Run tests
RUN ctest -C ${BUILD_TYPE}

# Build and upload Python package
WORKDIR /app/python
RUN ldconfig && pip3.10 install . 
CMD ["bash"]


