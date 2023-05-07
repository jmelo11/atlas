// fundation
#include <atlas/atlasdefine.hpp>
#include <atlas/fundation/context.hpp>
// data
#include <atlas/data/marketdata.hpp>

// models
#include <atlas/models/spotmarketdatamodel.hpp>

// curves
#include <atlas/rates/yieldtermstructure.hpp>
#include <atlas/rates/yieldtermstructure/discountcurve.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/rates/yieldtermstructure/zeroratecurve.hpp>

// othres
#include <atlas/others/interpolations/loglinearinterpolation.hpp>

// visitors
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/durationcalculator.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <atlas/visitors/zspreadcalculator.hpp>

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
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
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
             py::arg("startDate"), py::arg("endDate"), py::arg("notional"), py::arg("spread"), py::arg("rateIndexContext"),                 \
             py::arg("discountCurveContext"))                                                                                               \
        .def(py::init<const Date&, const Date&, double, dual, const Context<RateIndex<dual>>&>(), py::arg("startDate"), py::arg("endDate"), \
             py::arg("notional"), py::arg("spread"), py::arg("rateIndexContext"))

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    inline std::string dateToStr(const Date& date) {
        if (date == Date()) return "None";
        std::string d, m, y;
        if (date.dayOfMonth() < 10) {
            d = "0" + std::to_string(date.dayOfMonth());
        } else {
            d = std::to_string(date.dayOfMonth());
        }

        if (date.month() < 10) {
            m = "0" + std::to_string(date.month());
        } else {
            m = std::to_string(date.month());
        }
        y = std::to_string(date.year());
        return d + "-" + m + "-" + y;
    }

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

    class PyModel : public Model<dual> {
        using Model<dual>::Model;

        MarketData<dual> marketData(const Date& evalDate = Date()) const override {
            PYBIND11_OVERRIDE_PURE(MarketData<dual>, /* Return type */
                                   Model<dual>,      /* Parent class */
                                   marketData,       /* Name of function in C++ (must match Python name) */
                                   evalDate          /* Argument(s) */
            );
        };
    };
};  // namespace Aux

