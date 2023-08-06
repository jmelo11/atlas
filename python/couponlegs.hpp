#ifndef F01E58DD_C649_4B27_B416_BE9F8F03E002
#define F01E58DD_C649_4B27_B416_BE9F8F03E002

#include "config.hpp"
#include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/cashflowstreammixins.hpp>
#include <atlas/cashflows/coupon.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>
#include <atlas/fundation/indexable.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyCoupon : public Coupon<NumType> {
       public:
        using Coupon<NumType>::Coupon;

        const DayCounter& dayCounter() const override {
            PYBIND11_OVERRIDE_PURE(const DayCounter&, /* Return type */
                                   Coupon<NumType>,   /* Parent class */
                                   dayCounter,        /* Name of function in C++ (must match Python name) */
                                                      /* Argument(s) */
            );
        }

        double accruedPeriod(const Date& start, const Date& end) const override {
            PYBIND11_OVERRIDE_PURE(double,          /* Return type */
                                   Coupon<NumType>, /* Parent class */
                                   accruedPeriod,   /* Name of function in C++ (must match Python name) */
                                   start,           /* Argument(s) */
                                   end);
        }

        NumType accruedAmount(const Date& start, const Date& end) const override {
            PYBIND11_OVERRIDE_PURE(NumType,         /* Return type */
                                   Coupon<NumType>, /* Parent class */
                                   accruedAmount,   /* Name of function in C++ (must match Python name) */
                                   start,           /* Argument(s) */
                                   end);
        }
    };

}  // namespace Aux

