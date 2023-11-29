# Pipeline

- docker build -t boost182 -f docker/lambda/Dockerfile.boost182 .
- docker build -t python310 -f docker/lambda/Dockerfile.python310 .
- docker build -t pybind11 -f docker/lambda/Dockerfile.pybind11 .
- docker build -t quantlib -f docker/lambda/Dockerfile.quantlib .
- docker build -t nanobench -f docker/lambda/Dockerfile.nanobench .
- docker build -t atlas -f docker/lambda/Dockerfile.atlas .
