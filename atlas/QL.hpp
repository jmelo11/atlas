#pragma once
#include <ql/time/date.hpp>
#include <ql/utilities/dataparsers.hpp>
#include <ql/time/schedule.hpp>
#include <ql/interestrate.hpp>

#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/actual365Fixed.hpp>
#include <ql/time/daycounters/thirty360.hpp>

#include <ql/math/matrix.hpp>
#include <ql/math/array.hpp>

#include <ql/math/interpolations/linearinterpolation.hpp>
#include <ql/math/interpolations/cubicinterpolation.hpp>

#include <ql/math/solvers1d/brent.hpp>

namespace atlas {
	/*
	* Date and time
	*/
	using Date = QuantLib::Date;
	using DateParser = QuantLib::DateParser;
	using Month = QuantLib::Month;

	using Schedule = QuantLib::Schedule;
	using makeSchedule = QuantLib::MakeSchedule;
	using Period = QuantLib::Period;
	using TimeUnit = QuantLib::TimeUnit;
	using Frequency = QuantLib::Frequency;
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
	* Math
	*/
	using Array = QuantLib::Array;
	using Matrix = QuantLib::Matrix;
	/*
	* Curves/interpolation
	*/
	using Linear = QuantLib::Linear;
	using Cubic = QuantLib::Cubic;
	using Interpolation = QuantLib::Interpolation;
	/*
	* Solver
	*/
	using BrentSolver = QuantLib::Brent;
}