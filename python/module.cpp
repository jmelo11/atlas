// QuantLib
#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/termstructures/yield/flatforward.hpp>

// data
#include <atlas/data/marketdata.hpp>

// models
#include <atlas/models/staticcurvemodel.hpp>

// curves
#include <atlas/rates/curvecontext.hpp>
#include <atlas/rates/curvecontextstore.hpp>
#include <atlas/rates/curvestrategy.hpp>

// visitors
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/durationcalculator.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>

// pybind11
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

// cashflows
#include <atlas/cashflows/cashflow.hpp>
#include <atlas/cashflows/coupon.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/floatingratecoupon.hpp>

// legs
#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>
#include <atlas/cashflows/legs/leg.hpp>

// instruments
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/instruments/instrument.hpp>

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

PYBIND11_MODULE(Atlas, m) {
    m.doc() = "Atlas";  // optional module docstring

    // QL Types
    // enums
    py::enum_<Compounding>(m, "Compounding")
        .value("Simple", Compounding::Simple)
        .value("Compounded", Compounding::Compounded)
        .value("Continuous", Compounding::Continuous)
        .value("SimpleThenCompounded", Compounding::SimpleThenCompounded)
        .value("CompoundedThenSimple", Compounding::CompoundedThenSimple)
        .export_values();

    py::enum_<Frequency>(m, "Frequency")
        .value("NoFrequency", Frequency::NoFrequency)
        .value("Once", Frequency::Once)
        .value("Annual", Frequency::Annual)
        .value("Semiannual", Frequency::Semiannual)
        .value("EveryFourthMonth", Frequency::EveryFourthMonth)
        .value("Quarterly", Frequency::Quarterly)
        .value("Bimonthly", Frequency::Bimonthly)
        .value("Monthly", Frequency::Monthly)
        .value("EveryFourthWeek", Frequency::EveryFourthWeek)
        .value("Biweekly", Frequency::Biweekly)
        .value("Weekly", Frequency::Weekly)
        .value("Daily", Frequency::Daily)
        .value("OtherFrequency", Frequency::OtherFrequency)
        .export_values();

    py::enum_<Month>(m, "Month")
        .value("January", Month::January)
        .value("February", Month::February)
        .value("March", Month::March)
        .value("April", Month::April)
        .value("May", Month::May)
        .value("June", Month::June)
        .value("July", Month::July)
        .value("August", Month::August)
        .value("September", Month::September)
        .value("October", Month::October)
        .value("November", Month::November)
        .value("December", Month::December)
        .export_values();

    py::enum_<DateGeneration::Rule>(m, "DateGenerationRule")
        .value("Backward", DateGeneration::Rule::Backward)
        .value("Forward", DateGeneration::Rule::Forward)
        .value("Zero", DateGeneration::Rule::Zero)
        .value("ThirdWednesday", DateGeneration::Rule::ThirdWednesday)
        .value("Twentieth", DateGeneration::Rule::Twentieth)
        .value("TwentiethIMM", DateGeneration::Rule::TwentiethIMM)
        .value("OldCDS", DateGeneration::Rule::OldCDS)
        .value("CDS", DateGeneration::Rule::CDS)
        .value("CDS2015", DateGeneration::Rule::CDS2015)
        .export_values();

    py::enum_<BusinessDayConvention>(m, "BusinessDayConvention")
        .value("Following", BusinessDayConvention::Following)
        .value("ModifiedFollowing", BusinessDayConvention::ModifiedFollowing)
        .value("Preceding", BusinessDayConvention::Preceding)
        .value("ModifiedPreceding", BusinessDayConvention::ModifiedPreceding)
        .value("Unadjusted", BusinessDayConvention::Unadjusted)
        .value("HalfMonthModifiedFollowing", BusinessDayConvention::HalfMonthModifiedFollowing)
        .value("Nearest", BusinessDayConvention::Nearest)
        .export_values();

    py::enum_<TimeUnit>(m, "TimeUnit")
        .value("Days", TimeUnit::Days)
        .value("Weeks", TimeUnit::Weeks)
        .value("Months", TimeUnit::Months)
        .value("Years", TimeUnit::Years)
        .export_values();

    // classes
    py::class_<Period>(m, "Period").def(py::init<int, TimeUnit>());

    py::class_<Actual360>(m, "Actual360").def(py::init<>()).def("dayCount", &Actual360::dayCount).def("yearFraction", &Actual360::yearFraction);

    py::class_<Actual365Fixed>(m, "Actual365Fixed")
        .def(py::init<>())
        .def("dayCount", &Actual365Fixed::dayCount)
        .def("yearFraction", &Actual365Fixed::yearFraction);

    py::class_<Thirty360>(m, "ActualActual")
        .def(py::init<Thirty360::Convention, const Date&>())
        .def("dayCount", &Thirty360::dayCount)
        .def("yearFraction", &Thirty360::yearFraction);

    py::class_<Schedule>(m, "Schedule")
        .def(py::init<const Date&, const Date&, const Period&, const Calendar&, BusinessDayConvention, BusinessDayConvention, DateGeneration::Rule,
                      bool>())
        .def("size", &Schedule::size)
        .def("dates", &Schedule::dates);

    py::class_<MakeSchedule>(m, "MakeSchedule")
        .def(py::init<>())
        .def("from", &MakeSchedule::from)
        .def("to", &MakeSchedule::to)
        .def("withTenor", &MakeSchedule::withTenor)
        .def("withCalendar", &MakeSchedule::withCalendar)
        .def("withConvention", &MakeSchedule::withConvention)
        .def("withTerminationDateConvention", &MakeSchedule::withTerminationDateConvention)
        .def("withRule", &MakeSchedule::withRule)
        .def("endOfMonth", &MakeSchedule::endOfMonth)
        .def("forwards", &MakeSchedule::forwards)
        .def("backwards", &MakeSchedule::backwards)
        .def("endOfMonth", &MakeSchedule::endOfMonth);

    // Curve
    py::class_<CurveContext>(m, "CurveContext").def("curve", &CurveContext::curve).def("index", &CurveContext::index).def("idx", &CurveContext::idx);

    py::class_<CurveContextStore>(m, "CurveContextStore")
        .def("instance", &CurveContextStore::instance)
        .def("createCurveContext", &CurveContextStore::createCurveContext)
        .def("at", py::overload_cast<const std::string&>(&CurveContextStore::at, py::const_), "Get a curve context by name")
        .def("at", py::overload_cast<size_t>(&CurveContextStore::at, py::const_), "Get a curve context by index")
        .def("hasContext", &CurveContextStore::hasContext)
        .def("copyContextsFromStore", &CurveContextStore::copyContextsFromStore);

    // CurveStrategy -> Trampoline?
    py::class_<CurveStrategy, std::shared_ptr<CurveStrategy>>(m, "CurveStrategy");

    py::class_<DiscountCurveStrategy, CurveStrategy>(m, "DiscountCurveStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<double>&, DayCounter>())
        .def(py::init<const DiscountCurveStrategy&>())
        .def("copy", &DiscountCurveStrategy::copy)
        .def("curve", &DiscountCurveStrategy::curve);

    py::class_<FlatForwardStrategy, CurveStrategy>(m, "FlatForwardStrategy")
        .def(py::init<const Date&, double, const DayCounter&, Compounding, Frequency>())
        .def(py::init<const FlatForwardStrategy&>())
        .def("copy", &FlatForwardStrategy::copy)
        .def("curve", &FlatForwardStrategy::curve);

    // Cashflows
    py::class_<Cashflow<dual>>(m, "Cashflow")
        .def(py::init<>())
        .def(py::init<const CurveContext&>())
        .def(py::init<const Date&, dual>())
        .def(py::init<const Date&, dual, const CurveContext&>())
        .def("amount", &Cashflow<dual>::amount)
        .def("paymentDate", &Cashflow<dual>::paymentDate)
        .def("hasOcurred", &Cashflow<dual>::hasOcurred)
        .def("discountCurveContext", &Cashflow<dual>::discountCurveContext)
        .def("hasDiscountContext", &Cashflow<dual>::hasDiscountContext)
        .def("discountContextIdx", &Cashflow<dual>::discountContextIdx);

    py::class_<Coupon<dual>, Aux::PyCoupon, Cashflow<dual>>(m, "Coupon")
        .def(py::init<const Date&, const Date&, double>())
        .def(py::init<const Date&, const Date&, double, const CurveContext&>())
        .def("notional", &Coupon<dual>::notional)
        .def("startDate", &Coupon<dual>::startDate)
        .def("endDate", &Coupon<dual>::endDate)
        .def("dayCounter", &Coupon<dual>::dayCounter)
        .def("accruedPeriod", &Coupon<dual>::accruedPeriod)
        .def("accruedAmount", &Coupon<dual>::accruedAmount);

    py::class_<FixedRateCoupon<dual>, Coupon<dual>>(m, "FixedRateCoupon")
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&>())
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&, const CurveContext&>())
        .def("rate", py::overload_cast<>(&FixedRateCoupon<dual>::rate, py::const_))
        .def("rate", py::overload_cast<const InterestRate<dual>&>(&FixedRateCoupon<dual>::rate))
        .def("accruedPeriod", &FixedRateCoupon<dual>::accruedPeriod)
        .def("accruedAmount", &FixedRateCoupon<dual>::accruedAmount)
        .def("dayCounter", &FixedRateCoupon<dual>::dayCounter);

    py::class_<FloatingRateCoupon<dual>, Coupon<dual>>(m, "FloatingRateCoupon")
        .def(py::init<const Date&, const Date&, double, dual, const CurveContext&>())
        .def(py::init<const Date&, const Date&, double, dual, const CurveContext&, const CurveContext&>())
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
    py::class_<Leg<dual>>(m, "Leg");
    
}