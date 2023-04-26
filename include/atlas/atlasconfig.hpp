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
#include <iostream>

/**
 * @brief Atlas namespace
 */
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

}  // namespace Atlas

#endif /* AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D */
