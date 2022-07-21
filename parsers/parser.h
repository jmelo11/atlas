#pragma once

#include <ql/currencies/america.hpp>
#include <ql/currencies/europe.hpp>

#include <ql/time/daycounters/all.hpp>
#include <ql/time/period.hpp>
#include <ql/time/schedule.hpp>

#include <ql/compounding.hpp>
#include <ql/time/frequency.hpp>

#include <ql/time/date.hpp>
#include <ql/time/calendars/chile.hpp>
#include <ql/time/calendars/nullcalendar.hpp>
#include <ql/time/calendars/unitedstates.hpp>
#include <ql/utilities/dataparsers.hpp>

#include <nlohmann/json.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>


#include <map>
#include <string>

/*
* Se debe reformular esta libreria por algo más simple
* No es necesario que sea un "json" parser, ya que esa funcionalidad la utiliza solo el servidor
*/
namespace parsers {
	namespace QL = QuantLib;
	namespace ext = boost;
	using json = nlohmann::json;

	
	class JSONParser {

	public:
		JSONParser(const json& data);
		JSONParser();

		void setData(const json& data);

		std::string parseDate(QL::Date& date);
		QL::Date parseDate(std::string date);

		QL::Currency parseCurrency(std::string currency);
		std::string parseCurrency(QL::Currency& curr);

		QL::DayCounter parseDayCounter(std::string dayCounter);
		std::string parseDayCounter(QL::DayCounter& dayCounter);

		QL::Compounding parseCompounding(std::string compounding);
		std::string parseCompounding(QL::Compounding& compounding);

		QL::Frequency parseFrequency(std::string frequency);
		std::string parseFrequency(QL::Frequency& frequency);

		QL::TimeUnit parseTimeUnit(std::string timeUnit);
		QL::Calendar parseCalendar(std::string calendar);
		QL::BusinessDayConvention parseDBConvention(std::string convention);

	private:
		void initialize();
		void initCurrencyMap();
		void initDayCounterMap();
		void initCompoundingMap();
		void initDBConventionMap();
		void initCalendarMap();		 
		void initFreqMap();
		void initTimeUnitMap();

		json data_;

		const enum Currency { CLP, UF, USD, EUR };
		const enum Compounding { Simple, Compounded };
		const enum DayCounter { Act360, Act365, Thirty360 };
		const enum Frequency { Annual, Semiannual, Monthly , Quarterly};
		const enum Calendar { Chile, NullCalendar, USA };
		const enum TimeUnit { Days, Weeks, Months, Years };
		const enum BDConvention { Following, Unadjusted, ModFollowing };

		std::map<std::string, BDConvention> dbConventionMap_;
		std::map<std::string, TimeUnit> timeUnitMap_;
		std::map<std::string, Calendar> calendarMap_;
		std::map<std::string, Frequency> frequencyMap_;
		std::map<std::string, Compounding> compoundingMap_;
		std::map<std::string, DayCounter> dayCounterMap_;
		std::map<std::string, Currency> currencyMap_;
	};
}