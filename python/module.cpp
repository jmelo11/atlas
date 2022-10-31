#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>
#include "pythonwrapper.h"

namespace py = pybind11;
using namespace std;
using namespace atlaspythonwrapper;

PYBIND11_MODULE(atlaspython, m) {
    m.doc() = "atlaspython"; // optional module docstring

    // npv
    m.def("fixedBulletNPV", &pricer<FixedBullet, NPVCalculator>::defineAndCalculate, "NPV of a fixed rate bullet product.", py::arg("json"));
    m.def("equalPaymentNPV", &pricer<EqualPayment, NPVCalculator>::defineAndCalculate, "NPV of a fixed rate equal payment product.", py::arg("json"));
    m.def("fixedCustomStructureRateNPV", &pricer<FixedCustomStructure, NPVCalculator>::defineAndCalculate, "NPV of a fixed rate custom structure product.", py::arg("json"));
    m.def("floatingCustomStructureRateNPV", &pricer<FloatingCustomStructure, NPVCalculator>::defineAndCalculate, "NPV of a floating rate custom structure product.", py::arg("json"));

    //par rates - fixed
    m.def("equalPaymentParRate", &pricer<EqualPayment, ParRateSolver>::defineAndCalculate, "Par rate of a fixed rate equal payment product.", py::arg("json"));
    m.def("fixedBulletParRate", &pricer<FixedBullet, ParRateSolver>::defineAndCalculate, "Par rate of a fixed rate bullet product.", py::arg("json"));
    m.def("fixedCustomStructureParRate", &pricer<FixedCustomStructure, ParRateSolver>::defineAndCalculate, "Par rate of a fixed rate custom structure product.", py::arg("json"));

    //par spread - floating
    m.def("floatingBulletParSpread", &pricer<FloatingBullet, ParRateSolver>::defineAndCalculate, "Par spread of a floating rate bullet product.", py::arg("json"));
    m.def("floatingCustomStructureParSpread", &pricer<FloatingCustomStructure, ParRateSolver>::defineAndCalculate, "Par spread of a floating rate custom structure product.", py::arg("json"));

}