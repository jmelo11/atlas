#include <atlas/parsers/parsingmethods.hpp>
#include <gtest/gtest.h>

using namespace Atlas;

TEST(ParseDate, ValidDateYYYYMMDD) {
    // Test with a valid date in YYYY-MM-DD format
    std::string dateString = "2023-06-28";
    Atlas::Date parsedDate = Atlas::parseDate(dateString);
    EXPECT_EQ(parsedDate.year(), 2023);
    EXPECT_EQ(parsedDate.month(), 6);
    EXPECT_EQ(parsedDate.dayOfMonth(), 28);
}

TEST(ParseDate, ValidDateYYYYMMDDTHHMMSS) {
    // Test with a valid date in YYYY-MM-DDTHH:MM:SS format
    std::string dateString = "2023-06-28T12:34:56";
    Atlas::Date parsedDate = Atlas::parseDate(dateString);
    EXPECT_EQ(parsedDate.year(), 2023);
    EXPECT_EQ(parsedDate.month(), 6);
    EXPECT_EQ(parsedDate.dayOfMonth(), 28);
    // You can also check the time components if needed
}

TEST(ParseDate, InvalidDate) {
    // Test with an invalid date
    std::string invalidDateString = "2023-13-45";  // Invalid month and day
    EXPECT_THROW(Atlas::parseDate(invalidDateString), std::exception);
}

TEST(ParseDate, PartialDate) {
    // Test with an invalid date format
    std::string invalidDateString = "2023-06";  // Incomplete date format
    Date d                        = Atlas::parseDate(invalidDateString);
    EXPECT_EQ(d.year(), 2023);
    EXPECT_EQ(d.month(), 6);
    EXPECT_EQ(d.dayOfMonth(), 1);
    std::cout << d << std::endl;
}
TEST(ParseDate, InvalidFormat) {
    std::string invalidDateString = "06-28-2023";
    std::string format            = "%Y-%m-%dT%H:%M:%S";
    EXPECT_THROW(Atlas::parseDate(invalidDateString), std::exception);
}

TEST(ParseCurrencyTest, ValidCurrencies) {
    // Test valid currency inputs
    EXPECT_EQ(parseCurrency("USD"), Atlas::USD());
    EXPECT_EQ(parseCurrency("EUR"), Atlas::EUR());
    EXPECT_EQ(parseCurrency("GBP"), Atlas::GBP());
    // Add more test cases for other valid currencies
}

TEST(ParseCurrencyTest, UnknownCurrency) {
    // Test case for unknown currency
    std::string unknownCurrency = "XYZ";
    EXPECT_THROW(parseCurrency(unknownCurrency), std::runtime_error);
}

TEST(ParseDayCounterTest, ValidDayCounters) {
    // Test valid day counter inputs
    EXPECT_EQ(parseDayCounter("Actual360"), Actual360());
    EXPECT_EQ(parseDayCounter("Actual365"), Actual365Fixed());
    EXPECT_EQ(parseDayCounter("Thirty360"), Thirty360(Thirty360::BondBasis));
    // Add more test cases for other valid day counters
}

TEST(ParseDayCounterTest, UnsupportedDayCounter) {
    // Test case for unsupported day counter
    std::string unsupportedDayCounter = "InvalidDayCounter";
    EXPECT_THROW(parseDayCounter(unsupportedDayCounter), std::runtime_error);
}

TEST(ParseCompoundingTest, ValidCompoundings) {
    // Test valid compounding inputs
    EXPECT_EQ(parseCompounding("Simple"), Compounding::Simple);
    EXPECT_EQ(parseCompounding("Compounded"), Compounding::Compounded);
    EXPECT_EQ(parseCompounding("Continuous"), Compounding::Continuous);
    // Add more test cases for other valid compoundings
}

TEST(ParseCompoundingTest, UnsupportedCompounding) {
    // Test case for unsupported compounding
    std::string unsupportedCompounding = "InvalidCompounding";
    EXPECT_THROW(parseCompounding(unsupportedCompounding), std::runtime_error);
}

TEST(ParseFrequencyTest, ValidFrequencies) {
    // Test valid frequency inputs
    EXPECT_EQ(parseFrequency("NoFrequency"), Frequency::NoFrequency);
    EXPECT_EQ(parseFrequency("Once"), Frequency::Once);
    EXPECT_EQ(parseFrequency("Annual"), Frequency::Annual);
    // Add more test cases for other valid frequencies
}

TEST(ParseFrequencyTest, UnsupportedFrequency) {
    // Test case for unsupported frequency
    std::string unsupportedFrequency = "InvalidFrequency";
    EXPECT_THROW(parseFrequency(unsupportedFrequency), std::runtime_error);
}

TEST(ParseTimeUnitTest, ValidTimeUnits) {
    // Test valid time unit inputs
    EXPECT_EQ(parseTimeUnit("Days"), TimeUnit::Days);
    EXPECT_EQ(parseTimeUnit("Weeks"), TimeUnit::Weeks);
    EXPECT_EQ(parseTimeUnit("Months"), TimeUnit::Months);
    // Add more test cases for other valid time units
}

TEST(ParseTimeUnitTest, UnsupportedTimeUnit) {
    // Test case for unsupported time unit
    std::string unsupportedTimeUnit = "InvalidTimeUnit";
    EXPECT_THROW(parseTimeUnit(unsupportedTimeUnit), std::runtime_error);
}

TEST(ParseCalendarTest, ValidCalendars) {
    // Test valid calendar inputs
    EXPECT_EQ(parseCalendar("NullCalendar"), Calendar());
    EXPECT_EQ(parseCalendar("UnitedStates"), UnitedStates(UnitedStates::NYSE));
    EXPECT_EQ(parseCalendar("UnitedKingdom"), UnitedKingdom());
    // Add more test cases for other valid calendars
}

TEST(ParseCalendarTest, UnsupportedCalendar) {
    // Test case for unsupported calendar
    std::string unsupportedCalendar = "InvalidCalendar";
    EXPECT_THROW(parseCalendar(unsupportedCalendar), std::runtime_error);
}

TEST(ParseBusinessDayConventionTest, ValidConventions) {
    // Test valid business day convention inputs
    EXPECT_EQ(parseBusinessDayConvention("Following"), BusinessDayConvention::Following);
    EXPECT_EQ(parseBusinessDayConvention("ModifiedFollowing"), BusinessDayConvention::ModifiedFollowing);
    EXPECT_EQ(parseBusinessDayConvention("Preceding"), BusinessDayConvention::Preceding);
    // Add more test cases for other valid conventions
}

TEST(ParseBusinessDayConventionTest, UnsupportedConvention) {
    // Test case for unsupported business day convention
    std::string unsupportedConvention = "InvalidConvention";
    EXPECT_THROW(parseBusinessDayConvention(unsupportedConvention), std::runtime_error);
}