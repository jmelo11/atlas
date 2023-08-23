#ifndef EEEB563F_626F_4BF4_B962_66C282CA0562
#define EEEB563F_626F_4BF4_B962_66C282CA0562

#include "config.hpp"
#include <atlas/others/interpolations/loglinearinterpolation.hpp>
#include <atlas/rates/index/interestrateindex.hpp>
#include <atlas/rates/yieldtermstructure/discountcurve.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/rates/yieldtermstructure/zeroratecurve.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

void py_rates(py::module& m) {
    py::class_<InterestRate<NumType>>(m, "InterestRate")
        .def(py::init<>())
        .def(py::init<NumType, const DayCounter&, Compounding, Frequency>(), py::arg("rate"), py::arg("dc") = Actual360(),
             py::arg("comp") = Compounding::Simple, py::arg("freq") = Frequency::Annual)
        .def("rate", &InterestRate<NumType>::rate)
        .def("compoundFactor", py::overload_cast<double>(&InterestRate<NumType>::compoundFactor, py::const_))
        .def("compoundFactor",
             py::overload_cast<const Date&, const Date&, const Date&, const Date&>(&InterestRate<NumType>::compoundFactor, py::const_))
        .def("discountFactor",
             py::overload_cast<const Date&, const Date&, const Date&, const Date&>(&InterestRate<NumType>::discountFactor, py::const_))
        .def("discountFactor", py::overload_cast<double>(&InterestRate<NumType>::discountFactor, py::const_))
        .def("__str__", [](const InterestRate<NumType>& r) {
            std::string msg = "InterestRate(" + std::to_string(val(r.rate())) + ", " + r.dayCounter().name() + ", " +
                              std::to_string(r.compounding()) + ", " + std::to_string(r.frequency()) + ")";
            return msg;
        });

    // yield term structure
    py::class_<YieldTermStructure<NumType>>(m, "YieldTermStructure")
        .def(py::init())
        .def("discount", py::overload_cast<const Date&>(&YieldTermStructure<NumType>::discount, py::const_))
        .def("discount", py::overload_cast<double>(&YieldTermStructure<NumType>::discount, py::const_))
        .def("forwardRate", &YieldTermStructure<NumType>::forwardRate)
        .def("refDate", &YieldTermStructure<NumType>::refDate)
        .def("enableExtrapolation", &YieldTermStructure<NumType>::enableExtrapolation);

    py::class_<DiscountTermStructure<LogLinearInterpolator, NumType>, YieldTermStructure<NumType>>(m, "DiscountLogLinearTermStructure")
        .def(py::init<const std::vector<Date>&, const std::vector<NumType>&, const DayCounter&>(), py::arg("dates"), py::arg("discounts"),
             py::arg("dayCounter") = Actual360());

    py::class_<FlatForwardTermStructure<NumType>, YieldTermStructure<NumType>>(m, "FlatForwardTermStructure")
        .def(py::init<const Date&, NumType, const DayCounter&, Compounding, Frequency>(), py::arg("refDate"), py::arg("rate"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple, py::arg("frequency") = Frequency::Annual)
        .def(py::init<const Date&, const InterestRate<NumType>&>(), py::arg("refDate"), py::arg("rate"));

    py::class_<ZeroRateTermStructure<LinearInterpolator, NumType>, YieldTermStructure<NumType>>(m, "ZeroRateLinearTermStructure")
        .def(py::init<const std::vector<Date>&, const std::vector<NumType>&, const DayCounter&, Compounding, Frequency>(), py::arg("dates"),
             py::arg("zeroRates"), py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple,
             py::arg("frequency") = Frequency::Annual);

    // Index
    py::class_<Index<NumType>>(m, "Index")
        .def(py::init<>())
        .def(py::init<Frequency>(), py::arg("fixingFrequency"))
        .def(py::init<const Period&>(), py::arg("tenor"))
        .def("addHistoricalFixing", &Index<NumType>::addHistoricalFixing)
        .def("addHistoricalFixings", &Index<NumType>::addHistoricalFixings)
        .def("tenor", &Index<NumType>::tenor)
        .def("empty", &Index<NumType>::empty)
        .def("fixing", &Index<NumType>::fixing);

    py::class_<RateDefinition>(m, "RateDefinition")
        .def(py::init<const DayCounter&, Compounding, Frequency>(), py::arg("dayCounter"), py::arg("compounding"), py::arg("frequency"))
        .def("dayCounter", &RateDefinition::dayCounter)
        .def("frequency", &RateDefinition::frequency)
        .def("compounding", &RateDefinition::compounding);

    py::class_<InterestRateIndex<NumType>, Index<NumType>>(m, "InterestRateIndex")
        .def(py::init<Frequency, const RateDefinition&>(), py::arg("fixingFrequency") = Frequency::NoFrequency,
             py::arg("rateDefinition") = RateDefinition())
        .def(py::init<const Period&, const RateDefinition&>(), py::arg("tenor") = Period(), py::arg("rateDefinition") = RateDefinition())
        .def("rateDefinition", &InterestRateIndex<NumType>::rateDefinition)
        .def("isValid", &InterestRateIndex<NumType>::isValid);
};

#endif /* EEEB563F_626F_4BF4_B962_66C282CA0562 */
