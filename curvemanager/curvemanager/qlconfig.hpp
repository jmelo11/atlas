#pragma once
#include <ql/time/date.hpp>
#include <ql/utilities/dataparsers.hpp>
#include <ql/time/schedule.hpp>
#include <ql/interestrate.hpp>

#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/actual365Fixed.hpp>
#include <ql/time/daycounters/thirty360.hpp>

#include <ql/math/interpolations/linearinterpolation.hpp>
#include <ql/math/interpolations/cubicinterpolation.hpp>

#include <ql/termstructures/yieldtermstructure.hpp>
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>
#include <ql/handle.hpp>
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <ql/termstructures/yield/bondhelpers.hpp>

#include <ql/experimental/termstructures/basisswapratehelpers.hpp>
#include <qle/termstructures/crossccyfixfloatswaphelper.hpp>
#include <ql/termstructures/yield/oisratehelper.hpp>

#include <ql/indexes/iborindex.hpp>
#include <ql/indexes/all.hpp>

#include <ql/time/calendars/all.hpp>
#include <ql/currencies/all.hpp>

namespace CurveManager {
	
	/*
	* setting
	*/
	
	using Settings = QuantLib::Settings;
	using PeriodParser = QuantLib::PeriodParser;

	/*
	* Date and time
	*/
		
	using Month = QuantLib::Month;	
	using Date = QuantLib::Date;
	using DateParser = QuantLib::DateParser;	
	using Calendar = QuantLib::Calendar;
	using Schedule = QuantLib::Schedule;
	using makeSchedule = QuantLib::MakeSchedule;
	using Period = QuantLib::Period;
	using TimeUnit = QuantLib::TimeUnit;
	using Frequency = QuantLib::Frequency;
	using BusinessDayConvention = QuantLib::BusinessDayConvention;

	/*
	* Calendar
	*/
	
	using UnitedStates = QuantLib::UnitedStates;
	using Chile = QuantLib::Chile;
	using NullCalendar = QuantLib::NullCalendar;
	using JoinCalendar = QuantLib::JointCalendar;
	
	/*
	* Currencies 
	*/
	
	using Currency = QuantLib::Currency;
	using USDCurrency = QuantLib::USDCurrency;
	using EURCurrency = QuantLib::EURCurrency;
	using JPYCurrency = QuantLib::JPYCurrency;
	using CLPCurrency = QuantLib::CLPCurrency;
	using CLFCurrency = QuantLib::CLFCurrency;
		
	/*
	* Rates
	*/

	using InterestRate = QuantLib::InterestRate;
	using DayCounter = QuantLib::DayCounter;
	using Act360 = QuantLib::Actual360;
	using Thirty360 = QuantLib::Thirty360;
	using Act365 = QuantLib::Actual365Fixed;
	using Compounding = QuantLib::Compounding;

	/*
	* Interpolation
	*/

	using Linear = QuantLib::Linear;
	using Cubic = QuantLib::Cubic;
	using LogLinear = QuantLib::LogLinear;
	using LogCubic = QuantLib::LogCubic;
	
	using Interpolation = QuantLib::Interpolation;

	/*
	* TermStructure
	*/
	
	using YieldTermStructure = QuantLib::YieldTermStructure;	
	
	template <class Interpol, class DataType>
	using PiecewiseYieldCurve = QuantLib::PiecewiseYieldCurve<Interpol, DataType>;
	
	using RateHelper = QuantLib::RateHelper;
	
	template<typename T>
	using Handle = QuantLib::Handle<T>;
	using Quote = QuantLib::Quote;
	using SimpleQuote = QuantLib::SimpleQuote;
	using SwapIndex = QuantLib::SwapIndex;
	using OvernightIndex = QuantLib::OvernightIndex;
	using IborIndex = QuantLib::IborIndex;
	
	/*
	* DataTypes
	*/
	using Discount = QuantLib::Discount;
	using ForwardRate = QuantLib::ForwardRate;
	
	
	/*
	* Rate helpers
	*/
	//derivates
	using SwapRateHelper = QuantLib::SwapRateHelper;
	using FxSwapRateHelper = QuantLib::FxSwapRateHelper;
	
	using OISRateHelper = QuantLib::OISRateHelper;
	
	using IborIborBasisSwapRateHelper = QuantLib::IborIborBasisSwapRateHelper;
	using OvernightIborBasisSwapRateHelper = QuantLib::OvernightIborBasisSwapRateHelper;
	using CrossCcyFixFloatSwapHelper = QuantExt::CrossCcyFixFloatSwapHelper;
	//using ConstNotionalCrossCurrencyBasisSwapRateHelper = QuantLib::ConstNotionalCrossCurrencyBasisSwapRateHelper;
	
	//fixed rate helpers
	using DepositRateHelper = QuantLib::DepositRateHelper;
	using FixedRateBondHelper = QuantLib::FixedRateBondHelper;
	using CPIBondHelper = QuantLib::CPIBondHelper;

	/*
	* Instruments
	*/
	
	using FixedRateBond = QuantLib::FixedRateBond;
}