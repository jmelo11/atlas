#pragma once


#include <unordered_map>
#include <boost/preprocessor.hpp>
#include <others/qlconfig.hpp>


#define SERIALIZE_ENUM_TO_CASES(r, data, elem) case elem : return BOOST_PP_STRINGIZE(elem);

#define SERIALIZE_ENUM_TO_MAP(r, data, elem) {BOOST_PP_STRINGIZE(elem), elem},

#define SERIALIZE_ENUM_WITH_CONVERSIONS(name, enumerators)                  \
    enum name {                                                             \
        BOOST_PP_SEQ_ENUM(enumerators)                                      \
    };														\
	static const std::unordered_map<std::string, name> map_##name{									\
		BOOST_PP_SEQ_FOR_EACH(												\
			SERIALIZE_ENUM_TO_MAP,										\
			name,															\
			enumerators														\
		)																	\
	};			\
    static inline const char* ToString(name v)                                     \
    {                                                                       \
        switch (v)                                                          \
        {                                                                   \
            BOOST_PP_SEQ_FOR_EACH(                                          \
                SERIALIZE_ENUM_TO_CASES,								    \
                name,                                                       \
                enumerators                                                 \
            )                                                               \
            default: return "[Unknown " BOOST_PP_STRINGIZE(name) "]";       \
        }                                                                   \
    };  	

namespace parser {

	namespace C = atlas;

	/* header file with parsing string-to-ql or ql-to-string functions*/


	SERIALIZE_ENUM_WITH_CONVERSIONS(Currencies, (CLP)(CLF)(USD)(EUR))
	SERIALIZE_ENUM_WITH_CONVERSIONS(Compoundings, (SIMPLE)(COMPOUNDED)(CONTINUOUS))
	SERIALIZE_ENUM_WITH_CONVERSIONS(DayCounters, (ACT360)(ACT365)(THIRTY360))
	SERIALIZE_ENUM_WITH_CONVERSIONS(Calendars, (CHILE)(NULLCALENDAR)(USA)(JOINT))
	SERIALIZE_ENUM_WITH_CONVERSIONS(TimeUnits, (DAYS)(WEEKS)(MONTHS)(YEARS))
	SERIALIZE_ENUM_WITH_CONVERSIONS(BDConventions, (FOLLOWING)(UNADJUSTED)(MODIFIEDFOLLOWING))
	SERIALIZE_ENUM_WITH_CONVERSIONS(Frequencies, (ONCE)(ANNUAL)(SEMIANNUAL)(MONTHLY)(QUARTERLY)(NOFREQUENCY))



	enum DateFormat {
		MIXED,
		SLASH,
		HYPHEN
	};

	static std::string parseDate(const C::Date& date, DateFormat format) {
		std::string day = date.dayOfMonth() < 10 ? "0" + std::to_string(date.dayOfMonth()) : std::to_string(date.dayOfMonth());
		std::string month = date.month() < 10 ? "0" + std::to_string(date.month()) : std::to_string(date.month());
		switch (format)
		{
		case MIXED:
			return day + month + std::to_string(date.year());
		case SLASH:
			return day + "/" + month + "/" + std::to_string(date.year());
		case HYPHEN:
			return day + "-" + month + "-" + std::to_string(date.year());
		default:
			return day + month + std::to_string(date.year());
		}
	};

	static C::Date parseDate(const std::string& date, DateFormat format = DateFormat::MIXED) {
		int day, month, year;
		if (format == DateFormat::MIXED)
		{
			day = std::stoi(date.substr(0, 2));
			month = std::stoi(date.substr(2, 2));
			year = std::stoi(date.substr(4, 4));
		}
		else if (format == DateFormat::HYPHEN || format == DateFormat::SLASH){
			day = std::stoi(date.substr(0, 2));
			month = std::stoi(date.substr(2, 2));
			year = std::stoi(date.substr(4, 4));
		}
		else{
			throw std::runtime_error("Unknown date format: "+ format);
		}
		try{
			return C::Date(day, (C::Month)month, year);
		}
		catch (std::exception&) {
			throw std::runtime_error("Invalid date: " + date);
		}
		

	};

	static C::Currency parseCurrency(const std::string& currency) {
		switch (map_Currencies.at(currency))
		{
		case CLP:
			return C::CLPCurrency();
		case CLF:
			return C::CLFCurrency();
		case USD:
			return C::USDCurrency();
		case EUR:
			return C::EURCurrency();
		default:
			throw std::runtime_error("Unknown currency: " + currency);
		}
	};

