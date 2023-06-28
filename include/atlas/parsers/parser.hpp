#ifndef D8AE8195_9EFD_4318_8A84_3FD4F31637D7
#define D8AE8195_9EFD_4318_8A84_3FD4F31637D7

#include <atlas/parsers/parsingmethods.hpp>

namespace Atlas {

    /**
     * @brief Templated function to parse a string into a specific type
     * 
     * @tparam T The type to parse the string into
     * @param value The string to parse 
     * @return T The parsed value 
     */
    template <typename T>
    inline T parse(const std::string& value);

    template <>
    inline Date parse<Date>(const std::string& value) {
        return parseDate(value);
    }

    template <>
    inline Currency parse<Currency>(const std::string& value) {
        return parseCurrency(value);
    }

    template <>
    inline Period parse<Period>(const std::string& value) {
        return parsePeriod(value);
    }

    template <>
    inline DayCounter parse<DayCounter>(const std::string& value) {
        return parseDayCounter(value);
    }

    template <>
    inline Calendar parse<Calendar>(const std::string& value) {
        return parseCalendar(value);
    }

    template <>
    inline BusinessDayConvention parse<BusinessDayConvention>(const std::string& value) {
        return parseBusinessDayConvention(value);
    }

    template <>
    inline Frequency parse<Frequency>(const std::string& value) {
        return parseFrequency(value);
    }

    template <>
    inline Compounding parse<Compounding>(const std::string& value) {
        return parseCompounding(value);
    }

    template <>
    inline TimeUnit parse<TimeUnit>(const std::string& value) {
        return parseTimeUnit(value);
    }
}  // namespace Atlas

#endif /* D8AE8195_9EFD_4318_8A84_3FD4F31637D7 */
