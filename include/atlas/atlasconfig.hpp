#ifndef AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D
#define AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D

#include <ql/compounding.hpp>
#include <ql/currencies/america.hpp>
#include <ql/currencies/asia.hpp>
#include <ql/currencies/europe.hpp>
#include <ql/currencies/oceania.hpp>
#include <ql/currency.hpp>
#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/calendars/all.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/frequency.hpp>
#include <ql/time/period.hpp>
#include <ql/time/schedule.hpp>
#include <ql/utilities/dataparsers.hpp>
#include <atlas/atlasdefine.hpp>
#include <atlas/rates/interestrate.hpp>
#include <iostream>
/**
 * @brief Atlas namespace
 */
namespace Atlas {
    // QuantLib types, to be replaced with our own types
    using Date                  = QuantLib::Date;
    using DateParser            = QuantLib::DateParser;
    using DayCounter            = QuantLib::DayCounter;
    using Calendar              = QuantLib::Calendar;
    using Period                = QuantLib::Period;
    using PeriodParser          = QuantLib::PeriodParser;
    using Frequency             = QuantLib::Frequency;
    using BusinessDayConvention = QuantLib::BusinessDayConvention;
    using Compounding           = QuantLib::Compounding;
    using Schedule              = QuantLib::Schedule;
    using MakeSchedule          = QuantLib::MakeSchedule;
    using Month                 = QuantLib::Month;
    using TimeUnit              = QuantLib::TimeUnit;
    using DateGeneration        = QuantLib::DateGeneration;
    // QuantLib day counters, to be replaced with our own day counters

    using Actual360      = QuantLib::Actual360;
    using Actual365Fixed = QuantLib::Actual365Fixed;
    using Thirty360      = QuantLib::Thirty360;
    using DiscountCurve  = QuantLib::DiscountCurve;

    // QuantLib Currencies+
    using Rounding = QuantLib::Rounding;
    using Currency = QuantLib::Currency;
    using USD      = QuantLib::USDCurrency;
    using EUR      = QuantLib::EURCurrency;
    using GBP      = QuantLib::GBPCurrency;
    using CHF      = QuantLib::CHFCurrency;
    using JPY      = QuantLib::JPYCurrency;
    using CAD      = QuantLib::CADCurrency;
    using AUD      = QuantLib::AUDCurrency;
    using NZD      = QuantLib::NZDCurrency;
    using SEK      = QuantLib::SEKCurrency;
    using NOK      = QuantLib::NOKCurrency;
    using DKK      = QuantLib::DKKCurrency;
    using HKD      = QuantLib::HKDCurrency;
    using SGD      = QuantLib::SGDCurrency;
    using KRW      = QuantLib::KRWCurrency;
    using MXN      = QuantLib::MXNCurrency;
    using BRL      = QuantLib::BRLCurrency;
    using TRY      = QuantLib::TRYCurrency;
    using RUB      = QuantLib::RUBCurrency;
    using INR      = QuantLib::INRCurrency;
    using CNY      = QuantLib::CNYCurrency;
    using TWD      = QuantLib::TWDCurrency;
    using THB      = QuantLib::THBCurrency;
    using AED      = QuantLib::AEDCurrency;
    using SAR      = QuantLib::SARCurrency;
    using ILS      = QuantLib::ILSCurrency;
    using CLP      = QuantLib::CLPCurrency;
    using CLF      = QuantLib::CLFCurrency;
    using PEN      = QuantLib::PENCurrency;
    using COP      = QuantLib::COPCurrency;
    using ARS      = QuantLib::ARSCurrency;

    // calendars
    using NullCalendar  = QuantLib::NullCalendar;
    using UnitedStates  = QuantLib::UnitedStates;
    using UnitedKingdom = QuantLib::UnitedKingdom;
    using TARGET        = QuantLib::TARGET;
    using Chile         = QuantLib::Chile;
    using Japan         = QuantLib::Japan;
    using Canada        = QuantLib::Canada;
    using Australia     = QuantLib::Australia;
    using NewZealand    = QuantLib::NewZealand;
    using Sweden        = QuantLib::Sweden;
    using Norway        = QuantLib::Norway;
    using Denmark       = QuantLib::Denmark;
    using HongKong      = QuantLib::HongKong;
    using Singapore     = QuantLib::Singapore;
    using SouthKorea    = QuantLib::SouthKorea;
    using Mexico        = QuantLib::Mexico;
    using Brazil        = QuantLib::Brazil;
    using Turkey        = QuantLib::Turkey;
    using Russia        = QuantLib::Russia;
    using India         = QuantLib::India;
    using China         = QuantLib::China;
    using Taiwan        = QuantLib::Taiwan;
    using Thailand      = QuantLib::Thailand;

    // Other types
    enum class Side { Pay = -1, Receive = 1 };

}  // namespace Atlas

namespace std {
    /**
     * @brief hash specialization for Currency
     *
     * @tparam
     */
    template <>
    struct hash<Atlas::Currency> {
        size_t operator()(const Atlas::Currency& curr) const { return std::hash<size_t>()(curr.numericCode()); }
    };
}  // namespace std

#endif /* AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D */