	static std::string parseCurrency(const C::Currency& curr) {
		return curr.name();
	};

	static C::DayCounter parseDayCounter(const std::string& dayCounter) {
		switch (map_DayCounters.at(dayCounter))
		{
		case ACT360:
			return C::Act360();
		case ACT365:
			return C::Act365();
		case THIRTY360:
			return C::Thirty360(C::Thirty360::BondBasis);
		default:
			throw std::runtime_error("Unknown day counter:"+ dayCounter);
		}
	};

	static std::string parseDayCounter(C::DayCounter& dayCounter) {
		return dayCounter.name();
	};

	static C::Compounding parseCompounding(const std::string& compounding) {
		switch (map_Compoundings.at(compounding))
		{
		case SIMPLE:
			return C::Compounding::Simple;
		case COMPOUNDED:
			return C::Compounding::Compounded;
		case CONTINUOUS:
			return C::Compounding::Continuous;
		default:
			throw std::runtime_error("Unknown compounding:" + compounding);
		}

	};

	static std::string parseCompounding(C::Compounding& compounding) {
		switch (compounding)
		{
		case C::Compounding::Simple:
			return "SIMPLE";
		case C::Compounding::Compounded:
			return "COMPOUNDED";
		default:
			break;
		}
	};

	static C::Frequency parseFrequency(const std::string& frequency) {
		switch (map_Frequencies.at(frequency))
		{
		case ANNUAL:
			return C::Frequency::Annual;
		case SEMIANNUAL:
			return C::Frequency::Semiannual;
		case MONTHLY:
			return C::Frequency::Monthly;
		case QUARTERLY:
			return C::Frequency::Quarterly;
		case ONCE:
			return C::Frequency::Once;
		case NOFREQUENCY:
			return C::Frequency::NoFrequency;
		default:
			throw std::runtime_error("Unknown frequency:" + frequency);
		}
	};

	static std::string parseFrequency(C::Frequency& frequency) {
		switch (frequency)
		{
		case C::Frequency::Annual:
			return "ANNUAL";
		case C::Frequency::Semiannual:
			return "SEMIANNUAL";
		case C::Frequency::Monthly:
			return "MONTHLY";
		case C::Frequency::Quarterly:
			return "QUARTERLY";
		case C::Frequency::Once:
			return "ONCE";
		default:
			break;
		}
	};

	static C::Period parsePeriod(const std::string& period) {
		return C::PeriodParser::parse(period);
	}

	static std::string parsePeriod(const C::Period& period) {
		return std::to_string(period.length()) + std::to_string(period.units());
	}

	static C::TimeUnit parseTimeUnit(const std::string& timeUnit) {
		switch (map_TimeUnits.at(timeUnit))
		{
		case DAYS:
			return C::TimeUnit::Days;
		case WEEKS:
			return C::TimeUnit::Weeks;
		case MONTHS:
			return C::TimeUnit::Months;
		case YEARS:
			return C::TimeUnit::Years;
		default:
			throw std::runtime_error("Unknown timeunit:" + timeUnit);
		}
	};

	static C::Calendar parseCalendar(const std::string& calendar) {
		switch (map_Calendars.at(calendar))
		{
		case USA:
			return C::UnitedStates(C::UnitedStates::Market::Settlement);
		case CHILE:
			return C::Chile();
		case NULLCALENDAR:
			return C::NullCalendar();
		case JOINT:
			return C::JoinCalendar(C::Chile(), C::UnitedStates(C::UnitedStates::Market::Settlement));
		default:
			throw std::runtime_error("Unknown calendar: " + calendar);
		}
	}

	static std::string parseCalendar(C::Calendar& calendar) {
		return calendar.name();
	}

	static C::BusinessDayConvention parseBusinessDayConvention(const std::string& convention) {
		switch (map_BDConventions.at(convention))
		{
		case FOLLOWING:
			return C::BusinessDayConvention::Following;
		case MODIFIEDFOLLOWING:
			return C::BusinessDayConvention::ModifiedFollowing;
		case UNADJUSTED:
			return C::BusinessDayConvention::Unadjusted;
		default:
			throw std::runtime_error("Unknown convention: " + convention);
		}

	};			
}