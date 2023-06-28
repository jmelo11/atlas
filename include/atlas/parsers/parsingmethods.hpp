#ifndef AABE4AC2_5D19_49DC_96D1_F768D56CC12F
#define AABE4AC2_5D19_49DC_96D1_F768D56CC12F

#include <boost/preprocessor.hpp>
#include <atlas/atlasconfig.hpp>
#include <unordered_map>

namespace Atlas {

    /**
     * @brief Parses a string to a date in ISO format
     * @param date String to parse
     * @return Date
     */
    inline std::string parseDate(const Date& date) {
        std::string day   = date.dayOfMonth() < 10 ? "0" + std::to_string(date.dayOfMonth()) : std::to_string(date.dayOfMonth());
        std::string month = date.month() < 10 ? "0" + std::to_string(date.month()) : std::to_string(date.month());
        return std::to_string(date.year()) + "-" + month + "-" + day;
    };

    /**
     * @brief Parses a string to a date ISO format
     * @param date string representing the date
     * @return Date
     */
    inline Date parseDate(const std::string& date, const std::string& format = "%Y-%m-%d") {
        return DateParser::parseFormatted(date, format);
    };

    /**
     * @brief Parses a string to a currency
     * @details Possible currencies: USD, EUR, GBP, CHF, JPY, CAD, AUD, NZD, SEK, NOK, DKK, HKD, SGD, KRW, MXN, BRL, TRY, RUB, INR, CNY, TWD, THB,
     * AED, SAR, ILS, CLP, CLF, PEN, COP, ARS
     * @param currency string representing the currency
     * @return Currency
     */
    inline Currency parseCurrency(const std::string& currency) {
        if (currency == "USD") {
            return Atlas::USD();
        } else if (currency == "EUR") {
            return Atlas::EUR();
        } else if (currency == "GBP") {
            return Atlas::GBP();
        } else if (currency == "CHF") {
            return Atlas::CHF();
        } else if (currency == "JPY") {
            return Atlas::JPY();
        } else if (currency == "CAD") {
            return Atlas::CAD();
        } else if (currency == "AUD") {
            return Atlas::AUD();
        } else if (currency == "NZD") {
            return Atlas::NZD();
        } else if (currency == "SEK") {
            return Atlas::SEK();
        } else if (currency == "NOK") {
            return Atlas::NOK();
        } else if (currency == "DKK") {
            return Atlas::DKK();
        } else if (currency == "HKD") {
            return Atlas::HKD();
        } else if (currency == "SGD") {
            return Atlas::SGD();
        } else if (currency == "KRW") {
            return Atlas::KRW();
        } else if (currency == "MXN") {
            return Atlas::MXN();
        } else if (currency == "BRL") {
            return Atlas::BRL();
        } else if (currency == "TRY") {
            return Atlas::TRY();
        } else if (currency == "RUB") {
            return Atlas::RUB();
        } else if (currency == "INR") {
            return Atlas::INR();
        } else if (currency == "CNY") {
            return Atlas::CNY();
        } else if (currency == "TWD") {
            return Atlas::TWD();
        } else if (currency == "THB") {
            return Atlas::THB();
        } else if (currency == "AED") {
            return Atlas::AED();
        } else if (currency == "SAR") {
            return Atlas::SAR();
        } else if (currency == "ILS") {
            return Atlas::ILS();
        } else if (currency == "CLP") {
            return Atlas::CLP();
        } else if (currency == "CLF") {
            return Atlas::CLF();
        } else if (currency == "PEN") {
            return Atlas::PEN();
        } else if (currency == "COP") {
            return Atlas::COP();
        } else if (currency == "ARS") {
            return Atlas::ARS();
        } else {
            throw std::runtime_error("Currency not supported");
        }
    };

    /**
     * @brief Parses a currency to a string
     * @details Is not compatible with parseCurrency as it does not return the same string
     * @param currency Currency to parse
     * @return std::string
     */
    inline std::string parseCurrency(const Currency& currency) {
        return currency.name();
    };

    /**
     * @brief Parses a string to a day counter
     * @details Possible day counters: Actual360, Actual365, Thirty360
     * @param dayCounter string representing the day counter
     * @return DayCounter
     */
    inline DayCounter parseDayCounter(const std::string& dayCounter) {
        if (dayCounter == "Actual360") {
            return Actual360();
        } else if (dayCounter == "Actual365") {
            return Actual365Fixed();
        } else if (dayCounter == "Thirty360") {
            return Thirty360(Thirty360::BondBasis);
        } else {
            throw std::runtime_error("DayCounter not supported");
        }
    };

