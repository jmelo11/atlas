#ifndef AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D
#define AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D

#include <ql/compounding.hpp>
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
#include <atlas/atlasdefine.hpp>
#include <atlas/rates/interestrate.hpp>
#include <ql/currency.hpp>
#include <iostream>

namespace Atlas {

    // QuantLib types, to be replaced with our own types

    using Date                  = QuantLib::Date;
    using DayCounter            = QuantLib::DayCounter;
    using Calendar              = QuantLib::Calendar;
    using Period                = QuantLib::Period;
    using Frequency             = QuantLib::Frequency;
    using BusinessDayConvention = QuantLib::BusinessDayConvention;
    using Compounding           = QuantLib::Compounding;
    using Schedule              = QuantLib::Schedule;
    using MakeSchedule          = QuantLib::MakeSchedule;
    using Month                 = QuantLib::Month;
    using TimeUnit              = QuantLib::TimeUnit;
    using DateGeneration        = QuantLib::DateGeneration;
    using Currency              = QuantLib::Currency;
    // QuantLib day counters, to be replaced with our own day counters

    using Actual360      = QuantLib::Actual360;
    using Actual365Fixed = QuantLib::Actual365Fixed;
    using Thirty360      = QuantLib::Thirty360;
    using DiscountCurve  = QuantLib::DiscountCurve;

}  // namespace Atlas

#endif /* AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D */
