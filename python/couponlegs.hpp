#ifndef F01E58DD_C649_4B27_B416_BE9F8F03E002
#define F01E58DD_C649_4B27_B416_BE9F8F03E002

#include <atlas/fundation/indexable.hpp>
// cashflows
#include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/coupon.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>

// legs
#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/cashflows/legs/leg.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {
    class PyCoupon : public Coupon<dual> {
       public:
        using Coupon<dual>::Coupon;

        DayCounter dayCounter() const override {
            PYBIND11_OVERRIDE_PURE(DayCounter,   /* Return type */
                                   Coupon<dual>, /* Parent class */
                                   dayCounter,   /* Name of function in C++ (must match Python name) */
                                                 /* Argument(s) */
            );
        }

        double accruedPeriod(const Date& start, const Date& end) const override {
            PYBIND11_OVERRIDE_PURE(double,        /* Return type */
                                   Coupon<dual>,  /* Parent class */
                                   accruedPeriod, /* Name of function in C++ (must match Python name) */
                                   start,         /* Argument(s) */
                                   end);
        }

        dual accruedAmount(const Date& start, const Date& end) const override {
            PYBIND11_OVERRIDE_PURE(dual,          /* Return type */
                                   Coupon<dual>,  /* Parent class */
                                   accruedAmount, /* Name of function in C++ (must match Python name) */
                                   start,         /* Argument(s) */
                                   end);
        }
    };

}  // namespace Aux

void py_couponlegs(py::module& m) {
    py::class_<Cashflow<dual>, Indexable>(m, "Cashflow")
        .def(py::init<>())
        .def(py::init<const Context<YieldTermStructure<dual>>&>())
        .def(py::init<const Date&, dual>())
        .def(py::init<const Date&, dual, const Context<YieldTermStructure<dual>>&>())
        .def("paymentDate", &Cashflow<dual>::paymentDate)
        .def("amount", py::overload_cast<>(&Cashflow<dual>::amount, py::const_))
        .def("hasOcurred", &Cashflow<dual>::hasOcurred)
        .def("discountCurveContext", &Cashflow<dual>::discountCurveContext)
        .def("hasDiscountContext", &Cashflow<dual>::hasDiscountContext)
        .def("discountContextIdx", &Cashflow<dual>::discountContextIdx)
        .def("currency", &Cashflow<dual>::currency)
        .def("currencyCode", &Cashflow<dual>::currencyCode);

    py::class_<Coupon<dual>, Aux::PyCoupon, Cashflow<dual>>(m, "Coupon")
        .def(py::init<const Date&, const Date&, double>())
        .def(py::init<const Date&, const Date&, double, const Context<YieldTermStructure<dual>>&>())
        .def("notional", &Coupon<dual>::notional)
        .def("startDate", &Coupon<dual>::startDate)
        .def("endDate", &Coupon<dual>::endDate)
        .def("dayCounter", &Coupon<dual>::dayCounter)
        .def("accruedPeriod", &Coupon<dual>::accruedPeriod)
        .def("accruedAmount", &Coupon<dual>::accruedAmount);

    py::class_<FixedRateCoupon<dual>, Coupon<dual>>(m, "FixedRateCoupon")
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&>())
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&, const Context<YieldTermStructure<dual>>&>())
        .def("rate", py::overload_cast<>(&FixedRateCoupon<dual>::rate, py::const_))
        .def("rate", py::overload_cast<const InterestRate<dual>&>(&FixedRateCoupon<dual>::rate))
        .def("accruedPeriod", &FixedRateCoupon<dual>::accruedPeriod)
        .def("accruedAmount", &FixedRateCoupon<dual>::accruedAmount)
        .def("dayCounter", &FixedRateCoupon<dual>::dayCounter);

    py::class_<FloatingRateCoupon<dual>, Coupon<dual>>(m, "FloatingRateCoupon")
        .def(py::init<const Date&, const Date&, double, dual, const Context<RateIndex<dual>>&>())
        .def(py::init<const Date&, const Date&, double, dual, const Context<RateIndex<dual>>&, const Context<YieldTermStructure<dual>>&>())
        .def("spread", py::overload_cast<>(&FloatingRateCoupon<dual>::spread, py::const_))
        .def("spread", py::overload_cast<dual>(&FloatingRateCoupon<dual>::spread))
        .def("fixing", py::overload_cast<>(&FloatingRateCoupon<dual>::fixing, py::const_))
        .def("fixing", py::overload_cast<dual>(&FloatingRateCoupon<dual>::fixing))
        .def("forecastCurveContext", &FloatingRateCoupon<dual>::forecastCurveContext)
        .def("accruedAmount", &FloatingRateCoupon<dual>::accruedAmount)
        .def("dayCounter", &FloatingRateCoupon<dual>::dayCounter)
        .def("accruedPeriod", &FloatingRateCoupon<dual>::accruedPeriod)
        .def("forecastContextIdx", &FloatingRateCoupon<dual>::forecastContextIdx)
        .def("hasForecastContext", &FloatingRateCoupon<dual>::hasForecastContext);

    // Legs
    py::class_<Leg<dual>>(m, "Leg")
        .def(py::init<std::vector<Redemption<dual>>&, bool>())
        .def("redemptions", py::overload_cast<>(&Leg<dual>::redemptions, py::const_), py::return_value_policy::reference)
        .def("redemptions", py::overload_cast<>(&Leg<dual>::redemptions), py::return_value_policy::reference)
        .def("addRedemption", &Leg<dual>::addRedemption)
        .def("sortRedemptions", &Leg<dual>::sortRedemptions);

    py::class_<FixedRateLeg<dual>, Leg<dual>>(m, "FixedRateLeg")
        .def(py::init<std::vector<FixedRateCoupon<dual>>&, std::vector<Redemption<dual>>&, bool>())
        .def("coupons", py::overload_cast<>(&FixedRateLeg<dual>::coupons, py::const_), py::return_value_policy::reference)
        .def("coupons", py::overload_cast<>(&FixedRateLeg<dual>::coupons), py::return_value_policy::reference)
        .def("addCoupon", &FixedRateLeg<dual>::addCoupon)
        .def("sort", &FixedRateLeg<dual>::sort)
        .def("discountCurveContext", &FixedRateLeg<dual>::discountCurveContext);

    py::class_<FloatingRateLeg<dual>, Leg<dual>>(m, "FloatingRateLeg")
        .def(py::init<std::vector<FloatingRateCoupon<dual>>&, std::vector<Redemption<dual>>&, bool>())
        .def("coupons", py::overload_cast<>(&FloatingRateLeg<dual>::coupons, py::const_), py::return_value_policy::reference)
        .def("coupons", py::overload_cast<>(&FloatingRateLeg<dual>::coupons), py::return_value_policy::reference)
        .def("addCoupon", &FloatingRateLeg<dual>::addCoupon)
        .def("sort", &FloatingRateLeg<dual>::sort)
        .def("discountCurveContext", &FloatingRateLeg<dual>::discountCurveContext)
        .def("forecastCurveContext", &FloatingRateLeg<dual>::forecastCurveContext);
};

#endif /* F01E58DD_C649_4B27_B416_BE9F8F03E002 */
