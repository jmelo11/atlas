#ifndef CD9BD627_F812_406D_AEEC_B699240E41D8
#define CD9BD627_F812_406D_AEEC_B699240E41D8

#include "config.hpp"
#include <atlas/visitors/basevisitor.hpp>
#include <atlas/visitors/cashflowaggregation/accruedamountconstvisitor.hpp>
#include <atlas/visitors/cashflowaggregation/instrumentcashflowsconstvisitor.hpp>
#include <atlas/visitors/cashflowaggregation/maturingamountconstvisitor.hpp>
#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>
#include <atlas/visitors/parrateconstvisitor.hpp>
#include <atlas/visitors/sensitivityconstvisitor.hpp>
#include <atlas/visitors/zspreadconstvisitor.hpp>
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
    };
};  // namespace Aux

void py_visitors(py::module& m) {
    py::class_<InstrumentVariant<NumType>>(m, "InstrumentVariant").def(py::init<>());

    py::class_<BaseVisitor<NumType>, Aux::PyBaseVisitor>(m, "BaseVisitor")
        .def("__call__", py::overload_cast<CustomFixedRateInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<EqualPaymentInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FixedRateBulletInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<ZeroCouponInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<CustomFloatingRateInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FloatingRateBulletInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))
        .def("__call__", py::overload_cast<FloatingRateEqualRedemptionInstrument<NumType>&>(&BaseVisitor<NumType>::operator()))

        .def("visit", [](BaseVisitor<NumType>& v, CustomFixedRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, EqualPaymentInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FixedRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, ZeroCouponInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, CustomFloatingRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FloatingRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseVisitor<NumType>& v, FloatingRateEqualRedemptionInstrument<NumType>& i) { v(i); });

    py::class_<BaseConstVisitor<NumType>, Aux::PyBaseConstVisitor>(m, "BaseConstVisitor")
        .def("__call__", py::overload_cast<const CustomFixedRateInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const EqualPaymentInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FixedRateBulletInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const ZeroCouponInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const CustomFloatingRateInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FloatingRateBulletInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("__call__", py::overload_cast<const FloatingRateEqualRedemptionInstrument<NumType>&>(&BaseConstVisitor<NumType>::operator(), py::const_))
        .def("visit", [](BaseConstVisitor<NumType>& v, const CustomFixedRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const EqualPaymentInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FixedRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const ZeroCouponInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const CustomFloatingRateInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FloatingRateBulletInstrument<NumType>& i) { v(i); })
        .def("visit", [](BaseConstVisitor<NumType>& v, const FloatingRateEqualRedemptionInstrument<NumType>& i) { v(i); });

    py::class_<IndexingVisitor<NumType>, BaseVisitor<NumType>>(m, "IndexingVisitor")
        .def(py::init<>())
        .def("getResults", &IndexingVisitor<NumType>::getResults)
        .def("reset", &IndexingVisitor<NumType>::reset);

    py::class_<FixingVisitor<NumType>, BaseVisitor<NumType>>(m, "FixingVisitor").def(py::init<const MarketData<NumType>&>());

    py::class_<NPVConstVisitor<NumType>::Results>(m, "NPVResults")
        .def_readonly("npv", &NPVConstVisitor<NumType>::Results::npv)
        .def_readonly("redemptionsNPV", &NPVConstVisitor<NumType>::Results::redemptionsNPV)
        .def_readonly("disbursementsNPV", &NPVConstVisitor<NumType>::Results::disbursementsNPV)
        .def_readonly("fixedRateCouponsNPV", &NPVConstVisitor<NumType>::Results::fixedRateCouponsNPV)
        .def_readonly("floatingRateCouponsNPV", &NPVConstVisitor<NumType>::Results::floatingRateCouponsNPV);

    py::class_<NPVConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "NPVConstVisitor")
        .def(py::init<const MarketData<NumType>&>(), py::arg("marketData"))
        .def("getResults", &NPVConstVisitor<NumType>::getResults)
        .def("reset", &NPVConstVisitor<NumType>::reset);

    py::class_<ParRateConstVisitor<NumType>::Results>(m, "ParRateResults")
        .def_readonly("parRate", &ParRateConstVisitor<NumType>::Results::parRate)
        .def_readonly("parSpread", &ParRateConstVisitor<NumType>::Results::parSpread);

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

    py::class_<SensitivityConstVisitor<NumType>::Results>(m, "SensitivityResults")
        .def_readonly("couponSens", &SensitivityConstVisitor<NumType>::Results::couponSens)
        .def_readonly("spreadSens", &SensitivityConstVisitor<NumType>::Results::spreadSens);

    py::class_<SensitivityConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "SensitivityConstVisitor")
        .def(py::init<const MarketData<NumType>&, double, bool>(), py::arg("marketData"), py::arg("delta") = 0.0001, py::arg("showLogs") = false)
        .def("getResults", &SensitivityConstVisitor<NumType>::getResults)
        .def("reset", &SensitivityConstVisitor<NumType>::reset);

    py::class_<CashflowAggregationConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "CashflowAggregationConstVisitor")
        .def("getResults", &CashflowAggregationConstVisitor<NumType>::getResults)
        .def("reset", &CashflowAggregationConstVisitor<NumType>::reset);

    py::class_<AccruedAmountConstVisitor<NumType>, CashflowAggregationConstVisitor<NumType>>(m, "AccruedAmountConstVisitor")
        .def(py::init<const Date&, const Date&>(), py::arg("startDate"), py::arg("endDate"));

    py::class_<MaturingAmountConstVisitor<NumType>, CashflowAggregationConstVisitor<NumType>>(m, "MaturingAmountConstVisitor")
        .def(py::init<const Date&>(), py::arg("refDate"));

    py::class_<InstrumentCashflowsConstVisitor<NumType>::Results>(m, "InstrumentCashflowsResults")
        .def_readonly("redemptions", &InstrumentCashflowsConstVisitor<NumType>::Results::redemptions)
        .def_readonly("disbursements", &InstrumentCashflowsConstVisitor<NumType>::Results::disbursements)
        .def_readonly("fixedRateCoupons", &InstrumentCashflowsConstVisitor<NumType>::Results::fixedRateCoupons)
        .def_readonly("floatingRateCoupons", &InstrumentCashflowsConstVisitor<NumType>::Results::floatingRateCoupons);

    py::class_<InstrumentCashflowsConstVisitor<NumType>, BaseConstVisitor<NumType>>(m, "InstrumentCashflowsConstVisitor")
        .def(py::init<>())
        .def("getResults", &InstrumentCashflowsConstVisitor<NumType>::getResults)
        .def("reset", &InstrumentCashflowsConstVisitor<NumType>::reset);
}

#endif /* CD9BD627_F812_406D_AEEC_B699240E41D8 */
