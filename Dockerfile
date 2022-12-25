ARG tag=latest
FROM quantlibparser:${tag}

COPY . /atlas

WORKDIR /atlas/build.atlas
RUN cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=20 -DCMAKE_INCLUDE_PATH='/usr/local/include'
RUN make -j ${num_cores} && make install 

WORKDIR /atlas/python
RUN pip3 install pybind11 && pip3 install .
RUN cd / && rm -rf atlas && ldconfig

WORKDIR /
CMD bash