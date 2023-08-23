#ifndef EC62B1BA_4902_4FC0_A3B8_1C419E9778E3
#define EC62B1BA_4902_4FC0_A3B8_1C419E9778E3

#include "config.hpp"
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponfixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/zerocouponfloatingrateinstrument.hpp>
#include <atlas/instruments/instrument.hpp>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;
using namespace Atlas;

void py_fixedrateinstruments(py::module& m) {
    py::class_<FixedRateInstrument<NumType>::Cashflows, DisbursementStreamMixin<NumType>, RedemptionStreamMixin<NumType>,
               FixedRateCouponStreamMixin<NumType>>(m, "FixedRateCashflows")
        .def("currency", [](FixedRateInstrument<NumType>::Cashflows& self, const Currency& c) { return self.currency(c); })
        .def("applyCurrency", [](FixedRateInstrument<NumType>::Cashflows& self, bool f) { return self.applyCurrency(f); });

    py::class_<FixedRateInstrument<NumType>, Instrument<NumType>>(m, "FixedRateInstrument")
        .def(py::init<const Date&, const Date&, const InterestRate<NumType>&, Side, double, const FixedRateInstrument<NumType>::Cashflows&>())
        .def("rate", py::overload_cast<>(&FixedRateInstrument<NumType>::rate, py::const_))
        .def("rate", py::overload_cast<NumType>(&FixedRateInstrument<NumType>::rate))
        .def("rate", py::overload_cast<const InterestRate<NumType>&>(&FixedRateInstrument<NumType>::rate))
        .def("cashflows", py::overload_cast<>(&FixedRateInstrument<NumType>::cashflows, py::const_), py::return_value_policy::reference)
        .def("cashflows", py::overload_cast<>(&FixedRateInstrument<NumType>::cashflows), py::return_value_policy::reference);

    py::class_<FixedRateBulletInstrument<NumType>, FixedRateInstrument<NumType>>(m, "FixedRateBulletInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("side") = Side::Recieve)
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, size_t, Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("discountContextIdx"),
             py::arg("side") = Side::Recieve);

    py::class_<FixedRateEqualRedemptionInstrument<NumType>, FixedRateInstrument<NumType>>(m, "FixedRateEqualRedemptionInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("side") = Side::Recieve)
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, size_t, Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("discountContextIdx"),
             py::arg("side") = Side::Recieve);

    py::class_<ZeroCouponFixedRateInstrument<NumType>, FixedRateInstrument<NumType>>(m, "ZeroCouponFixedRateInstrument")
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("rate"), py::arg("side") = Side::Recieve)
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, size_t, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("rate"), py::arg("discountContextIdx"), py::arg("side") = Side::Recieve);

    py::class_<EqualPaymentInstrument<NumType>, FixedRateInstrument<NumType>>(m, "EqualPaymentInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("side") = Side::Recieve)
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, size_t, Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("discountContextIdx"),
             py::arg("side") = Side::Recieve)
        .def("withRate", &EqualPaymentInstrument<NumType>::withRate);

    py::class_<CustomFixedRateInstrument<NumType>, FixedRateInstrument<NumType>>(m, "CustomFixedRateInstrument")
        .def(py::init<const std::map<Date, double>&, const std::map<Date, double>&, const InterestRate<NumType>&, Side, const std::set<Date>&>(), py::arg("disbursements"),
             py::arg("redemptions"), py::arg("rate"), py::arg("side") = Side::Recieve, py::arg("additionalCouponDates") = std::set<Date>())
        .def(py::init<const std::map<Date, double>&, const std::map<Date, double>&, const InterestRate<NumType>&, size_t, Side, const std::set<Date>&>(), py::arg("disbursements"),
             py::arg("redemptions"), py::arg("rate"), py::arg("discountContextIdx"), py::arg("side") = Side::Recieve,
             py::arg("additionalCouponDates") = std::set<Date>());
};
void py_floatingrateinstruments(py::module& m) {
    py::class_<FloatingRateInstrument<NumType>::Cashflows, DisbursementStreamMixin<NumType>, FloatingRateCouponStreamMixin<NumType>,
               RedemptionStreamMixin<NumType>>(m, "LoanFloatingRateCashflowStream")
        .def("currency", [](FloatingRateInstrument<NumType>::Cashflows& self, const Currency& c) { return self.currency(c); })
        .def("applyCurrency", [](FloatingRateInstrument<NumType>::Cashflows& self, bool f) { return self.applyCurrency(f); });

    py::class_<FloatingRateInstrument<NumType>, Instrument<NumType>>(m, "FloatingRateInstrument")
        .def(py::init<const Date&, const Date&, double, NumType, Side, const FloatingRateInstrument<NumType>::Cashflows&>())
        .def("spread", py::overload_cast<>(&FloatingRateInstrument<NumType>::spread, py::const_))
        .def("spread", py::overload_cast<NumType>(&FloatingRateInstrument<NumType>::spread))
        .def("cashflows", py::overload_cast<>(&FloatingRateInstrument<NumType>::cashflows, py::const_), py::return_value_policy::reference)
        .def("cashflows", py::overload_cast<>(&FloatingRateInstrument<NumType>::cashflows), py::return_value_policy::reference);

    py::class_<FloatingRateBulletInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "FloatingRateBulletInstrument")
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("spread"), py::arg("index"), py::arg("side") = Side::Recieve)
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, size_t, size_t, Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("notional"), py::arg("spread"), py::arg("index"), py::arg("discountContextIdx"), py::arg("indexContextIdx"),
             py::arg("side") = Side::Recieve);

    py::class_<FloatingRateEqualRedemptionInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "FloatingRateEqualRedemptionInstrument")
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("spread"), py::arg("index"), py::arg("side") = Side::Recieve)
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, size_t, size_t, Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("notional"), py::arg("spread"), py::arg("index"), py::arg("discountContextIdx"), py::arg("indexContextIdx"),
             py::arg("side") = Side::Recieve);

    py::class_<CustomFloatingRateInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "CustomFloatingRateInstrument")
        .def(py::init<const std::map<Date, double>&, const std::map<Date, double>&, NumType, const InterestRateIndex<NumType>&, Side,
                      const std::set<Date>>(),
             py::arg("disbursements"), py::arg("redempetions"), py::arg("spread"), py::arg("index"), py::arg("side") = Side::Recieve,
             py::arg("additionalCouponDates") = std::set<Date>())
        .def(py::init<const std::map<Date, double>&, const std::map<Date, double>&, NumType, const InterestRateIndex<NumType>&, size_t, size_t, Side,
                      const std::set<Date>>(),
             py::arg("disbursements"), py::arg("redempetions"), py::arg("spread"), py::arg("index"), py::arg("discountContextIdx"),
             py::arg("indexContextIdx"), py::arg("side") = Side::Recieve, py::arg("additionalCouponDates") = std::set<Date>());

    py::class_<ZeroCouponFloatingRateInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "ZeroCouponFloatingRateInstrument")
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, Side>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("spread"), py::arg("index"), py::arg("side") = Side::Recieve)
        .def(py::init<const Date&, const Date&, double, NumType, const InterestRateIndex<NumType>&, size_t, size_t, Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("notional"), py::arg("spread"), py::arg("index"), py::arg("discountContextIdx"), py::arg("indexContextIdx"),
             py::arg("side") = Side::Recieve);
};
void py_instruments(py::module& m) {
    // instruments
    py::class_<Instrument<NumType>>(m, "Instrument")
        .def("startDate", &Instrument<NumType>::startDate)
        .def("endDate", &Instrument<NumType>::endDate)
        .def("notional", &Instrument<NumType>::notional)
        .def("side", &Instrument<NumType>::side);

    py_fixedrateinstruments(m);
    py_floatingrateinstruments(m);
};
#endif /* EC62B1BA_4902_4FC0_A3B8_1C419E9778E3 */
