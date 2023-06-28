#ifndef E5FDDD37_823C_4981_84F9_793DB88D909E
#define E5FDDD37_823C_4981_84F9_793DB88D909E

#include "config.hpp"
#include <atlas/atlasconfig.hpp>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

void py_currencies(py::module& m) {
    py::class_<Rounding>(m, "Rounding").def("__call__", &Rounding::operator());

    py::class_<Currency>(m, "Currency")
        .def("name", &Currency::name)
        .def("code", &Currency::code)
        .def("numericCode", &Currency::numericCode)
        .def("symbol", &Currency::symbol)
        .def("rounding", &Currency::rounding);

    py::class_<USD, Currency>(m, "USD").def(py::init<>());
    py::class_<CLP, Currency>(m, "CLP").def(py::init<>());
    py::class_<EUR, Currency>(m, "EUR").def(py::init<>());
    py::class_<GBP, Currency>(m, "GBP").def(py::init<>());
    py::class_<JPY, Currency>(m, "JPY").def(py::init<>());
    py::class_<MXN, Currency>(m, "MXN").def(py::init<>());
    py::class_<NOK, Currency>(m, "NOK").def(py::init<>());
    py::class_<SEK, Currency>(m, "SEK").def(py::init<>());
    py::class_<CHF, Currency>(m, "CHF").def(py::init<>());
    py::class_<CAD, Currency>(m, "CAD").def(py::init<>());
    py::class_<AUD, Currency>(m, "AUD").def(py::init<>());
    py::class_<NZD, Currency>(m, "NZD").def(py::init<>());
    py::class_<CNY, Currency>(m, "CNY").def(py::init<>());
    py::class_<HKD, Currency>(m, "HKD").def(py::init<>());
    py::class_<CLF, Currency>(m, "CLF").def(py::init<>());
};

#endif /* E5FDDD37_823C_4981_84F9_793DB88D909E */
