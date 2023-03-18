#ifndef AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D
#define AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D

#include <ql/compounding.hpp>
#include <ql/interestrate.hpp>
#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/frequency.hpp>
#include <ql/time/period.hpp>
#include <ql/time/schedule.hpp>
#include <autodiff/forward/dual.hpp>

namespace Atlas {
#ifdef USE_AUTODIFF
    using Number = autodiff::dual;
#else
    using Number = double;
#endif

    // QuantLib types, to be replaced with our own types

    using Date                  = QuantLib::Date;
    using DayCounter            = QuantLib::DayCounter;
    using Calendar              = QuantLib::Calendar;
    using Period                = QuantLib::Period;
    using Frequency             = QuantLib::Frequency;
    using BusinessDayConvention = QuantLib::BusinessDayConvention;
    using Compounding           = QuantLib::Compounding;
    using InterestRate          = QuantLib::InterestRate;
    using Schedule              = QuantLib::Schedule;
    using MakeSchedule          = QuantLib::MakeSchedule;
    using Month                 = QuantLib::Month;

    // QuantLib day counters, to be replaced with our own day counters

    using Actual360      = QuantLib::Actual360;
    using Actual365Fixed = QuantLib::Actual365Fixed;
    using Thirty360      = QuantLib::Thirty360;

    using DiscountCurve = QuantLib::DiscountCurve;
}  // namespace Atlas

#endif /* AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D */
