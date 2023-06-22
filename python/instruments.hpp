#ifndef EC62B1BA_4902_4FC0_A3B8_1C419E9778E3
#define EC62B1BA_4902_4FC0_A3B8_1C419E9778E3

// pybind11
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// instruments
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/instruments/instrument.hpp>

// fixed rate
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>

// floating rate
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>

// derivatives
#include <atlas/instruments/derivatives/fixfloatswap.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>

#define exportFixedRateInstrument(name)                                                                                                    \
    py::class_<name<dual>, FixedRateInstrument<dual>>(m, #name)                                                                            \
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<dual>&>(), py::arg("startDate"), py::arg("endDate"), \
             py::arg("paymentFrequency"), py::arg("notional"), py::arg("rate"))                                                            \
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<dual>&, const Context<YieldTermStructure<dual>>&>(), \
             py::arg("startDate"), py::arg("endDate"), py::arg("paymentFrequency"), py::arg("notional"), py::arg("rate"),                  \
             py::arg("discountCurveContext"))

#define exportFloatingRateInstrument(name)                                                                                                  \
    py::class_<name<dual>, FloatingRateInstrument<dual>>(m, #name)                                                                          \
        .def(py::init<const Date&, const Date&, double, dual, const Context<RateIndex<dual>>&, const Context<YieldTermStructure<dual>>&>(), \
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("spread"), py::arg("rateIndexContext"),             \
             py::arg("discountCurveContext"))                                                                                               \
        .def(py::init<const Date&, const Date&, double, dual, const Context<RateIndex<dual>>&>(), py::arg("startDate"), py::arg("endDate"), \
             py::arg("notional"), py::arg("spread"), py::arg("rateIndexContext"))

namespace py = pybind11;
using namespace Atlas;

namespace Aux {
    class PyInstrument : public Instrument<dual> {
        using Instrument<dual>::Instrument;

        void accept(Visitor<dual>& visitor) override {
            PYBIND11_OVERRIDE_PURE(void,             /* Return type */
                                   Instrument<dual>, /* Parent class */
                                   accept,           /* Name of function in C++ (must match Python name) */
                                   visitor           /* Argument(s) */
            );
        }

        void accept(ConstVisitor<dual>& visitor) const override {
            PYBIND11_OVERRIDE_PURE(void,             /* Return type */
                                   Instrument<dual>, /* Parent class */
                                   accept,           /* Name of function in C++ (must match Python name) */
                                   visitor           /* Argument(s) */
            );
        }
    };
}  // namespace Aux

void py_instruments(py::module& m) {
    // Instruments
    py::class_<Instrument<dual>, Aux::PyInstrument>(m, "Instrument")
        .def("startDate", &Instrument<dual>::startDate)
        .def("endDate", &Instrument<dual>::endDate)
        .def("notional", &Instrument<dual>::notional)
        .def("accept", py::overload_cast<Visitor<dual>&>(&Instrument<dual>::accept))
        .def("accept", py::overload_cast<ConstVisitor<dual>&>(&Instrument<dual>::accept, py::const_));

    py::class_<OneLegMixin<dual, FixedRateLeg<dual>>>(m, "OneFixRateLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<dual, FixedRateLeg<dual>>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<dual, FixedRateLeg<dual>>::leg), py::return_value_policy::reference)
        .def("discountCurveContext", &OneLegMixin<dual, FixedRateLeg<dual>>::discountCurveContext)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<dual, FixedRateLeg<dual>>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<dual>&>(&OneLegMixin<dual, FixedRateLeg<dual>>::disbursement))
        .def("currency", &OneLegMixin<dual, FixedRateLeg<dual>>::currency)
        .def("applyCcy", &OneLegMixin<dual, FixedRateLeg<dual>>::applyCcy);

    py::class_<OneLegMixin<dual, FloatingRateLeg<dual>>>(m, "OneFloatingRateLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<dual, FloatingRateLeg<dual>>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<dual, FloatingRateLeg<dual>>::leg), py::return_value_policy::reference)
        .def("discountCurveContext", &OneLegMixin<dual, FloatingRateLeg<dual>>::discountCurveContext)
        .def("rateIndexContext", &OneLegMixin<dual, FloatingRateLeg<dual>>::rateIndexContext)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<dual, FloatingRateLeg<dual>>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<dual>&>(&OneLegMixin<dual, FloatingRateLeg<dual>>::disbursement))
        .def("currency", &OneLegMixin<dual, FloatingRateLeg<dual>>::currency)
        .def("applyCcy", &OneLegMixin<dual, FloatingRateLeg<dual>>::applyCcy);

    py::class_<FloatingRateInstrument<dual>, Instrument<dual>, OneLegMixin<dual, FloatingRateLeg<dual>>>(m, "FloatingRateInstrument")
        .def(py::init<const Date&, const Date&, double, dual, const FloatingRateLeg<dual>&>())
        .def("spread", py::overload_cast<>(&FloatingRateInstrument<dual>::spread, py::const_))
        .def("spread", py::overload_cast<dual>(&FloatingRateInstrument<dual>::spread))
        .def("accept", py::overload_cast<Visitor<dual>&>(&FloatingRateInstrument<dual>::accept))
        .def("accept", py::overload_cast<ConstVisitor<dual>&>(&FloatingRateInstrument<dual>::accept, py::const_));

    py::class_<FixedRateInstrument<dual>, Instrument<dual>, OneLegMixin<dual, FixedRateLeg<dual>>>(m, "FixedRateInstrument")
        .def(py::init<const Date&, const Date&, const InterestRate<dual>&, double, const FixedRateLeg<dual>&>())
        .def("accept", py::overload_cast<Visitor<dual>&>(&FixedRateInstrument<dual>::accept))
        .def("accept", py::overload_cast<ConstVisitor<dual>&>(&FixedRateInstrument<dual>::accept, py::const_))
        .def("rate", py::overload_cast<>(&FixedRateInstrument<dual>::rate, py::const_))
        .def("rate", py::overload_cast<dual>(&FixedRateInstrument<dual>::rate))
        .def("rate", py::overload_cast<const InterestRate<dual>&>(&FixedRateInstrument<dual>::rate));

    // Fixed rate instruments
    exportFixedRateInstrument(FixedRateBulletInstrument);
    exportFixedRateInstrument(FixedRateEqualRedemptionInstrument);

    py::class_<ZeroCouponInstrument<dual>, FixedRateInstrument<dual>>(m, "ZeroCouponInstrument")
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&>(), py::arg("startDate"), py::arg("endDate"), py::arg("notional"),
             py::arg("rate"))
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&, const Context<YieldTermStructure<dual>>&>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("notional"), py::arg("rate"), py::arg("discountCurveContext"));

    py::enum_<EqualPaymentInstrument<dual>::BuildType>(m, "EqualPaymentInstrumentBuildType")
        .value("Fast", EqualPaymentInstrument<dual>::BuildType::Fast)
        .value("Precise", EqualPaymentInstrument<dual>::BuildType::Precise)
        .export_values();

    py::class_<EqualPaymentInstrument<dual>, FixedRateInstrument<dual>>(m, "EqualPaymentInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<dual>&, bool, EqualPaymentInstrument<dual>::BuildType>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"),
             py::arg("recalculateNotionals") = false, py::arg("buildType") = EqualPaymentInstrument<dual>::BuildType::Fast)
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<dual>&, const Context<YieldTermStructure<dual>>&, bool,
                      EqualPaymentInstrument<dual>::BuildType>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("discountCurveContext"),
             py::arg("recalculateNotionals") = false, py::arg("buildType") = EqualPaymentInstrument<dual>::BuildType::Fast);

    py::class_<CustomFixedRateInstrument<dual>, FixedRateInstrument<dual>>(m, "CustomFixedRateInstrument")
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, const InterestRate<dual>&>(), py::arg("dates"), py::arg("notionals"),
             py::arg("rate"))
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, const InterestRate<dual>&, const Context<YieldTermStructure<dual>>&>(),
             py::arg("dates"), py::arg("notionals"), py::arg("rate"), py::arg("discountCurveContext"));

    // Floating rate instruments
    exportFloatingRateInstrument(FloatingRateBulletInstrument);
    exportFloatingRateInstrument(FloatingRateEqualRedemptionInstrument);

    py::class_<CustomFloatingRateInstrument<dual>, FloatingRateInstrument<dual>>(m, "CustomFloatingRateInstrument")
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, dual, const Context<RateIndex<dual>>&>(), py::arg("dates"),
             py::arg("notionals"), py::arg("spread"), py::arg("rateIndexContext"))
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, dual, const Context<RateIndex<dual>>&,
                      const Context<YieldTermStructure<dual>>&>(),
             py::arg("dates"), py::arg("notionals"), py::arg("spread"), py::arg("rateIndexContext"), py::arg("discountCurveContext"));

    // Derivatives
    py::class_<OneLegMixin<dual, Leg<dual>>>(m, "OneLegMixin")
        .def("leg", py::overload_cast<>(&OneLegMixin<dual, Leg<dual>>::leg, py::const_), py::return_value_policy::reference)
        .def("leg", py::overload_cast<>(&OneLegMixin<dual, Leg<dual>>::leg), py::return_value_policy::reference)
        .def("discountCurveContext", &OneLegMixin<dual, Leg<dual>>::discountCurveContext)
        .def("disbursement", py::overload_cast<>(&OneLegMixin<dual, Leg<dual>>::disbursement), py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<const Cashflow<dual>&>(&OneLegMixin<dual, Leg<dual>>::disbursement))
        .def("currency", &OneLegMixin<dual, Leg<dual>>::currency)
        .def("applyCcy", &OneLegMixin<dual, Leg<dual>>::applyCcy);

    py::class_<FxForward<dual>, Instrument<dual>, OneLegMixin<dual, Leg<dual>>>(m, "FxForward")
        .def(py::init<const Date&, const Date&, dual, const Currency&, const Currency&, double, FxForward<dual>::Side>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("fwdPrice"), py::arg("currency1"), py::arg("currency2"), py::arg("notional"), py::arg("side"));

    py::enum_<FxForward<dual>::Side>(m, "FxForwardSide")
        .value("Buy", FxForward<dual>::Side::BUY)
        .value("Sell", FxForward<dual>::Side::SELL)
        .export_values();

    py::class_<TwoLegMixin<FixedRateLeg<dual>, FloatingRateLeg<dual>>>(m, "FixFloatLegMixin")
        .def("firstLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<dual>, FloatingRateLeg<dual>>::firstLeg, py::const_),
             py::return_value_policy::reference)
        .def("firstLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<dual>, FloatingRateLeg<dual>>::firstLeg), py::return_value_policy::reference)
        .def("secondLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<dual>, FloatingRateLeg<dual>>::secondLeg, py::const_),
             py::return_value_policy::reference)
        .def("secondLeg", py::overload_cast<>(&TwoLegMixin<FixedRateLeg<dual>, FloatingRateLeg<dual>>::secondLeg),
             py::return_value_policy::reference);

    py::class_<FixFloatSwap<dual>, Instrument<dual>, TwoLegMixin<FixedRateLeg<dual>, FloatingRateLeg<dual>>>(m, "FixFloatSwap")
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&, dual, Frequency, const Context<RateIndex<dual>>&,
                      FixFloatSwap<dual>::Side>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("fixedRate"), py::arg("spread"), py::arg("fixLegFreq"),
             py::arg("rateIndexContext"), py::arg("side"))
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&, dual, Frequency, const Context<RateIndex<dual>>&,
                      FixFloatSwap<dual>::Side, const Context<YieldTermStructure<dual>>&>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("fixedRate"), py::arg("spread"), py::arg("fixLegFreq"),
             py::arg("rateIndexContext"), py::arg("side"), py::arg("discountCurveContext"));

    py::enum_<FixFloatSwap<dual>::Side>(m, "FixFloatSwapSide")
        .value("Payer", FixFloatSwap<dual>::Side::PAY)
        .value("Receiver", FixFloatSwap<dual>::Side::RECIEVE)
        .export_values();
};

#endif /* EC62B1BA_4902_4FC0_A3B8_1C419E9778E3 */
