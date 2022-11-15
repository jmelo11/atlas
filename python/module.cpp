#include "pythonwrapper.h"
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>

namespace py = pybind11;

using namespace std;

using namespace QuantLib;

PYBIND11_MODULE(atlaspython, m) {
    m.doc() = "atlaspython";  // optional module docstring
    py::class_<>
}