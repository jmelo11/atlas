#pragma once


#include <unordered_map>
#include <boost/preprocessor.hpp>
#include "../curvemanager/qlconfig.hpp"


#define SERIALIZE_ENUM_TO_CASES(r, data, elem) case elem : return BOOST_PP_STRINGIZE(elem);

#define SERIALIZE_ENUM_TO_MAP(r, data, elem) {BOOST_PP_STRINGIZE(elem), elem},

#define SERIALIZE_ENUM_WITH_CONVERSIONS(name, enumerators)                  \
    static enum name {                                                             \
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
	
	namespace C = CurveManager;

	/* header file with parsing string-to-ql or ql-to-string functions*/
	

		SERIALIZE_ENUM_WITH_CONVERSIONS(Currencies, (CLP)(CLF)(USD)(EUR))
		SERIALIZE_ENUM_WITH_CONVERSIONS(Compoundings, (Simple)(Compounded))
		SERIALIZE_ENUM_WITH_CONVERSIONS(DayCounters, (Act360)(Act365)(Thirty360))
		
		SERIALIZE_ENUM_WITH_CONVERSIONS(Calendars, (Chile)(NullCalendar)(USA)(Joint))
		SERIALIZE_ENUM_WITH_CONVERSIONS(TimeUnits, (Days)(Weeks)(Months)(Years))
		SERIALIZE_ENUM_WITH_CONVERSIONS(BDConventions, (Following)(Unadjusted)(ModifiedFollowing))
		
		SERIALIZE_ENUM_WITH_CONVERSIONS(Frequencies, (Annual)(Semiannual)(Monthly)(Quarterly))
	
		
		static enum DateFormat {
			MIXED,
			SLASH,
			HYPHEN
		};
		
		static std::string parseDate(const C::Date& date, DateFormat format) {
			std::string day = date.dayOfMonth()<10 ? "0" + std::to_string(date.dayOfMonth()) : std::to_string(date.dayOfMonth());
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
					break;
			}			
		};
		
		static C::Date parseDate(const std::string& date, DateFormat format) {
			int day, month, year;
			switch (format)
			{
				case MIXED:
					day = std::stoi(date.substr(0, 2));
					month = std::stoi(date.substr(3, 2));
					year = std::stoi(date.substr(6, 4));
					return C::Date(day, (C::Month)month, year);
				case SLASH || HYPHEN:
					day = std::stoi(date.substr(0, 2));
					month = std::stoi(date.substr(2, 2));
					year = std::stoi(date.substr(4, 4));
					return C::Date(day, (C::Month)month, year);			
				default:
					break;
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
					break;
			}			
		};
		
		static std::string parseCurrency(const C::Currency& curr) {
			return curr.name();
		};

		static C::DayCounter parseDayCounter(const std::string& dayCounter) {
			switch (map_DayCounters.at(dayCounter))
			{
				case Act360:
					return C::Act360();
				case Act365:
					return C::Act365();
				case Thirty360:
					return C::Thirty360(C::Thirty360::BondBasis);
				default:
					break;
			}
		};
		
		static std::string parseDayCounter(C::DayCounter& dayCounter) {
			return dayCounter.name();
		};

		static C::Compounding parseCompounding(const std::string& compounding) {
			switch (map_Compoundings.at(compounding))
			{
				case Simple:
					return C::Compounding::Simple;
				case Compounded:
					return C::Compounding::Compounded;
				default:
					break;
			}
		
		};
		
		static std::string parseCompounding(C::Compounding& compounding) {
			switch (compounding)
			{
				case C::Compounding::Simple:
					return "Simple";
				case C::Compounding::Compounded:
					return "Compounded";
				default:
					break;
			}
		};

		static C::Frequency parseFrequency(const std::string& frequency) {
			switch (map_Frequencies.at(frequency))
			{
				case Annual:
					return C::Frequency::Annual;
				case Semiannual:
					return C::Frequency::Semiannual;
				case Monthly:
					return C::Frequency::Monthly;
				case Quarterly:
					return C::Frequency::Quarterly;
				default:
					break;
			}
		};
		
		static std::string parseFrequency(C::Frequency& frequency) {
		switch (frequency)
			{
				case C::Frequency::Annual:
					return "Annual";
				case C::Frequency::Semiannual:
					return "Semiannual";
				case C::Frequency::Monthly:
					return "Monthly";
				case C::Frequency::Quarterly:
					return "Quarterly";
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
				case Days:
					return C::TimeUnit::Days;
				case Weeks:
					return C::TimeUnit::Weeks;
				case Months:
					return C::TimeUnit::Months;
				case Years:
					return C::TimeUnit::Years;
				default:
					break;
			}
		};


		static C::Calendar parseCalendar(const std::string& calendar) {
			switch (map_Calendars.at(calendar))
			{
			case USA:
				return C::UnitedStates(C::UnitedStates::Market::NYSE);
			case Chile:
				return C::Chile();
			case NullCalendar:
				return C::NullCalendar();
			case Joint:
				return C::JoinCalendar(C::Chile(), C::UnitedStates(C::UnitedStates::Market::NYSE));
			default:
				return C::NullCalendar();
			}
		}
		
		static std::string parseCalendar(C::Calendar & calendar) {
			return calendar.name();
		}
		
		static C::BusinessDayConvention parseDBConvention(const std::string& convention) {
			switch (map_BDConventions.at(convention))
				{
				case Following:
					return C::BusinessDayConvention::Following;
				case ModifiedFollowing:
					return C::BusinessDayConvention::ModifiedFollowing;
				case Unadjusted:
					return C::BusinessDayConvention::Unadjusted;
				default:
					break;
				}
		
		};
}