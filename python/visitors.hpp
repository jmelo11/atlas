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
#include <atlas/visitors/newvisitors/basevisitor.hpp>
#include <atlas/visitors/newvisitors/fixingvisitor.hpp>
#include <atlas/visitors/newvisitors/indexingvisitor.hpp>
#include <atlas/visitors/newvisitors/npvconstvisitor.hpp>
#include <atlas/visitors/newvisitors/parrateconstvisitor.hpp>
#include <atlas/visitors/newvisitors/zspreadconstvisitor.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <atlas/visitors/zspreadcalculator.hpp>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyBaseVisitor : public BaseVisitor<dual> {
       public:
        using BaseVisitor<dual>::BaseVisitor;

        void operator()(CustomFixedRateInstrument<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }

        void operator()(EqualPaymentInstrument<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }

        void operator()(FixedRateBulletInstrument<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }

        void operator()(ZeroCouponInstrument<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }

        void operator()(CustomFloatingRateInstrument<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }

        void operator()(FloatingRateBulletInstrument<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }

        void operator()(FloatingRateEqualRedemptionInstrument<dual>& inst) override {
            PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst);
        }

        void operator()(FxForward<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }

        void operator()(FixFloatSwap<dual>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<dual>, operator(), inst); }
    };

    class PyBaseConstVisitor : public BaseConstVisitor<dual> {
       public:
        using BaseConstVisitor<dual>::BaseConstVisitor;

        void operator()(const CustomFixedRateInstrument<dual>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst);
        }

        void operator()(const EqualPaymentInstrument<dual>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst);
        }

        void operator()(const FixedRateBulletInstrument<dual>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst);
        }

        void operator()(const ZeroCouponInstrument<dual>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst);
        }

        void operator()(const CustomFloatingRateInstrument<dual>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst);
        }

        void operator()(const FloatingRateBulletInstrument<dual>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst);
        }

        void operator()(const FloatingRateEqualRedemptionInstrument<dual>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst);
        }

        void operator()(const FxForward<dual>& inst) const override { PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst); }

        void operator()(const FixFloatSwap<dual>& inst) const override { PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<dual>, operator(), inst); }
    };
};  // namespace Aux

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

void py_newvisitors(py::module& m) {
    py::class_<Instruments<dual>>(m, "Instruments").def(py::init<>());

    py::class_<BaseVisitor<dual>, Aux::PyBaseVisitor>(m, "BaseVisitor")
        .def("__call__", py::overload_cast<CustomFixedRateInstrument<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<EqualPaymentInstrument<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<FixedRateBulletInstrument<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<ZeroCouponInstrument<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<CustomFloatingRateInstrument<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<FloatingRateBulletInstrument<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<FloatingRateEqualRedemptionInstrument<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<FxForward<dual>&>(&BaseVisitor<dual>::operator()))
        .def("__call__", py::overload_cast<FixFloatSwap<dual>&>(&BaseVisitor<dual>::operator()))
        .def("visit", [](BaseVisitor<dual>& v, Instruments<dual>& i) { std::visit(v, i); });

    py::class_<BaseConstVisitor<dual>, Aux::PyBaseConstVisitor>(m, "BaseConstVisitor")
        .def("__call__", py::overload_cast<const CustomFixedRateInstrument<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const EqualPaymentInstrument<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FixedRateBulletInstrument<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const ZeroCouponInstrument<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const CustomFloatingRateInstrument<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FloatingRateBulletInstrument<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FloatingRateEqualRedemptionInstrument<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FxForward<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FixFloatSwap<dual>&>(&BaseConstVisitor<dual>::operator(), py::const_))
        .def("visit", [](BaseConstVisitor<dual>& v, const Instruments<dual>& i) { std::visit(v, i); });

    py::class_<IndexingVisitor<dual>, BaseVisitor<dual>>(m, "IndexingVisitor")
        .def(py::init<>())
        .def("getResults", &IndexingVisitor<dual>::getResults)
        .def("reset", &IndexingVisitor<dual>::reset);

    py::class_<FixingVisitor<dual>, BaseVisitor<dual>>(m, "FixingVisitor").def(py::init<const MarketData<dual>&>());

    py::class_<NPVConstVisitor<dual>, BaseConstVisitor<dual>>(m, "NPVConstVisitor")
        .def(py::init<const MarketData<dual>&>(), py::arg("marketData"))
        .def("getResults", &NPVConstVisitor<dual>::getResults)
        .def("reset", &NPVConstVisitor<dual>::reset);
}

#endif /* CD9BD627_F812_406D_AEEC_B699240E41D8 */
