#pragma once

#include "parser.h"

namespace parsers {
	/*
	* JSONParser
	*/

	JSONParser::JSONParser(const json& data) : data_(data) { initialize(); }
	JSONParser::JSONParser() { initialize(); }

	void JSONParser::setData(const json& data) { data_ = data; }

	QL::Date JSONParser::parseDate(std::string date) {
		return std::move(QuantLib::DateParser::parseFormatted(date, "%d-%m-%Y"));
	}
	std::string JSONParser::parseDate(QL::Date& date) {		
		return std::move(std::to_string(date.dayOfMonth()) + "-" + std::to_string(date.month()) + "-" + std::to_string(date.year()));
	}

	QL::Currency JSONParser::parseCurrency(std::string currency) {
		switch (currencyMap_[currency])
		{
		case Currency::CLP:
			return std::move(QL::CLPCurrency());
		case Currency::UF:
			return std::move(QL::CLFCurrency());
		case Currency::USD:
			return std::move(QL::USDCurrency());
		case Currency::EUR:
			return std::move(QL::EURCurrency());
			
		default:
			break;
		}
	}
	std::string JSONParser::parseCurrency(QL::Currency& curr) {		
		if (curr.code()=="CLP")
		{
			return "CLP";
		}
		else if (curr.name()=="CLF")
		{
			return "UF";
		}
		else if (curr.name()=="USD")
		{
			return "USD";
		}		
	}

	QL::DayCounter JSONParser::parseDayCounter(std::string dayCounter) {
		switch (dayCounterMap_[dayCounter])
		{
		case DayCounter::Act360:
			return std::move(QL::Actual360());
		case DayCounter::Act365:
			return std::move(QL::Actual365Fixed());
		case DayCounter::Thirty360:
			return std::move(QL::Thirty360(QL::Thirty360::BondBasis));
		default:
			break;
		}
	}
	/*
	* todo: filter with enum
	*/
	std::string JSONParser::parseDayCounter(QL::DayCounter& dayCounter) {
		std::string strDayCounter;
		if (dayCounter.name()=="Actual/360")
		{
			strDayCounter = "Act360";
		}
		else if (dayCounter.name() == "30/360 (Bond Basis)")
		{
			strDayCounter = "Thirty360";
		}
		else if (dayCounter.name() == "Actual/365 (Fixed)")
		{
			strDayCounter = "Act360";
		}
		return strDayCounter;
	}

	QL::Compounding JSONParser::parseCompounding(std::string compounding) {
		switch (compoundingMap_[compounding])
		{
		case Compounding::Simple:
			return QL::Simple;
		case Compounding::Compounded:
			return QL::Compounded;
		default:
			break;
		}
	}
	std::string JSONParser::parseCompounding(QL::Compounding& compounding) {
		std::string strComp;
		switch (compounding)
		{
		case QuantLib::Simple:
			strComp = "Simple";
			return strComp;			
		case QuantLib::Compounded:
			strComp = "Compounded";
			return strComp;
		case QuantLib::Continuous:
			return strComp;
		case QuantLib::SimpleThenCompounded:
			return strComp;
		case QuantLib::CompoundedThenSimple:
			return strComp;
		default:
			return strComp;
		}
	}

	QL::Frequency JSONParser::parseFrequency(std::string frequency) {
		switch (frequencyMap_[frequency])
		{
		case Frequency::Annual:
			return QL::Annual;
		case Frequency::Semiannual:
			return QL::Semiannual;
		case Frequency::Monthly:
			return QL::Monthly;
		case Frequency::Quarterly:
			return QL::Quarterly;
		default:
			break;
		}
	}
	std::string JSONParser::parseFrequency(QL::Frequency& frequency) {
		std::string strFreq;
		switch (frequency)
		{
		case QuantLib::NoFrequency:
			return strFreq;
		case QuantLib::Once:
			return strFreq;
		case QuantLib::Annual:
			strFreq = "Annual";
			return strFreq;			
		case QuantLib::Semiannual:
			strFreq = "Semiannual";
			return strFreq;
		case QuantLib::EveryFourthMonth:
			return strFreq;
		case QuantLib::Quarterly:
			strFreq = "Quarterly";
			return strFreq;
		case QuantLib::Bimonthly:
			return strFreq;
		case QuantLib::Monthly:
			strFreq = "Monthly";
			return strFreq;
		case QuantLib::EveryFourthWeek:
			return strFreq;
		case QuantLib::Biweekly:
			return strFreq;
		case QuantLib::Weekly:
			return strFreq;
		case QuantLib::Daily:
			strFreq = "Daily";
			return strFreq;
		case QuantLib::OtherFrequency:
			return strFreq;
		default:
			return strFreq;
		}
	}

