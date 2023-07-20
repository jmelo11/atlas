#ifndef CD9BD627_F812_406D_AEEC_B699240E41D8
#define CD9BD627_F812_406D_AEEC_B699240E41D8

#include "config.hpp"
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/durationcalculator.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/newvisitors/basevisitor.hpp>
#include <atlas/visitors/newvisitors/cashflowaggregation/accruedamountconstvisitor.hpp>
#include <atlas/visitors/newvisitors/cashflowaggregation/instrumentcashflowsconstvisitor.hpp>
#include <atlas/visitors/newvisitors/cashflowaggregation/maturingamountconstvisitor.hpp>
#include <atlas/visitors/newvisitors/fixingvisitor.hpp>
#include <atlas/visitors/newvisitors/indexingvisitor.hpp>
#include <atlas/visitors/newvisitors/npvconstvisitor.hpp>
#include <atlas/visitors/newvisitors/parrateconstvisitor.hpp>
#include <atlas/visitors/newvisitors/sensibilityconstvisitor.hpp>
#include <atlas/visitors/newvisitors/zspreadconstvisitor.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <atlas/visitors/zspreadcalculator.hpp>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyBaseVisitor : public BaseVisitor<NumType> {
       public:
        using BaseVisitor<NumType>::BaseVisitor;

        void operator()(CustomFixedRateInstrument<NumType>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst); }

        void operator()(EqualPaymentInstrument<NumType>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst); }

        void operator()(FixedRateBulletInstrument<NumType>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst); }

        void operator()(ZeroCouponInstrument<NumType>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst); }

        void operator()(CustomFloatingRateInstrument<NumType>& inst) override {
            PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst);
        }

        void operator()(FloatingRateBulletInstrument<NumType>& inst) override {
            PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst);
        }

        void operator()(FloatingRateEqualRedemptionInstrument<NumType>& inst) override {
            PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst);
        }

        void operator()(FxForward<NumType>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst); }

        void operator()(FixFloatSwap<NumType>& inst) override { PYBIND11_OVERRIDE_PURE(void, BaseVisitor<NumType>, operator(), inst); }
    };

    class PyBaseConstVisitor : public BaseConstVisitor<NumType> {
       public:
        using BaseConstVisitor<NumType>::BaseConstVisitor;

        void operator()(const CustomFixedRateInstrument<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }

        void operator()(const EqualPaymentInstrument<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }

        void operator()(const FixedRateBulletInstrument<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }

        void operator()(const ZeroCouponInstrument<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }

        void operator()(const CustomFloatingRateInstrument<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }

        void operator()(const FloatingRateBulletInstrument<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }

        void operator()(const FloatingRateEqualRedemptionInstrument<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }

        void operator()(const FxForward<NumType>& inst) const override { PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst); }

        void operator()(const FixFloatSwap<NumType>& inst) const override {
            PYBIND11_OVERRIDE_PURE(void, BaseConstVisitor<NumType>, operator(), inst);
        }
    };
};  // namespace Aux