PYBIND11_MODULE(Atlas, m) {
    m.doc() = "Atlas";  // optional module docstring

    // autodiff
    m.def("getValue", [](const dual& d) { return val(d); });

    py::class_<dual>(m, "Dual")
        .def(py::init<double>())
        .def("__str__", [](const dual& d) { return "Dual(" + std::to_string(val(d)) + ")"; })
        .def("__repr__", [](const dual& d) { return "Dual(" + std::to_string(val(d)) + ")"; })
        .def("__float__", [](const dual& d) { return val(d); })
        .def("setDerivative", py::overload_cast<double>(&dual::setDerivative))
        .def("getDerivative", py::overload_cast<>(&dual::getDerivative, py::const_));

    py::class_<tape_type, std::shared_ptr<tape_type>>(m, "Tape")
        .def(py::init<>())
        .def("registerInput", py::overload_cast<dual&>(&tape_type::registerInput))
        .def("registerOutput", py::overload_cast<dual&>(&tape_type::registerOutput))
        .def("computeAdjoints", &tape_type::computeAdjoints)
        .def("newRecording", &tape_type::newRecording)
        .def("clearAll", &tape_type::clearAll)
        .def("getMemory", &tape_type::getMemory)
        .def("clearDerivatives", &tape_type::clearDerivatives);

    // QL Types
    // Date
    py::class_<Date>(m, "Date")
        .def(py::init<int, Month, int>())
        .def(py::init<const Date&>())
        .def("serialNumber", &Date::serialNumber)
        .def("weekday", &Date::weekday)
        .def("dayOfMonth", &Date::dayOfMonth)
        .def("month", &Date::month)
        .def("year", &Date::year)
        .def("__str__", [](const Date& d) { return Aux::dateToStr(d); })
        .def("__repr__", [](const Date& d) { return Aux::dateToStr(d); })
        .def(py::self + int())
        .def(py::self - int())
        .def(py::self - py::self)
        .def(py::self += int())
        .def(py::self -= int())
        .def("__lt__", [](const Date& d1, const Date& d2) { return d1 < d2; })
        .def("__le__", [](const Date& d1, const Date& d2) { return d1 <= d2; })
        .def("__gt__", [](const Date& d1, const Date& d2) { return d1 > d2; })
        .def("__ge__", [](const Date& d1, const Date& d2) { return d1 >= d2; })
        .def("__eq__", [](const Date& d1, const Date& d2) { return d1 == d2; })
        .def("__ne__", [](const Date& d1, const Date& d2) { return d1 != d2; })
        .def("__hash__", [](const Date& d) { return std::hash<Date>()(d); });

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

    // currencies
    py::class_<Rounding>(m, "Rounding").def("__call__", &Rounding::operator());

    py::class_<Currency>(m, "Currency")
        .def("name", &Currency::name)
        .def("code", &Currency::code)
        .def("numericCode", &Currency::numericCode)
        .def("symbol", &Currency::symbol)
        .def("rounding", &Currency::rounding);

    py::class_<USD, Currency>(m, "USD").def(py::init<>());
    py::class_<CLP, Currency>(m, "CLP").def(py::init<>());
    py::class_<EUR, Currency>(m, "EUR").def(py::init<>());
    py::class_<GBP, Currency>(m, "GBP").def(py::init<>());
    py::class_<JPY, Currency>(m, "JPY").def(py::init<>());
    py::class_<MXN, Currency>(m, "MXN").def(py::init<>());
    py::class_<NOK, Currency>(m, "NOK").def(py::init<>());
    py::class_<SEK, Currency>(m, "SEK").def(py::init<>());
    py::class_<CHF, Currency>(m, "CHF").def(py::init<>());
    py::class_<CAD, Currency>(m, "CAD").def(py::init<>());
    py::class_<AUD, Currency>(m, "AUD").def(py::init<>());
    py::class_<NZD, Currency>(m, "NZD").def(py::init<>());
    py::class_<CNY, Currency>(m, "CNY").def(py::init<>());
    py::class_<HKD, Currency>(m, "HKD").def(py::init<>());
    py::class_<CLF, Currency>(m, "CLF").def(py::init<>());

    // classes
    py::class_<DayCounter>(m, "DayCounter").def("__str__", &DayCounter::name);

    py::class_<Period>(m, "Period").def(py::init<int, TimeUnit>());

    py::class_<Actual360, DayCounter>(m, "Actual360")
        .def(py::init<>())
        .def("dayCount", &Actual360::dayCount)
        .def("yearFraction", &Actual360::yearFraction);

    py::class_<Actual365Fixed, DayCounter>(m, "Actual365")
        .def(py::init<>())

        .def("dayCount", &Actual365Fixed::dayCount)
        .def("yearFraction", &Actual365Fixed::yearFraction);

    py::class_<Thirty360, DayCounter>(m, "Thirty360")
        .def(py::init([]() { return Thirty360(Thirty360::BondBasis); }))
        .def("dayCount", &Thirty360::dayCount)
        .def("yearFraction", &Thirty360::yearFraction);

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
    ;

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

    // Context
    py::class_<Context<YieldTermStructure<dual>>>(m, "CurveContext")
        .def("object", &Context<YieldTermStructure<dual>>::object)
        .def("idx", &Context<YieldTermStructure<dual>>::idx);

    py::class_<Context<RateIndex<dual>>>(m, "RateIndexContext")
        .def("object", &Context<RateIndex<dual>>::object)
        .def("idx", &Context<RateIndex<dual>>::idx);

    py::class_<MarketStore<dual>>(m, "MarketStore")
        .def(py::init<const Date&, Currency>(), py::arg("refDate"), py::arg("localCcy"))
        .def("addCurve", &MarketStore<dual>::addCurve)
        .def("curveContext", py::overload_cast<const std::string&>(&MarketStore<dual>::curveContext, py::const_), "Get a curve context by name")
        .def("curveContext", py::overload_cast<size_t>(&MarketStore<dual>::curveContext, py::const_), "Get a curve context by index")
        .def("rateIndexContext", py::overload_cast<const std::string&>(&MarketStore<dual>::rateIndexContext, py::const_),
             "Get a rate index context by name")
        .def("rateIndexContext", py::overload_cast<size_t>(&MarketStore<dual>::rateIndexContext, py::const_), "Get a rate index context by index")
        .def("cloneFromStore", &MarketStore<dual>::cloneFromStore)
        .def("addExchangeRate", &MarketStore<dual>::addExchangeRate)
        .def("exchange", py::overload_cast<const Currency&, const Currency&>(&MarketStore<dual>::exchange, py::const_),
             "Get the exchange rate between two currencies")
        .def("riskFreeCurveIdx", py::overload_cast<const Currency&>(&MarketStore<dual>::riskFreeCurveIdx, py::const_),
             "Set the risk free curve index for a currency");

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
        .def(py::init<const DiscountStrategy<dual, LogLinearInterpolator<dual>>&>());

    py::class_<FlatForwardStrategy<dual>, YieldTermStructureStrategy<dual>, std::shared_ptr<FlatForwardStrategy<dual>>>(m, "FlatForwardStrategy")
        .def(py::init<const Date&, dual, const DayCounter&, Compounding, Frequency>(), py::arg("refDate"), py::arg("rate"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple, py::arg("frequency") = Frequency::Annual)
        .def(py::init<const FlatForwardStrategy<dual>&>());

    py::class_<ZeroRateStrategy<dual, LinearInterpolator<dual>>, YieldTermStructureStrategy<dual>,
               std::shared_ptr<ZeroRateStrategy<dual, LinearInterpolator<dual>>>>(m, "ZeroRateLinearStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<dual>&, const DayCounter&, Compounding, Frequency>(), py::arg("dates"),
             py::arg("zeroRates"), py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple,
             py::arg("frequency") = Frequency::Annual)
        .def(py::init<const ZeroRateStrategy<dual, LinearInterpolator<dual>>&>());

    // RateIndex
    py::class_<RateIndex<dual>>(m, "RateIndex")
        .def(py::init<const Date&, Frequency, DayCounter, Frequency, Compounding>())
        .def("addFixing", &RateIndex<dual>::addFixing)
        .def("getFixing", &RateIndex<dual>::getFixing);

    // MarketData & MarketRequest

    py::class_<MarketRequest::ForwardRate>(m, "ForwardRateRequest").def("__str__", [](const MarketRequest::ForwardRate& r) {
        std::string rep = "ForwardRateRequest(startDate=" + Aux::dateToStr(r.startDate_) + ", endDate=" + Aux::dateToStr(r.endDate_) +
                          ", idx=" + std::to_string(r.curve_) + ")";
        return rep;
    });

    py::class_<MarketRequest::DiscountFactor>(m, "DiscountFactorRequest").def("__str__", [](const MarketRequest::DiscountFactor& r) {
        std::string rep = "DiscountFactorRequest(date=" + Aux::dateToStr(r.date_) + ", idx=" + std::to_string(r.curve_) + ")";
        return rep;
    });

    py::class_<MarketRequest::ExchangeRate>(m, "ExchangeRateRequest").def("__str__", [](const MarketRequest::ExchangeRate& r) {
        std::string rep =
            "ExchangeRateRequest(ccy1=" + std::to_string(r.ccy1_) + ", ccy2=" + std::to_string(r.ccy2_) + ", date=" + Aux::dateToStr(r.date_) + ")";
        return rep;
    });

    py::class_<MarketData<dual>>(m, "MarketData")
        .def_readonly("refDate", &MarketData<dual>::refDate)
        .def_readonly("dfs", &MarketData<dual>::dfs)
        .def_readonly("fwds", &MarketData<dual>::fwds)
        .def_readonly("fxs", &MarketData<dual>::fxs);

    py::class_<MarketRequest>(m, "MarketRequest")
        .def_readonly("dfs", &MarketRequest::dfs)
        .def_readonly("fwd", &MarketRequest::fwds)
        .def_readonly("fxs", &MarketRequest::fxs);

    // Cashflows
    py::class_<Cashflow<dual>>(m, "Cashflow")
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

    // Instruments
    py::class_<Instrument<dual>, Aux::PyInstrument>(m, "Instrument")
        .def("startDate", &Instrument<dual>::startDate)
        .def("endDate", &Instrument<dual>::endDate)
        .def("notional", &Instrument<dual>::notional)
        .def("accept", py::overload_cast<Visitor<dual>&>(&Instrument<dual>::accept))
        .def("accept", py::overload_cast<ConstVisitor<dual>&>(&Instrument<dual>::accept, py::const_));

    py::class_<FloatingRateInstrument<dual>, Instrument<dual>>(m, "FloatingRateInstrument")
        .def(py::init<const Date&, const Date&, double, dual, const FloatingRateLeg<dual>&>())
        .def("leg", py::overload_cast<>(&FloatingRateInstrument<dual>::leg, py::const_))
        .def("leg", py::overload_cast<>(&FloatingRateInstrument<dual>::leg))
        .def("spread", py::overload_cast<>(&FloatingRateInstrument<dual>::spread, py::const_))
        .def("spread", py::overload_cast<dual>(&FloatingRateInstrument<dual>::spread))
        .def("discountCurveContext", &FloatingRateInstrument<dual>::discountCurveContext)
        .def("forecastCurveContext", &FloatingRateInstrument<dual>::forecastCurveContext)
        .def("accept", py::overload_cast<Visitor<dual>&>(&FloatingRateInstrument<dual>::accept))
        .def("accept", py::overload_cast<ConstVisitor<dual>&>(&FloatingRateInstrument<dual>::accept, py::const_))
        .def("disbursement", py::overload_cast<>(&FloatingRateInstrument<dual>::disbursement))
        .def("disbursement", py::overload_cast<const Cashflow<dual>&>(&FloatingRateInstrument<dual>::disbursement))
        .def("currency", &FloatingRateInstrument<dual>::currency);

    py::class_<FixedRateInstrument<dual>, Instrument<dual>>(m, "FixedRateInstrument")
        .def(py::init<const Date&, const Date&, const InterestRate<dual>&, double, const FixedRateLeg<dual>&>())
        .def("leg", py::overload_cast<>(&FixedRateInstrument<dual>::leg, py::const_))
        .def("leg", py::overload_cast<>(&FixedRateInstrument<dual>::leg))
        .def("discountCurveContext", &FixedRateInstrument<dual>::discountCurveContext)
        .def("accept", py::overload_cast<Visitor<dual>&>(&FixedRateInstrument<dual>::accept))
        .def("accept", py::overload_cast<ConstVisitor<dual>&>(&FixedRateInstrument<dual>::accept, py::const_))
        .def("rate", py::overload_cast<>(&FixedRateInstrument<dual>::rate, py::const_))
        .def("rate", py::overload_cast<dual>(&FixedRateInstrument<dual>::rate))
        .def("rate", py::overload_cast<const InterestRate<dual>&>(&FixedRateInstrument<dual>::rate))
        .def("disbursement", py::overload_cast<>(&FixedRateInstrument<dual>::disbursement))
        .def("disbursement", py::overload_cast<const Cashflow<dual>&>(&FixedRateInstrument<dual>::disbursement))
        .def("currency", &FixedRateInstrument<dual>::currency);

    // Fixed rate instruments
    exportFixedRateInstrument(FixedRateBulletInstrument);
    exportFixedRateInstrument(FixedRateEqualRedemptionInstrument);

    py::class_<ZeroCouponInstrument<dual>, FixedRateInstrument<dual>>(m, "ZeroCouponInstrument")
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&>(), py::arg("startDate"), py::arg("endDate"), py::arg("notional"),
             py::arg("rate"))
        .def(py::init<const Date&, const Date&, double, const InterestRate<dual>&, const Context<YieldTermStructure<dual>>&>(), py::arg("startDate"),
             py::arg("endDate"), py::arg("notional"), py::arg("rate"), py::arg("discountCurveContext"));

    py::class_<EqualPaymentInstrument<dual>, FixedRateInstrument<dual>>(m, "EqualPaymentInstrument")
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<dual>&, bool>(), py::arg("startDate"), py::arg("endDate"),
             py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("recalculateNotionals") = false)
        .def(py::init<const Date&, const Date&, Frequency, double, const InterestRate<dual>&, const Context<YieldTermStructure<dual>>&, bool>(),
             py::arg("startDate"), py::arg("endDate"), py::arg("frequency"), py::arg("notional"), py::arg("rate"), py::arg("discountCurveContext"),
             py::arg("recalculateNotionals") = false);

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
        .def("currency", &OneLegMixin<dual, Leg<dual>>::currency);

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
    // Visitors
    py::class_<Visitor<dual>>(m, "Visitor")
        .def("visit", py::overload_cast<FloatingRateInstrument<dual>&>(&Visitor<dual>::visit))
        .def("visit", py::overload_cast<FixedRateInstrument<dual>&>(&Visitor<dual>::visit))
        .def("visit", py::overload_cast<FxForward<dual>&>(&Visitor<dual>::visit))
        .def("visit", py::overload_cast<FixFloatSwap<dual>&>(&Visitor<dual>::visit));

    py::class_<ConstVisitor<dual>>(m, "ConstVisitor")
        .def("visit", py::overload_cast<const FloatingRateInstrument<dual>&>(&ConstVisitor<dual>::visit, py::const_))
        .def("visit", py::overload_cast<const FixedRateInstrument<dual>&>(&ConstVisitor<dual>::visit, py::const_))
        .def("visit", py::overload_cast<const FxForward<dual>&>(&ConstVisitor<dual>::visit, py::const_))
        .def("visit", py::overload_cast<const FixFloatSwap<dual>&>(&ConstVisitor<dual>::visit, py::const_));

    py::class_<NPVCalculator<dual>, Visitor<dual>>(m, "NPVCalculator")
        .def(py::init<const MarketData<dual>&>(), py::arg("marketData"))
        .def("results", &NPVCalculator<dual>::results)
        .def("clear", &NPVCalculator<dual>::clear);

    py::class_<Indexer<dual>, Visitor<dual>>(m, "Indexer")
        .def(py::init<>())
        .def("request", &Indexer<dual>::request)
        .def("clear", &Indexer<dual>::clear);

    py::class_<ParSolver<dual>, ConstVisitor<dual>>(m, "ParSolver")
        .def(py::init<const MarketData<dual>&>(), py::arg("marketData"))
        .def("results", &ParSolver<dual>::results)
        .def("clear", &ParSolver<dual>::clear);

    py::class_<ZSpreadCalculator<dual>, ConstVisitor<dual>>(m, "ZSpreadCalculator")
        .def(py::init<const MarketData<dual>&, dual, const DayCounter&, Compounding, Frequency>(), py::arg("marketData"), py::arg("targetNpv"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Compounded, py::arg("frequency") = Frequency::Semiannual)
        .def("results", &ZSpreadCalculator<dual>::results)
        .def("clear", &ZSpreadCalculator<dual>::clear);

    py::class_<CashflowProfiler<dual>, ConstVisitor<dual>>(m, "CashflowProfiler")
        .def(py::init<>())
        .def("redemptions", &CashflowProfiler<dual>::redemptions)
        .def("interests", &CashflowProfiler<dual>::interests)
        .def("clear", &CashflowProfiler<dual>::clear);

    // Models
    py::class_<Model<dual>, Aux::PyModel>(m, "Model").def("marketData", &Model<dual>::marketData);

    py::class_<SpotMarketDataModel<dual>, Model<dual>>(m, "SpotMarketDataModel")
        .def(py::init<const MarketRequest&, const MarketStore<dual>&>(), py::arg("marketRequest"), py::arg("store"));
}