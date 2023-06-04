#ifndef CD9BD627_F812_406D_AEEC_B699240E41D8
#define CD9BD627_F812_406D_AEEC_B699240E41D8

// pybind11
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// visitors
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/durationcalculator.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <atlas/visitors/zspreadcalculator.hpp>

namespace py = pybind11;
using namespace Atlas;

void py_visitors(py::module& m) {
    py::class_<Visitor<dual>>(m, "Visitor")
        .def(py::init<>())
        .def("visit", py::overload_cast<FloatingRateInstrument<dual>&>(&Visitor<dual>::visit))
        .def("visit", py::overload_cast<FixedRateInstrument<dual>&>(&Visitor<dual>::visit))
        .def("visit", py::overload_cast<FxForward<dual>&>(&Visitor<dual>::visit))
        .def("visit", py::overload_cast<FixFloatSwap<dual>&>(&Visitor<dual>::visit));

    py::class_<ConstVisitor<dual>>(m, "ConstVisitor")
        .def(py::init<>())
        .def("visit", py::overload_cast<const FloatingRateInstrument<dual>&>(&ConstVisitor<dual>::visit, py::const_))
        .def("visit", py::overload_cast<const FixedRateInstrument<dual>&>(&ConstVisitor<dual>::visit, py::const_))
        .def("visit", py::overload_cast<const FxForward<dual>&>(&ConstVisitor<dual>::visit, py::const_))
        .def("visit", py::overload_cast<const FixFloatSwap<dual>&>(&ConstVisitor<dual>::visit, py::const_));

    py::class_<NPVCalculator<dual>, Visitor<dual>>(m, "NPVCalculator")
        .def(py::init<const MarketData<dual>&>(), py::arg("marketData"))
        .def("results", &NPVCalculator<dual>::results)
        .def("clear", &NPVCalculator<dual>::clear);

    py::class_<Indexer<dual>, Visitor<dual>>(m, "Indexer")
        .def(py::init<>())
        .def("request", &Indexer<dual>::request)
        .def("clear", &Indexer<dual>::clear);

    py::class_<ParSolver<dual>, ConstVisitor<dual>>(m, "ParSolver")
        .def(py::init<const MarketData<dual>&>(), py::arg("marketData"))
        .def("results", &ParSolver<dual>::results)
        .def("clear", &ParSolver<dual>::clear);

    py::class_<ZSpreadCalculator<dual>, ConstVisitor<dual>>(m, "ZSpreadCalculator")
        .def(py::init<const MarketData<dual>&, dual, const DayCounter&, Compounding, Frequency>(), py::arg("marketData"), py::arg("targetNpv"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Compounded, py::arg("frequency") = Frequency::Semiannual)
        .def("results", &ZSpreadCalculator<dual>::results)
        .def("clear", &ZSpreadCalculator<dual>::clear);

    py::class_<CashflowProfiler<dual>, ConstVisitor<dual>>(m, "CashflowProfiler")
        .def(py::init<>())
        .def("redemptions", &CashflowProfiler<dual>::redemptions)
        .def("interests", &CashflowProfiler<dual>::interests)
        .def("clear", &CashflowProfiler<dual>::clear);
}

#endif /* CD9BD627_F812_406D_AEEC_B699240E41D8 */