void py_couponlegs(py::module& m) {
    py::class_<Indexable>(m, "Indexable")
        .def("dfIdx", py::overload_cast<>(&Indexable::dfIdx, py::const_))
        .def("dfIdx", py::overload_cast<size_t>(&Indexable::dfIdx))
        .def("fwdIdx", py::overload_cast<>(&Indexable::fwdIdx, py::const_))
        .def("fwdIdx", py::overload_cast<size_t>(&Indexable::fwdIdx))
        .def("fxIdx", py::overload_cast<>(&Indexable::fxIdx, py::const_))
        .def("fxIdx", py::overload_cast<size_t>(&Indexable::fxIdx));

    py::class_<Cashflow<NumType>, Indexable>(m, "Cashflow")
        .def(py::init<>())
        .def(py::init<size_t>())
        .def(py::init<const Date&, NumType>())
        .def(py::init<const Date&, NumType, size_t>())
        .def("paymentDate", &Cashflow<NumType>::paymentDate)
        .def("amount", py::overload_cast<>(&Cashflow<NumType>::amount, py::const_))
        .def("hasOcurred", &Cashflow<NumType>::hasOcurred)
        .def("discountContextIdx", py::overload_cast<size_t>(&Cashflow<NumType>::discountContextIdx))
        .def("discountContextIdx", py::overload_cast<>(&Cashflow<NumType>::discountContextIdx, py::const_))
        .def("hasDiscountContext", &Cashflow<NumType>::hasDiscountContext)
        .def("currency", py::overload_cast<const Currency&>(&Cashflow<NumType>::currency))
        .def("currency", py::overload_cast<>(&Cashflow<NumType>::currency, py::const_))
        .def("currencyCode", &Cashflow<NumType>::currencyCode)
        .def("applyCurrency", py::overload_cast<>(&Cashflow<NumType>::applyCurrency, py::const_))
        .def("applyCurrency", py::overload_cast<bool>(&Cashflow<NumType>::applyCurrency));

    py::class_<Coupon<NumType>, Aux::PyCoupon, Cashflow<NumType>>(m, "Coupon")
        .def(py::init<const Date&, const Date&, double>())
        .def(py::init<const Date&, const Date&, double, size_t>())
        .def("notional", &Coupon<NumType>::notional)
        .def("startDate", &Coupon<NumType>::startDate)
        .def("endDate", &Coupon<NumType>::endDate)
        .def("dayCounter", &Coupon<NumType>::dayCounter)
        .def("accruedPeriod", &Coupon<NumType>::accruedPeriod)
        .def("accruedAmount", &Coupon<NumType>::accruedAmount);

    py::class_<FixedRateCoupon<NumType>, Coupon<NumType>>(m, "FixedRateCoupon")
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&>())
        .def(py::init<const Date&, const Date&, double, const InterestRate<NumType>&, size_t>())
        .def("rate", py::overload_cast<>(&FixedRateCoupon<NumType>::rate, py::const_))
        .def("rate", py::overload_cast<const InterestRate<NumType>&>(&FixedRateCoupon<NumType>::rate));

    py::class_<FloatingRateCoupon<NumType>, Coupon<NumType>>(m, "FloatingRateCoupon")
        .def(py::init<const Date&, const Date&, double, NumType, const RateDefinition&>())
        .def(py::init<const Date&, const Date&, double, NumType, const RateDefinition&, size_t, size_t>())
        .def("spread", py::overload_cast<>(&FloatingRateCoupon<NumType>::spread, py::const_))
        .def("spread", py::overload_cast<NumType>(&FloatingRateCoupon<NumType>::spread))
        .def("fixing", py::overload_cast<>(&FloatingRateCoupon<NumType>::fixing, py::const_))
        .def("fixing", py::overload_cast<NumType>(&FloatingRateCoupon<NumType>::fixing))
        .def("indexContextIdx", py::overload_cast<>(&FloatingRateCoupon<NumType>::indexContextIdx, py::const_))
        .def("indexContextIdx", py::overload_cast<size_t>(&FloatingRateCoupon<NumType>::indexContextIdx))
        .def("hasIndexContextIdx", &FloatingRateCoupon<NumType>::hasIndexContextIdx)
        .def("isFixingSet", &FloatingRateCoupon<NumType>::isFixingSet);

    py::class_<RedemptionStreamMixin<NumType>>(m, "RedemptionStreamMixin")
        .def("redemption", py::overload_cast<size_t>(&RedemptionStreamMixin<NumType>::redemption, py::const_), py::return_value_policy::reference)
        .def("redemption", py::overload_cast<size_t>(&RedemptionStreamMixin<NumType>::redemption), py::return_value_policy::reference)
        .def("redemptions", py::overload_cast<>(&RedemptionStreamMixin<NumType>::redemptions, py::const_), py::return_value_policy::reference)
        .def("redemptions", py::overload_cast<>(&RedemptionStreamMixin<NumType>::redemptions), py::return_value_policy::reference)
        .def("addRedemption", &RedemptionStreamMixin<NumType>::addRedemption);

    py::class_<FixedRateCouponStreamMixin<NumType>>(m, "FixedRateCouponStreamMixin")
        .def("fixedRateCoupon", py::overload_cast<size_t>(&FixedRateCouponStreamMixin<NumType>::fixedRateCoupon, py::const_),
             py::return_value_policy::reference)
        .def("fixedRateCoupon", py::overload_cast<size_t>(&FixedRateCouponStreamMixin<NumType>::fixedRateCoupon), py::return_value_policy::reference)
        .def("fixedRateCoupons", py::overload_cast<>(&FixedRateCouponStreamMixin<NumType>::fixedRateCoupons, py::const_),
             py::return_value_policy::reference)
        .def("fixedRateCoupons", py::overload_cast<>(&FixedRateCouponStreamMixin<NumType>::fixedRateCoupons), py::return_value_policy::reference)
        .def("addFixedRateCoupon", &FixedRateCouponStreamMixin<NumType>::addFixedRateCoupon);

    py::class_<FloatingRateCouponStreamMixin<NumType>>(m, "FloatingRateCouponStreamMixin")
        .def("floatingRateCoupon", py::overload_cast<size_t>(&FloatingRateCouponStreamMixin<NumType>::floatingRateCoupon, py::const_),
             py::return_value_policy::reference)
        .def("floatingRateCoupon", py::overload_cast<size_t>(&FloatingRateCouponStreamMixin<NumType>::floatingRateCoupon),
             py::return_value_policy::reference)
        .def("floatingRateCoupons", py::overload_cast<>(&FloatingRateCouponStreamMixin<NumType>::floatingRateCoupons, py::const_),
             py::return_value_policy::reference)
        .def("floatingRateCoupons", py::overload_cast<>(&FloatingRateCouponStreamMixin<NumType>::floatingRateCoupons),
             py::return_value_policy::reference)
        .def("addFloatingRateCoupon", &FloatingRateCouponStreamMixin<NumType>::addFloatingRateCoupon);

    py::class_<DisbursementStreamMixin<NumType>>(m, "DisbursementStreamMixin")
        .def("disbursement", py::overload_cast<size_t>(&DisbursementStreamMixin<NumType>::disbursement, py::const_),
             py::return_value_policy::reference)
        .def("disbursement", py::overload_cast<size_t>(&DisbursementStreamMixin<NumType>::disbursement), py::return_value_policy::reference)
        .def("disbursements", py::overload_cast<>(&DisbursementStreamMixin<NumType>::disbursements, py::const_), py::return_value_policy::reference)
        .def("disbursements", py::overload_cast<>(&DisbursementStreamMixin<NumType>::disbursements), py::return_value_policy::reference)
        .def("addDisbursement", &DisbursementStreamMixin<NumType>::addDisbursement);
};

#endif /* F01E58DD_C649_4B27_B416_BE9F8F03E002 */
