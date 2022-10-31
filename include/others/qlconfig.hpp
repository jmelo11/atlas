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
#include <ql/math/interpolations/loginterpolation.hpp>

#include <ql/time/calendars/all.hpp>
#include <ql/currencies/all.hpp>


#include <ql/math/solvers1d/brent.hpp>

#include <ql/math/array.hpp>
#include <ql/math/matrix.hpp>

namespace atlas {
	
	/*
	* Solvers & MAth
	*/
	using BrentSolver = QuantLib::Brent;
	using Array = QuantLib::Array;
	using Matrix = QuantLib::Matrix;

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


	
}