    /**
     * @brief Parses a day counter to a string
     * @details Is not compaitble with the parseDayCounter function that takes a string as input
     * @param dayCounter DayCounter to parse
     * @return std::string
     */
    inline std::string parseDayCounter(DayCounter& dayCounter) {
        return dayCounter.name();
    };

    /**
     * @brief Parses a string to a compounding
     * @details Possible compoundings: Simple, Compounded, Continuous
     * @param compounding string representing the compounding
     * @return Compounding
     */
    inline Compounding parseCompounding(const std::string& compounding) {
        if (compounding == "Simple") {
            return Compounding::Simple;
        } else if (compounding == "Compounded") {
            return Compounding::Compounded;
        } else if (compounding == "Continuous") {
            return Compounding::Continuous;
        } else {
            throw std::runtime_error("Compounding not supported");
        }
    };

    /**
     * @brief Parses a compounding to a string
     * @details Is compatible with the parseCompounding function that takes a string as input
     * @param compounding Compounding to parse
     * @return std::string
     */
    inline std::string parseCompounding(Compounding& compounding) {
        switch (compounding) {
            case Compounding::Simple:
                return "Simple";
            case Compounding::Compounded:
                return "Compounded";
            case Compounding::Continuous:
                return "Continuous";
            default:
                throw std::runtime_error("Unknown compounding");
        }
    };

    /**
     * @brief Parses a string to a frequency
     * @details Possible frequencies: NoFrequency, Once, Annual, Semiannual, EveryFourthMonth, Quarterly, Bimonthly, Monthly, EveryFourthWeek,
     * Biweekly, Weekly, Daily, OtherFrequency
     * @param frequency string representation of the frequency
     * @return Frequency
     */
    inline Frequency parseFrequency(const std::string& frequency) {
        if (frequency == "NoFrequency") {
            return Frequency::NoFrequency;
        } else if (frequency == "Once") {
            return Frequency::Once;
        } else if (frequency == "Annual") {
            return Frequency::Annual;
        } else if (frequency == "Semiannual") {
            return Frequency::Semiannual;
        } else if (frequency == "EveryFourthMonth") {
            return Frequency::EveryFourthMonth;
        } else if (frequency == "Quarterly") {
            return Frequency::Quarterly;
        } else if (frequency == "Bimonthly") {
            return Frequency::Bimonthly;
        } else if (frequency == "Monthly") {
            return Frequency::Monthly;
        } else if (frequency == "EveryFourthWeek") {
            return Frequency::EveryFourthWeek;
        } else if (frequency == "Biweekly") {
            return Frequency::Biweekly;
        } else if (frequency == "Weekly") {
            return Frequency::Weekly;
        } else if (frequency == "Daily") {
            return Frequency::Daily;
        } else if (frequency == "OtherFrequency") {
            return Frequency::OtherFrequency;
        } else {
            throw std::runtime_error("Frequency not supported");
        }
    }

    /**
     * @brief Parses a frequency to a string
     * @details Is compatible with the parseFrequency function that takes a string as input
     * @param frequency Frequency to be parsed
     * @return std::string
     */
    inline std::string parseFrequency(Frequency& frequency) {
        switch (frequency) {
            case Frequency::NoFrequency:
                return "NoFrequency";
            case Frequency::Once:
                return "Once";
            case Frequency::Annual:
                return "Annual";
            case Frequency::Semiannual:
                return "Semiannual";
            case Frequency::EveryFourthMonth:
                return "EveryFourthMonth";
            case Frequency::Quarterly:
                return "Quarterly";
            case Frequency::Bimonthly:
                return "Bimonthly";
            case Frequency::Monthly:
                return "Monthly";
            case Frequency::EveryFourthWeek:
                return "EveryFourthWeek";
            case Frequency::Biweekly:
                return "Biweekly";
            case Frequency::Weekly:
                return "Weekly";
            case Frequency::Daily:
                return "Daily";
            case Frequency::OtherFrequency:
                return "OtherFrequency";
            default:
                throw std::runtime_error("Unknown frequency");
        }
    };

    /**
     * @brief Parses a string to a period
     *
     * @param period string representation of a period
     * @return Period
     */
    inline Period parsePeriod(const std::string& period) {
        return PeriodParser::parse(period);
    }

    /**
     * @brief Parses a period to a string
     *
     * @param period Period object
     * @return std::string
     */
    inline std::string parsePeriod(const Period& period) {
        return std::to_string(period.length()) + std::to_string(period.units());
    }

