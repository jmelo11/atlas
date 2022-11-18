#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>

namespace py = pybind11;

using namespace std;

PYBIND11_MODULE(atlaspython, m) {
    m.doc() = "atlaspython";  // optional module docstring
}