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

#define exportFixedRateInstrument(name)                                                                                                          \
    py::class_<name<NumType>, FixedRateInstrument<NumType>>(m, #name)                                                                            \
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&>(), py::arg("startDate"), py::arg("endDate"),    \
             py::arg("paymentFrequency"), py::arg("notional"), py::arg("rate"))                                                                  \
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, const Context<YieldTermStructure<NumType>>&>(), \
             py::arg("startDate"), py::arg("endDate"), py::arg("paymentFrequency"), py::arg("notional"), py::arg("rate"),                        \
             py::arg("discountCurveContext"))

#define exportFloatingRateInstrument(name)                                                                                                           \
    py::class_<name<NumType>, FloatingRateInstrument<NumType>>(m, #name)                                                                             \
        .def(py::init<const Date&, const Date&, double, NumType, const Context<RateIndex<NumType>>&, const Context<YieldTermStructure<NumType>>&>(), \
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("spread"), py::arg("rateIndexContext"),                          \
             py::arg("discountCurveContext"))                                                                                                        \
        .def(py::init<const Date&, const Date&, double, NumType, const Context<RateIndex<NumType>>&>(), py::arg("startDate"), py::arg("endDate"),    \
             py::arg("notional"), py::arg("spread"), py::arg("rateIndexContext"))

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyInstrument : public Instrument<NumType> {
        using Instrument<NumType>::Instrument;

        void accept(Visitor<NumType>& visitor) override {
            PYBIND11_OVERRIDE_PURE(void,                /* Return type */
                                   Instrument<NumType>, /* Parent class */
                                   accept,              /* Name of function in C++ (must match Python name) */
                                   visitor              /* Argument(s) */
            );
        }

        void accept(ConstVisitor<NumType>& visitor) const override {
            PYBIND11_OVERRIDE_PURE(void,                /* Return type */
                                   Instrument<NumType>, /* Parent class */
                                   accept,              /* Name of function in C++ (must match Python name) */
                                   visitor              /* Argument(s) */
            );
        }
    };
}  // namespace Aux