    /**
     * @brief Parses a string to a time unit
     * @details Possible time units: Days, Weeks, Months, Years, Hours, Minutes, Seconds, Milliseconds, Microseconds
     * @param timeUnit string representation of a time unit
     * @return TimeUnit
     */
    inline TimeUnit parseTimeUnit(const std::string& timeUnit) {
        if (timeUnit == "Days") {
            return TimeUnit::Days;
        } else if (timeUnit == "Weeks") {
            return TimeUnit::Weeks;
        } else if (timeUnit == "Months") {
            return TimeUnit::Months;
        } else if (timeUnit == "Years") {
            return TimeUnit::Years;
        } else if (timeUnit == "Hours") {
            return TimeUnit::Hours;
        } else if (timeUnit == "Minutes") {
            return TimeUnit::Minutes;
        } else if (timeUnit == "Seconds") {
            return TimeUnit::Seconds;
        } else if (timeUnit == "Milliseconds") {
            return TimeUnit::Milliseconds;
        } else if (timeUnit == "Microseconds") {
            return TimeUnit::Microseconds;
        } else {
            throw std::runtime_error("Time unit not supported");
        }
    };

    /**
     * @brief Parses a string to a calendar
     * @details Possible calendars: NullCalendar, UnitedStates, UnitedKingdom, TARGET, Chile, Japan, Canada, Australia, NewZealand, Sweden, Norway,
     * Denmark, HongKong, Singapore, SouthKorea, Mexico, Brazil, Turkey, Russia, India, China, Taiwan, Thailand
     * @param calendar string representation of the calendar
     * @return Calendar
     */
    inline Calendar parseCalendar(const std::string& calendar) {
        if (calendar == "NullCalendar") {
            return Calendar();
        } else if (calendar == "UnitedStates") {
            return UnitedStates(UnitedStates::NYSE);
        } else if (calendar == "UnitedKingdom") {
            return UnitedKingdom();
        } else if (calendar == "TARGET") {
            return TARGET();
        } else if (calendar == "Chile") {
            return Chile();
        } else if (calendar == "Japan") {
            return Japan();
        } else if (calendar == "Canada") {
            return Canada();
        } else if (calendar == "Australia") {
            return Australia();
        } else if (calendar == "NewZealand") {
            return NewZealand();
        } else if (calendar == "Sweden") {
            return Sweden();
        } else if (calendar == "Norway") {
            return Norway();
        } else if (calendar == "Denmark") {
            return Denmark();
        } else if (calendar == "HongKong") {
            return HongKong();
        } else if (calendar == "Singapore") {
            return Singapore();
        } else if (calendar == "SouthKorea") {
            return SouthKorea();
        } else if (calendar == "Mexico") {
            return Mexico();
        } else if (calendar == "Brazil") {
            return Brazil();
        } else if (calendar == "Turkey") {
            return Turkey();
        } else if (calendar == "Russia") {
            return Russia();
        } else if (calendar == "India") {
            return India();
        } else if (calendar == "China") {
            return China();
        } else if (calendar == "Taiwan") {
            return Taiwan();
        } else if (calendar == "Thailand") {
            return Thailand();
        } else {
            throw std::runtime_error("Calendar not supported");
        }
    }

    /**
     * @brief Parses a calendar to a string
     * @details Its not compatible with the parseCalendar function that takes a string as input
     * @param calendar
     * @return std::string
     */
    inline std::string parseCalendar(Calendar& calendar) {
        return calendar.name();
    }

    /**
     * @brief Parses a string to a business day convention
     * @details Possible business day conventions: Following, ModifiedFollowing, Preceding, ModifiedPreceding, Unadjusted, HalfMonthModifiedFollowing,
     * @param convention string representation of the business day convention
     * @return BusinessDayConvention
     */
    inline BusinessDayConvention parseBusinessDayConvention(const std::string& convention) {
        if (convention == "Following") {
            return BusinessDayConvention::Following;
        } else if (convention == "ModifiedFollowing") {
            return BusinessDayConvention::ModifiedFollowing;
        } else if (convention == "Preceding") {
            return BusinessDayConvention::Preceding;
        } else if (convention == "ModifiedPreceding") {
            return BusinessDayConvention::ModifiedPreceding;
        } else if (convention == "Unadjusted") {
            return BusinessDayConvention::Unadjusted;
        } else if (convention == "HalfMonthModifiedFollowing") {
            return BusinessDayConvention::HalfMonthModifiedFollowing;
        } else {
            throw std::runtime_error("Business day convention not supported");
        }
    };
}  // namespace Atlas

#endif /* AABE4AC2_5D19_49DC_96D1_F768D56CC12F */
