#ifndef EEEB563F_626F_4BF4_B962_66C282CA0562
#define EEEB563F_626F_4BF4_B962_66C282CA0562

#include "config.hpp"
#include <atlas/fundation/context.hpp>
#include <atlas/others/interpolations/loglinearinterpolation.hpp>
#include <atlas/rates/yieldtermstructure.hpp>
#include <atlas/rates/yieldtermstructure/discountcurve.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/rates/yieldtermstructure/zeroratecurve.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

void py_rates(py::module& m) {
    py::class_<InterestRate<NumType>>(m, "InterestRate")
        .def(py::init<>())
        .def(py::init<NumType, DayCounter, Compounding, Frequency>(), py::arg("rate"), py::arg("dc") = Actual360(),
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

    // Context
    py::class_<Context<YieldTermStructure<NumType>>>(m, "CurveContext")
        .def("object", &Context<YieldTermStructure<NumType>>::object)
        .def("idx", &Context<YieldTermStructure<NumType>>::idx);

    py::class_<Context<RateIndex<NumType>>>(m, "RateIndexContext")
        .def("object", &Context<RateIndex<NumType>>::object)
        .def("idx", &Context<RateIndex<NumType>>::idx);

    py::class_<YieldTermStructureStrategy<NumType>, std::shared_ptr<YieldTermStructureStrategy<NumType>>>(m, "YieldTermStructureStrategy")
        .def("discount", py::overload_cast<const Date&>(&YieldTermStructureStrategy<NumType>::discount, py::const_))
        .def("discount", py::overload_cast<double>(&YieldTermStructureStrategy<NumType>::discount, py::const_))
        .def("forwardRate", &YieldTermStructureStrategy<NumType>::forwardRate)
        .def("refDate", &YieldTermStructureStrategy<NumType>::refDate);

    py::class_<YieldTermStructure<NumType>>(m, "YieldTermStructure")
        .def(py::init([](std::shared_ptr<YieldTermStructureStrategy<NumType>> strat) { return YieldTermStructure<NumType>(strat->clone()); }))
        .def("discount", py::overload_cast<const Date&>(&YieldTermStructure<NumType>::discount, py::const_))
        .def("discount", py::overload_cast<double>(&YieldTermStructure<NumType>::discount, py::const_))
        .def("forwardRate", &YieldTermStructure<NumType>::forwardRate)
        .def("refDate", &YieldTermStructure<NumType>::refDate);

    py::class_<DiscountStrategy<NumType, LogLinearInterpolator<NumType>>, YieldTermStructureStrategy<NumType>,
               std::shared_ptr<DiscountStrategy<NumType, LogLinearInterpolator<NumType>>>>(m, "DiscountLogLinearStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<NumType>&, const DayCounter&>(), py::arg("dates"), py::arg("discounts"),
             py::arg("dayCounter") = Actual360())
        .def(py::init<const DiscountStrategy<NumType, LogLinearInterpolator<NumType>>&>())
        .def("enableExtrapolation", &DiscountStrategy<NumType, LogLinearInterpolator<NumType>>::enableExtrapolation);

    py::class_<FlatForwardStrategy<NumType>, YieldTermStructureStrategy<NumType>, std::shared_ptr<FlatForwardStrategy<NumType>>>(
        m, "FlatForwardStrategy")
        .def(py::init<const Date&, NumType, const DayCounter&, Compounding, Frequency>(), py::arg("refDate"), py::arg("rate"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple, py::arg("frequency") = Frequency::Annual)
        .def(py::init<const FlatForwardStrategy<NumType>&>());

    py::class_<ZeroRateStrategy<NumType, LinearInterpolator<NumType>>, YieldTermStructureStrategy<NumType>,
               std::shared_ptr<ZeroRateStrategy<NumType, LinearInterpolator<NumType>>>>(m, "ZeroRateLinearStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<NumType>&, const DayCounter&, Compounding, Frequency>(), py::arg("dates"),
             py::arg("zeroRates"), py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple,
             py::arg("frequency") = Frequency::Annual)
        .def(py::init<const ZeroRateStrategy<NumType, LinearInterpolator<NumType>>&>())
        .def("enableExtrapolation", &ZeroRateStrategy<NumType, LinearInterpolator<NumType>>::enableExtrapolation);

    // RateIndex
    py::class_<RateIndex<NumType>>(m, "RateIndex")
        .def(py::init<const Date&, Frequency, DayCounter, Frequency, Compounding>())
        .def(py::init<Frequency, DayCounter, Frequency, Compounding>())
        .def("addFixing", &RateIndex<NumType>::addFixing)
        .def("getFixing", &RateIndex<NumType>::getFixing);
};

#endif /* EEEB563F_626F_4BF4_B962_66C282CA0562 */
