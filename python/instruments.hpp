#ifndef EC62B1BA_4902_4FC0_A3B8_1C419E9778E3
#define EC62B1BA_4902_4FC0_A3B8_1C419E9778E3

#include "config.hpp"
#include <atlas/instruments/derivatives/fixfloatswap.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/instruments/instrument.hpp>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace Atlas;

void py_instruments(py::module& m) {
    // mixins
    py::class_<OneLegMixin<FixedRateLeg, NumType>>(m, "OneFixRateLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<FixedRateLeg, NumType>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<FixedRateLeg, NumType>::leg), py::return_value_policy::reference)
        .def("discountContextIdx", &OneLegMixin<FixedRateLeg, NumType>::discountContextIdx)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<FixedRateLeg, NumType>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<NumType>&>(&OneLegMixin<FixedRateLeg, NumType>::disbursement))
        .def("currency", &OneLegMixin<FixedRateLeg, NumType>::currency)
        .def("applyCcy", &OneLegMixin<FixedRateLeg, NumType>::applyCcy);

    py::class_<OneLegMixin<FloatingRateLeg, NumType>>(m, "OneFloatingRateLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<FloatingRateLeg, NumType>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<FloatingRateLeg, NumType>::leg), py::return_value_policy::reference)
        .def("discountContextIdx", &OneLegMixin<FloatingRateLeg, NumType>::discountContextIdx)
        .def("indexContextIdx", &OneLegMixin<FloatingRateLeg, NumType>::indexContextIdx)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<FloatingRateLeg, NumType>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<NumType>&>(&OneLegMixin<FloatingRateLeg, NumType>::disbursement))
        .def("currency", &OneLegMixin<FloatingRateLeg, NumType>::currency)
        .def("applyCcy", &OneLegMixin<FloatingRateLeg, NumType>::applyCcy);

    py::class_<TwoLegMixin<FixedRateLeg, FloatingRateLeg, NumType>>(m, "FixFloatLegMixin")
        .def("firstLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg, FloatingRateLeg, NumType>::firstLeg, py::const_),
             py::return_value_policy::reference)
        .def("firstLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg, FloatingRateLeg, NumType>::firstLeg), py::return_value_policy::reference)
        .def("secondLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg, FloatingRateLeg, NumType>::secondLeg, py::const_),
             py::return_value_policy::reference)
        .def("secondLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg, FloatingRateLeg, NumType>::secondLeg), py::return_value_policy::reference);

    py::class_<OneLegMixin<Leg, NumType>>(m, "OneLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<Leg, NumType>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<Leg, NumType>::leg), py::return_value_policy::reference)
        .def("discountContextIdx", &OneLegMixin<Leg, NumType>::discountContextIdx)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<Leg, NumType>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<NumType>&>(&OneLegMixin<Leg, NumType>::disbursement))
        .def("currency", &OneLegMixin<Leg, NumType>::currency)
        .def("applyCcy", &OneLegMixin<Leg, NumType>::applyCcy);

    // instruments
    py::class_<Instrument<NumType>>(m, "Instrument")
        .def("startDate", &Instrument<NumType>::startDate)
        .def("endDate", &Instrument<NumType>::endDate)
        .def("notional", &Instrument<NumType>::notional)
        .def("side", &Instrument<NumType>::side);

    // fixed rate
    py::class_<FixedRateInstrument<NumType>, Instrument<NumType>, OneLegMixin<FixedRateLeg, NumType>>(m, "FixedRateInstrument")
        .def(py::init<const Date&, const Date&, const InterestRate<NumType>&, Side, double, const FixedRateLeg<NumType>&>())
        .def("rate", py::overload_cast<>(&FixedRateInstrument<NumType>::rate, py::const_))
        .def("rate", py::overload_cast<NumType>(&FixedRateInstrument<NumType>::rate))
        .def("rate", py::overload_cast<const InterestRate<NumType>&>(&FixedRateInstrument<NumType>::rate));

    py::class_<FixedRateBulletInstrument<NumType>, FixedRateInstrument<NumType>>(m, "FixedRateBulletInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, Side>())
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, size_t, Side>());

    py::class_<FixedRateEqualRedemptionInstrument<NumType>, FixedRateInstrument<NumType>>(m, "FixedRateEqualRedemptionInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, Side>())
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, size_t, Side>());

    py::class_<ZeroCouponInstrument<NumType>, FixedRateInstrument<NumType>>(m, "ZeroCouponInstrument")
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("rate"), py::arg("side") = Side::Long)
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, size_t, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("rate"), py::arg("discountContextIdx"), py::arg("side") = Side::Long);

    py::class_<EqualPaymentInstrument<NumType>, FixedRateInstrument<NumType>>(m, "EqualPaymentInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, Side, bool>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("side") = Side::Long,
             py::arg("recalculateNotionals") = false)
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, size_t, Side, bool>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("discountContextIdx"),
             py::arg("side") = Side::Long, py::arg("recalculateNotionals") = false);

    py::class_<CustomFixedRateInstrument<NumType>, FixedRateInstrument<NumType>>(m, "CustomFixedRateInstrument")
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, const InterestRate<NumType>&>(), py::arg("dates"), py::arg("notionals"),
             py::arg("rate"))
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, const InterestRate<NumType>&, size_t>(), py::arg("dates"),
             py::arg("notionals"), py::arg("rate"), py::arg("discountContextIdx"));

    // floating rate
    py::class_<FloatingRateInstrument<NumType>, Instrument<NumType>, OneLegMixin<FloatingRateLeg, NumType>>(m, "FloatingRateInstrument")
        .def(py::init<const Date&, const Date&, Side, double, NumType, const FloatingRateLeg<NumType>&>())
        .def("spread", py::overload_cast<>(&FloatingRateInstrument<NumType>::spread, py::const_))
        .def("spread", py::overload_cast<NumType>(&FloatingRateInstrument<NumType>::spread));

    py::class_<FloatingRateBulletInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "FloatingRateBulletInstrument")
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, Side>())
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, size_t, size_t, Side>());

    py::class_<FloatingRateEqualRedemptionInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "FloatingRateEqualRedemptionInstrument")
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, Side>())
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, size_t, size_t, Side>());

    py::class_<CustomFloatingRateInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "CustomFloatingRateInstrument")
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, NumType, const InterestRateIndex<NumType>&>())
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, NumType, const InterestRateIndex<NumType>&, size_t, size_t>());

    // derivatives
    py::class_<FxForward<NumType>, Instrument<NumType>, OneLegMixin<Leg, NumType>>(m, "FxForward")
        .def(py::init<const Date&, const Date&, NumType, const Currency&, const Currency&, double, Side>())
        .def(py::init<const Date&, const Date&, NumType, const Currency&, const Currency&, double, Side, size_t>());

    py::class_<FixFloatSwap<NumType>, Instrument<NumType>, TwoLegMixin<FixedRateLeg, FloatingRateLeg, NumType>>(m, "FixFloatSwap")
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, NumType, Frequency, const InterestRateIndex<NumType>&, Side>())
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, NumType, Frequency, const InterestRateIndex<NumType>&, Side,
                      size_t, size_t>());
};

#endif /* EC62B1BA_4902_4FC0_A3B8_1C419E9778E3 */