void py_visitors(py::module& m) {
    py::class_<Visitor<NumType>>(m, "Visitor")
        .def(py::init<>())
        .def("visit", py::overload_cast<FloatingRateInstrument<NumType>&>(&Visitor<NumType>::visit))
        .def("visit", py::overload_cast<FixedRateInstrument<NumType>&>(&Visitor<NumType>::visit))
        .def("visit", py::overload_cast<FxForward<NumType>&>(&Visitor<NumType>::visit))
        .def("visit", py::overload_cast<FixFloatSwap<NumType>&>(&Visitor<NumType>::visit));

    py::class_<ConstVisitor<NumType>>(m, "ConstVisitor")
        .def(py::init<>())
        .def("visit", py::overload_cast<const FloatingRateInstrument<NumType>&>(&ConstVisitor<NumType>::visit, py::const_))
        .def("visit", py::overload_cast<const FixedRateInstrument<NumType>&>(&ConstVisitor<NumType>::visit, py::const_))
        .def("visit", py::overload_cast<const FxForward<NumType>&>(&ConstVisitor<NumType>::visit, py::const_))
        .def("visit", py::overload_cast<const FixFloatSwap<NumType>&>(&ConstVisitor<NumType>::visit, py::const_));

    py::class_<NPVCalculator<NumType>, Visitor<NumType>>(m, "NPVCalculator")
        .def(py::init<const MarketData<NumType>&>(), py::arg("marketData"))
        .def("results", &NPVCalculator<NumType>::results)
        .def("clear", &NPVCalculator<NumType>::clear);

    py::class_<Indexer<NumType>, Visitor<NumType>>(m, "Indexer")
        .def(py::init<>())
        .def("request", &Indexer<NumType>::request)
        .def("clear", &Indexer<NumType>::clear);

    py::class_<ParSolver<NumType>, ConstVisitor<NumType>>(m, "ParSolver")
        .def(py::init<const MarketData<NumType>&>(), py::arg("marketData"))
        .def("results", &ParSolver<NumType>::results)
        .def("clear", &ParSolver<NumType>::clear);

    py::class_<ZSpreadCalculator<NumType>, ConstVisitor<NumType>>(m, "ZSpreadCalculator")
        .def(py::init<const MarketData<NumType>&, NumType, const DayCounter&, Compounding, Frequency>(), py::arg("marketData"), py::arg("targetNpv"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Compounded, py::arg("frequency") = Frequency::Semiannual)
        .def("results", &ZSpreadCalculator<NumType>::results)
        .def("clear", &ZSpreadCalculator<NumType>::clear);

    py::class_<CashflowProfiler<NumType>, ConstVisitor<NumType>>(m, "CashflowProfiler")
        .def(py::init<>())
        .def("redemptions", &CashflowProfiler<NumType>::redemptions)
        .def("interests", &CashflowProfiler<NumType>::interests)
        .def("clear", &CashflowProfiler<NumType>::clear);
}

void py_newvisitors(py::module& m) {
    py::class_<InstrumentVariant<NumType>>(m, "InstrumentVariant").def(py::init<>());

    py::class_<BaseVisitor<NumType>, Aux::PyBaseVisitor>(m, "BaseVisitor")
        .def("__call__", py::overload_cast<CustomFixedRateInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<EqualPaymentInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FixedRateBulletInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<ZeroCouponInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<CustomFloatingRateInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FloatingRateBulletInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FloatingRateEqualRedemptionInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FxForward<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FixFloatSwap<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("visit", [](BaseVisitor<NumType>& v, CustomFixedRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, EqualPaymentInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FixedRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, ZeroCouponInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, CustomFloatingRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FloatingRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FloatingRateEqualRedemptionInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FxForward<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FixFloatSwap<NumType>& i) { v(i); });

    py::class_<BaseConstVisitor<NumType>, Aux::PyBaseConstVisitor>(m, "BaseConstVisitor")
        .def("__call__", py::overload_cast<const CustomFixedRateInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const EqualPaymentInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FixedRateBulletInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const ZeroCouponInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const CustomFloatingRateInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FloatingRateBulletInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FloatingRateEqualRedemptionInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FxForward<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FixFloatSwap<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("visit", [](BaseConstVisitor<NumType>& v, const CustomFixedRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const EqualPaymentInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FixedRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const ZeroCouponInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const CustomFloatingRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FloatingRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FloatingRateEqualRedemptionInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FxForward<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FixFloatSwap<NumType>& i) { v(i); });

    py::class_<IndexingVisitor<NumType>, BaseVisitor<NumType>>(m, "IndexingVisitor")
        .def(py::init<>())
        .def("getResults", &IndexingVisitor<NumType>::getResults)
        .def("reset", &IndexingVisitor<NumType>::reset);

    py::class_<FixingVisitor<NumType>, BaseVisitor<NumType>>(m, "FixingVisitor").def(py::init<const MarketData<NumType>&>());

    py::class_<NPVConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "NPVConstVisitor")
        .def(py::init<const MarketData<NumType>&>(), py::arg("marketData"))
        .def("getResults", &NPVConstVisitor<NumType>::getResults)
        .def("reset", &NPVConstVisitor<NumType>::reset);

    py::class_<ParRateConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "ParRateConstVisitor")
        .def(py::init<const MarketData<NumType>&>(), py::arg("marketData"))
        .def("getResults", &ParRateConstVisitor<NumType>::getResults)
        .def("reset", &ParRateConstVisitor<NumType>::reset);

    py::class_<ZSpreadConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "ZSpreadConstVisitor")
        .def(py::init<const MarketData<NumType>&, NumType, const DayCounter&, Compounding, Frequency, double, double, size_t, bool>(),
             py::arg("marketData"), py::arg("targetNpv"), py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Compounded,
             py::arg("frequency") = Frequency::Semiannual, py::arg("guess") = 0.0, py::arg("accuracy") = 1e-6, py::arg("maxIterations") = 100,
             py::arg("showLogs") = false)
        .def("getResults", &ZSpreadConstVisitor<NumType>::getResults)
        .def("reset", &ZSpreadConstVisitor<NumType>::reset);

    py::class_<SensibilityConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "SensibilityConstVisitor")
        .def(py::init<const MarketData<NumType>&, double, bool>(), py::arg("marketData"), py::arg("delta") = 0.0001, py::arg("showLogs") = false)
        .def("getResults", &SensibilityConstVisitor<NumType>::getResults)
        .def("reset", &SensibilityConstVisitor<NumType>::reset);

    py::class_<CashflowAggregationConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "CashflowAggregationConstVisitor")
        .def("getResults", &CashflowAggregationConstVisitor<NumType>::getResults)
        .def("reset", &CashflowAggregationConstVisitor<NumType>::reset);

    py::class_<AccruedAmountConstVisitor<NumType>, CashflowAggregationConstVisitor<NumType>>(m, "AccruedAmountConstVisitor")
        .def(py::init<const Date&, const Date&>(), py::arg("startDate"), py::arg("endDate"));

    py::class_<MaturingAmountConstVisitor<NumType>, CashflowAggregationConstVisitor<NumType>>(m, "MaturingAmountConstVisitor")
        .def(py::init<const Date&>(), py::arg("refDate"));

    py::class_<InstrumentCashflowsConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "InstrumentCashflowsConstVisitor")
        .def(py::init<>())
        .def("getResults", &InstrumentCashflowsConstVisitor<NumType>::getResults)
        .def("reset", &InstrumentCashflowsConstVisitor<NumType>::reset);
}

#endif /* CD9BD627_F812_406D_AEEC_B699240E41D8 */
