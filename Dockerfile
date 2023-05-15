# for the latest Amazon Linux
#FROM amazonlinux:latest

# for uploading to PyPI/aarch64
FROM quay.io/pypa/manylinux_2_28_aarch64:latest

# for uploading to PyPI/x86_64
#FROM quay.io/pypa/manylinux_2_24_x86_64:latest

# Install dependencies
RUN yum update -y && \   
    yum install -y \
        boost-devel \
        ninja-build \
        python3 \
        python3-devel \
        python3-pip \
        cmake \
        git  \
        gcc-c++ 


# Install Python dependencies
RUN pip3 install --upgrade pip
RUN pip3 install setuptools wheel twine pybind11

# Set the build type and install directory as build arguments
ARG BUILD_TYPE=Release
ARG INSTALL_DIR=install

# Copy the source code to the container
COPY . /app

# Add the install directory to the path
ENV LIBRARY_PATH=${INSTALL_DIR}/lib:$LIBRARY_PATH
ENV LD_LIBRARY_PATH=${INSTALL_DIR}/lib:$LD_LIBRARY_PATH

# Set the working directory
WORKDIR /app

# Get repos
RUN git submodule update --init --recursive

# Build and install pybind11
WORKDIR /app/repos/pybind11/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
             -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
             -DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
             -DBUILD_TESTING=OFF \
             -DCMAKE_INSTALL_LIBDIR=lib && \
    cmake --build . --config ${BUILD_TYPE} --target install -j 12

# Build and install XAD
WORKDIR /app/repos/XAD/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
             -DXAD_SIMD_OPTION='SSE' \
             -DXAD_ENABLE_TESTS=OFF \
             -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
             -DCMAKE_PREFIX_PATH=${INSTALL_DIR} && \
    cmake --build . --config ${BUILD_TYPE} --target install -j 12

# Build and install QuantLib
WORKDIR /app/repos/QuantLib/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
             -DQL_BUILD_BENCHMARK=OFF \
             -DQL_BUILD_EXAMPLES=OFF \
             -DQL_BUILD_TEST_SUITE=OFF \
             -DQL_TAGGED_LAYOUT=OFF \
             -DBOOST_ROOT=/usr \
             -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
             -DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
             -DCMAKE_INSTALL_LIBDIR=lib && \
    cmake --build . --config ${BUILD_TYPE} --target install -j 12

# Build Atlas
WORKDIR /app/build
RUN rm -rf * && cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
             -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
             -DCMAKE_PREFIX_PATH=${INSTALL_DIR} \
             -DBUILD_BENCHMARK=OFF \              
             -DBUILD_EXAMPLES=OFF \
             -DCMAKE_INSTALL_LIBDIR=lib && \
    cmake --build . --config ${BUILD_TYPE} --target install -j 12

# Set the working directory for testing
WORKDIR /app/build/test

# Run tests
RUN ctest -C ${BUILD_TYPE}

# Build and upload Python package
WORKDIR /app/python
RUN ldconfig && python3 setup.py build && python3 setup.py install
CMD ["bash"]