void py_instruments(py::module& m) {
    // Instruments
    py::class_<Instrument<NumType>, Aux::PyInstrument>(m, "Instrument")
        .def("startDate", &Instrument<NumType>::startDate)
        .def("endDate", &Instrument<NumType>::endDate)
        .def("notional", &Instrument<NumType>::notional)
        .def("accept", py::overload_cast<Visitor<NumType>&>(&Instrument<NumType>::accept))
        .def("accept", py::overload_cast<ConstVisitor<NumType>&>(&Instrument<NumType>::accept, py::const_));

    py::class_<OneLegMixin<NumType, FixedRateLeg<NumType>>>(m, "OneFixRateLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<NumType, FixedRateLeg<NumType>>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<NumType, FixedRateLeg<NumType>>::leg), py::return_value_policy::reference)
        .def("discountCurveContext", &OneLegMixin<NumType, FixedRateLeg<NumType>>::discountCurveContext)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<NumType, FixedRateLeg<NumType>>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<NumType>&>(&OneLegMixin<NumType, FixedRateLeg<NumType>>::disbursement))
        .def("currency", &OneLegMixin<NumType, FixedRateLeg<NumType>>::currency)
        .def("applyCcy", &OneLegMixin<NumType, FixedRateLeg<NumType>>::applyCcy);

    py::class_<OneLegMixin<NumType, FloatingRateLeg<NumType>>>(m, "OneFloatingRateLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<NumType, FloatingRateLeg<NumType>>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<NumType, FloatingRateLeg<NumType>>::leg), py::return_value_policy::reference)
        .def("discountCurveContext", &OneLegMixin<NumType, FloatingRateLeg<NumType>>::discountCurveContext)
        .def("rateIndexContext", &OneLegMixin<NumType, FloatingRateLeg<NumType>>::rateIndexContext)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<NumType, FloatingRateLeg<NumType>>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<NumType>&>(&OneLegMixin<NumType, FloatingRateLeg<NumType>>::disbursement))
        .def("currency", &OneLegMixin<NumType, FloatingRateLeg<NumType>>::currency)
        .def("applyCcy", &OneLegMixin<NumType, FloatingRateLeg<NumType>>::applyCcy);

    py::class_<FloatingRateInstrument<NumType>, Instrument<NumType>, OneLegMixin<NumType, FloatingRateLeg<NumType>>>(m, "FloatingRateInstrument")
        .def(py::init<const Date&, const Date&, double, NumType, const FloatingRateLeg<NumType>&>())
        .def("spread", py::overload_cast<>(&FloatingRateInstrument<NumType>::spread, py::const_))
        .def("spread", py::overload_cast<NumType>(&FloatingRateInstrument<NumType>::spread))
        .def("accept", py::overload_cast<Visitor<NumType>&>(&FloatingRateInstrument<NumType>::accept))
        .def("accept", py::overload_cast<ConstVisitor<NumType>&>(&FloatingRateInstrument<NumType>::accept, py::const_));

    py::class_<FixedRateInstrument<NumType>, Instrument<NumType>, OneLegMixin<NumType, FixedRateLeg<NumType>>>(m, "FixedRateInstrument")
        .def(py::init<const Date&, const Date&, const InterestRate<NumType>&, double, const FixedRateLeg<NumType>&>())
        .def("accept", py::overload_cast<Visitor<NumType>&>(&FixedRateInstrument<NumType>::accept))
        .def("accept", py::overload_cast<ConstVisitor<NumType>&>(&FixedRateInstrument<NumType>::accept, py::const_))
        .def("rate", py::overload_cast<>(&FixedRateInstrument<NumType>::rate, py::const_))
        .def("rate", py::overload_cast<NumType>(&FixedRateInstrument<NumType>::rate))
        .def("rate", py::overload_cast<const InterestRate<NumType>&>(&FixedRateInstrument<NumType>::rate));

    // Fixed rate instruments
    exportFixedRateInstrument(FixedRateBulletInstrument);
    exportFixedRateInstrument(FixedRateEqualRedemptionInstrument);

    py::class_<ZeroCouponInstrument<NumType>, FixedRateInstrument<NumType>>(m, "ZeroCouponInstrument")
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("notional"), py::arg("rate"))
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, const Context<YieldTermStructure<NumType>>&>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("rate"), py::arg("discountCurveContext"));

    py::enum_<EqualPaymentInstrument<NumType>::BuildType>(m, "EqualPaymentInstrumentBuildType")
        .value("Fast", EqualPaymentInstrument<NumType>::BuildType::Fast)
        .value("Precise", EqualPaymentInstrument<NumType>::BuildType::Precise)
        .export_values();

    py::class_<EqualPaymentInstrument<NumType>, FixedRateInstrument<NumType>>(m, "EqualPaymentInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, bool, EqualPaymentInstrument<NumType>::BuildType>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"),
             py::arg("recalculateNotionals") = false, py::arg("buildType") = EqualPaymentInstrument<NumType>::BuildType::Fast)
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<NumType>&, const Context<YieldTermStructure<NumType>>&, bool,
                      EqualPaymentInstrument<NumType>::BuildType>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("discountCurveContext"),
             py::arg("recalculateNotionals") = false, py::arg("buildType") = EqualPaymentInstrument<NumType>::BuildType::Fast);

    py::class_<CustomFixedRateInstrument<NumType>, FixedRateInstrument<NumType>>(m, "CustomFixedRateInstrument")
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, const InterestRate<NumType>&>(), py::arg("dates"), py::arg("notionals"),
             py::arg("rate"))
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, const InterestRate<NumType>&,
                      const Context<YieldTermStructure<NumType>>&>(),
             py::arg("dates"), py::arg("notionals"), py::arg("rate"), py::arg("discountCurveContext"));

    // Floating rate instruments
    exportFloatingRateInstrument(FloatingRateBulletInstrument);
    exportFloatingRateInstrument(FloatingRateEqualRedemptionInstrument);

    py::class_<CustomFloatingRateInstrument<NumType>, FloatingRateInstrument<NumType>>(m, "CustomFloatingRateInstrument")
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, NumType, const Context<RateIndex<NumType>>&>(), py::arg("dates"),
             py::arg("notionals"), py::arg("spread"), py::arg("rateIndexContext"))
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, NumType, const Context<RateIndex<NumType>>&,
                      const Context<YieldTermStructure<NumType>>&>(),
             py::arg("dates"), py::arg("notionals"), py::arg("spread"), py::arg("rateIndexContext"), py::arg("discountCurveContext"));

    // Derivatives
    py::class_<OneLegMixin<NumType, Leg<NumType>>>(m, "OneLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<NumType, Leg<NumType>>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<NumType, Leg<NumType>>::leg), py::return_value_policy::reference)
        .def("discountCurveContext", &OneLegMixin<NumType, Leg<NumType>>::discountCurveContext)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<NumType, Leg<NumType>>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<NumType>&>(&OneLegMixin<NumType, Leg<NumType>>::disbursement))
        .def("currency", &OneLegMixin<NumType, Leg<NumType>>::currency)
        .def("applyCcy", &OneLegMixin<NumType, Leg<NumType>>::applyCcy);

    py::class_<FxForward<NumType>, Instrument<NumType>, OneLegMixin<NumType, Leg<NumType>>>(m, "FxForward")
        .def(py::init<const Date&, const Date&, NumType, const Currency&, const Currency&, double, FxForward<NumType>::Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("fwdPrice"), py::arg("currency1"), py::arg("currency2"), py::arg("notional"), py::arg("side"));

    py::enum_<FxForward<NumType>::Side>(m, "FxForwardSide")
        .value("Buy", FxForward<NumType>::Side::BUY)
        .value("Sell", FxForward<NumType>::Side::SELL)
        .export_values();

    py::class_<TwoLegMixin<FixedRateLeg<NumType>, FloatingRateLeg<NumType>>>(m, "FixFloatLegMixin")
        .def("firstLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<NumType>, FloatingRateLeg<NumType>>::firstLeg, py::const_),
             py::return_value_policy::reference)
        .def("firstLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<NumType>, FloatingRateLeg<NumType>>::firstLeg),
             py::return_value_policy::reference)
        .def("secondLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<NumType>, FloatingRateLeg<NumType>>::secondLeg, py::const_),
             py::return_value_policy::reference)
        .def("secondLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<NumType>, FloatingRateLeg<NumType>>::secondLeg),
             py::return_value_policy::reference);

    py::class_<FixFloatSwap<NumType>, Instrument<NumType>, TwoLegMixin<FixedRateLeg<NumType>, FloatingRateLeg<NumType>>>(m, "FixFloatSwap")
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, NumType, Frequency, const Context<RateIndex<NumType>>&,
                      FixFloatSwap<NumType>::Side>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("fixedRate"), py::arg("spread"), py::arg("fixLegFreq"),
             py::arg("rateIndexContext"), py::arg("side"))
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, NumType, Frequency, const Context<RateIndex<NumType>>&,
                      FixFloatSwap<NumType>::Side, const Context<YieldTermStructure<NumType>>&>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("fixedRate"), py::arg("spread"), py::arg("fixLegFreq"),
             py::arg("rateIndexContext"), py::arg("side"), py::arg("discountCurveContext"));

    py::enum_<FixFloatSwap<NumType>::Side>(m, "FixFloatSwapSide")
        .value("Payer", FixFloatSwap<NumType>::Side::PAY)
        .value("Receiver", FixFloatSwap<NumType>::Side::RECIEVE)
        .export_values();
};

#endif /* EC62B1BA_4902_4FC0_A3B8_1C419E9778E3 */
