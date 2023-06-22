#ifndef EEEB563F_626F_4BF4_B962_66C282CA0562
#define EEEB563F_626F_4BF4_B962_66C282CA0562

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
    py::class_<InterestRate<dual>>(m, "InterestRate")
        .def(py::init<>())
        .def(py::init<dual, DayCounter, Compounding, Frequency>(), py::arg("rate"), py::arg("dc") = Actual360(),
             py::arg("comp") = Compounding::Simple, py::arg("freq") = Frequency::Annual)
        .def("rate", &InterestRate<dual>::rate)
        .def("compoundFactor", py::overload_cast<double>(&InterestRate<dual>::compoundFactor, py::const_))
        .def("compoundFactor", py::overload_cast<const Date&, const Date&, const Date&, const Date&>(&InterestRate<dual>::compoundFactor, py::const_))
        .def("discountFactor", py::overload_cast<const Date&, const Date&, const Date&, const Date&>(&InterestRate<dual>::discountFactor, py::const_))
        .def("discountFactor", py::overload_cast<double>(&InterestRate<dual>::discountFactor, py::const_))
        .def("__str__", [](const InterestRate<dual>& r) {
            std::string msg = "InterestRate(" + std::to_string(val(r.rate())) + ", " + r.dayCounter().name() + ", " +
                              std::to_string(r.compounding()) + ", " + std::to_string(r.frequency()) + ")";
            return msg;
        });

    // Context
    py::class_<Context<YieldTermStructure<dual>>>(m, "CurveContext")
        .def("object", &Context<YieldTermStructure<dual>>::object)
        .def("idx", &Context<YieldTermStructure<dual>>::idx);

    py::class_<Context<RateIndex<dual>>>(m, "RateIndexContext")
        .def("object", &Context<RateIndex<dual>>::object)
        .def("idx", &Context<RateIndex<dual>>::idx);

    py::class_<YieldTermStructureStrategy<dual>, std::shared_ptr<YieldTermStructureStrategy<dual>>>(m, "YieldTermStructureStrategy")
        .def("discount", py::overload_cast<const Date&>(&YieldTermStructureStrategy<dual>::discount, py::const_))
        .def("discount", py::overload_cast<double>(&YieldTermStructureStrategy<dual>::discount, py::const_))
        .def("forwardRate", &YieldTermStructureStrategy<dual>::forwardRate)
        .def("refDate", &YieldTermStructureStrategy<dual>::refDate);

    py::class_<YieldTermStructure<dual>>(m, "YieldTermStructure")
        .def(py::init([](std::shared_ptr<YieldTermStructureStrategy<dual>> strat) { return YieldTermStructure<dual>(strat->clone()); }))
        .def("discount", py::overload_cast<const Date&>(&YieldTermStructure<dual>::discount, py::const_))
        .def("discount", py::overload_cast<double>(&YieldTermStructure<dual>::discount, py::const_))
        .def("forwardRate", &YieldTermStructure<dual>::forwardRate)
        .def("refDate", &YieldTermStructure<dual>::refDate);

    py::class_<DiscountStrategy<dual, LogLinearInterpolator<dual>>, YieldTermStructureStrategy<dual>,
               std::shared_ptr<DiscountStrategy<dual, LogLinearInterpolator<dual>>>>(m, "DiscountLogLinearStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<dual>&, const DayCounter&>(), py::arg("dates"), py::arg("discounts"),
             py::arg("dayCounter") = Actual360())
        .def(py::init<const DiscountStrategy<dual, LogLinearInterpolator<dual>>&>())
        .def("enableExtrapolation", &DiscountStrategy<dual, LogLinearInterpolator<dual>>::enableExtrapolation);

    py::class_<FlatForwardStrategy<dual>, YieldTermStructureStrategy<dual>, std::shared_ptr<FlatForwardStrategy<dual>>>(m, "FlatForwardStrategy")
        .def(py::init<const Date&, dual, const DayCounter&, Compounding, Frequency>(), py::arg("refDate"), py::arg("rate"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple, py::arg("frequency") = Frequency::Annual)
        .def(py::init<const FlatForwardStrategy<dual>&>());

    py::class_<ZeroRateStrategy<dual, LinearInterpolator<dual>>, YieldTermStructureStrategy<dual>,
               std::shared_ptr<ZeroRateStrategy<dual, LinearInterpolator<dual>>>>(m, "ZeroRateLinearStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<dual>&, const DayCounter&, Compounding, Frequency>(), py::arg("dates"),
             py::arg("zeroRates"), py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple,
             py::arg("frequency") = Frequency::Annual)
        .def(py::init<const ZeroRateStrategy<dual, LinearInterpolator<dual>>&>())
        .def("enableExtrapolation", &ZeroRateStrategy<dual, LinearInterpolator<dual>>::enableExtrapolation);

    // RateIndex
    py::class_<RateIndex<dual>>(m, "RateIndex")
        .def(py::init<const Date&, Frequency, DayCounter, Frequency, Compounding>())
        .def(py::init<Frequency, DayCounter, Frequency, Compounding>())
        .def("addFixing", &RateIndex<dual>::addFixing)
        .def("getFixing", &RateIndex<dual>::getFixing);
};

#endif /* EEEB563F_626F_4BF4_B962_66C282CA0562 */