	QL::TimeUnit JSONParser::parseTimeUnit(std::string timeUnit) {
		switch (timeUnitMap_[timeUnit])
		{
		case TimeUnit::Days:
			return QL::Days;
		case TimeUnit::Months:
			return QL::Months;
		case TimeUnit::Years:
			return QL::Years;
		case TimeUnit::Weeks:
			return QL::Weeks;
		default:
			break;
		}
	}
	QL::Calendar JSONParser::parseCalendar(std::string calendar) {
		switch (calendarMap_[calendar])
		{
		case Calendar::Chile:
			return std::move(QL::Chile(QL::Chile::Market::SSE));
		case Calendar::NullCalendar:
			return std::move(QL::NullCalendar());			
		case Calendar::USA:
			return std::move(QL::UnitedStates(QL::UnitedStates::Market::FederalReserve));			
		default:
			break;
		}
	}
	QL::BusinessDayConvention JSONParser::parseDBConvention(std::string convention) {
		switch (dbConventionMap_[convention])
		{
		case BDConvention::Following:
			return QL::Following;
		case BDConvention::Unadjusted:
			return QL::Unadjusted;
		case BDConvention::ModFollowing:
			return QL::ModifiedFollowing;
		default:
			break;
		}
	}


	void JSONParser::initialize() {
		initCurrencyMap();
		initDayCounterMap();
		initCompoundingMap();
		initFreqMap();
		initDBConventionMap();
		initCalendarMap();
		initTimeUnitMap();
	}
	void JSONParser::initCurrencyMap() {
		currencyMap_.insert({ "CLP", Currency::CLP });
		currencyMap_.insert({ "UF", Currency::UF });
		currencyMap_.insert({ "USD", Currency::USD });
		currencyMap_.insert({ "EUR", Currency::EUR });
	}
	void JSONParser::initDayCounterMap() {
		dayCounterMap_.insert({ "Act360", DayCounter::Act360 });
		dayCounterMap_.insert({ "Act365", DayCounter::Act365 });
		dayCounterMap_.insert({ "Thirty360", DayCounter::Thirty360 });
	}
	void JSONParser::initCompoundingMap() {
		compoundingMap_.insert({ "Simple", Compounding::Simple });
		compoundingMap_.insert({ "Compounded", Compounding::Compounded });
	}
	void JSONParser::initDBConventionMap() {
		dbConventionMap_.insert({ "Unadjusted", BDConvention::Unadjusted });
		dbConventionMap_.insert({ "ModFollowing", BDConvention::ModFollowing });
		dbConventionMap_.insert({ "Following", BDConvention::Following });
	}
	void JSONParser::initCalendarMap() {
		calendarMap_.insert({ "Chile", Calendar::Chile });
		calendarMap_.insert({ "NullCalendar", Calendar::NullCalendar });
		calendarMap_.insert({ "USA", Calendar::USA });
	}
	void JSONParser::initFreqMap() {
		frequencyMap_.insert({ "Annual", Frequency::Annual });
		frequencyMap_.insert({ "Semiannual", Frequency::Semiannual });
		frequencyMap_.insert({ "Monthly", Frequency::Monthly });
	}
	void JSONParser::initTimeUnitMap() {
		timeUnitMap_.insert({ "Days", TimeUnit::Days });
		timeUnitMap_.insert({ "Weeks", TimeUnit::Weeks });
		timeUnitMap_.insert({ "Months", TimeUnit::Months });
		timeUnitMap_.insert({ "Years", TimeUnit::Years });
	}
}