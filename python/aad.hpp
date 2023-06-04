#ifndef CEA28FC0_98E3_4B84_BDE0_965AF48F38CB
#define CEA28FC0_98E3_4B84_BDE0_965AF48F38CB

#include <atlas/atlasdefine.hpp>
// pybind11
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

void py_aad(py::module& m) {
    // autodiff
    m.def("getValue", [](const dual& d) { return val(d); });

    py::class_<dual>(m, "Dual")
        .def(py::init<double>())
        .def("__str__", [](const dual& d) { return "Dual(" + std::to_string(val(d)) + ")"; })
        .def("__repr__", [](const dual& d) { return "Dual(" + std::to_string(val(d)) + ")"; })
        .def("__float__", [](const dual& d) { return val(d); })
        .def("setDerivative", py::overload_cast<double>(&dual::setDerivative))
        .def("getDerivative", py::overload_cast<>(&dual::getDerivative, py::const_));

    py::class_<tape_type, std::shared_ptr<tape_type>>(m, "Tape")
        .def(py::init<>())
        .def("registerInput", py::overload_cast<dual&>(&tape_type::registerInput))
        .def("registerOutput", py::overload_cast<dual&>(&tape_type::registerOutput))
        .def("computeAdjoints", &tape_type::computeAdjoints)
        .def("newRecording", &tape_type::newRecording)
        .def("clearAll", &tape_type::clearAll)
        .def("getMemory", &tape_type::getMemory)
        .def("clearDerivatives", &tape_type::clearDerivatives)
        .def("printStatus", &tape_type::printStatus);
}

#endif /* CEA28FC0_98E3_4B84_BDE0_965AF48F38CB */
