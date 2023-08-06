#ifndef C9B676D2_7070_4A54_B98A_9F20D364C8C7
#define C9B676D2_7070_4A54_B98A_9F20D364C8C7

#include "config.hpp"
#include <atlas/atlasconfig.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

void py_enums(py::module& m) {
    // enums
    py::enum_<Compounding>(m, "Compounding")
        .value("Simple", Compounding::Simple)
        .value("Compounded", Compounding::Compounded)
        .value("Continuous", Compounding::Continuous)
        .value("SimpleThenCompounded", Compounding::SimpleThenCompounded)
        .value("CompoundedThenSimple", Compounding::CompoundedThenSimple)
        .export_values();

    py::enum_<Frequency>(m, "Frequency")
        .value("NoFrequency", Frequency::NoFrequency)
        .value("Once", Frequency::Once)
        .value("Annual", Frequency::Annual)
        .value("Semiannual", Frequency::Semiannual)
        .value("EveryFourthMonth", Frequency::EveryFourthMonth)
        .value("Quarterly", Frequency::Quarterly)
        .value("Bimonthly", Frequency::Bimonthly)
        .value("Monthly", Frequency::Monthly)
        .value("EveryFourthWeek", Frequency::EveryFourthWeek)
        .value("Biweekly", Frequency::Biweekly)
        .value("Weekly", Frequency::Weekly)
        .value("Daily", Frequency::Daily)
        .value("OtherFrequency", Frequency::OtherFrequency)
        .export_values();

    py::enum_<Month>(m, "Month")
        .value("January", Month::January)
        .value("February", Month::February)
        .value("March", Month::March)
        .value("April", Month::April)
        .value("May", Month::May)
        .value("June", Month::June)
        .value("July", Month::July)
        .value("August", Month::August)
        .value("September", Month::September)
        .value("October", Month::October)
        .value("November", Month::November)
        .value("December", Month::December)
        .export_values();

    py::enum_<DateGeneration::Rule>(m, "DateGenerationRule")
        .value("Backward", DateGeneration::Rule::Backward)
        .value("Forward", DateGeneration::Rule::Forward)
        .value("Zero", DateGeneration::Rule::Zero)
        .value("ThirdWednesday", DateGeneration::Rule::ThirdWednesday)
        .value("Twentieth", DateGeneration::Rule::Twentieth)
        .value("TwentiethIMM", DateGeneration::Rule::TwentiethIMM)
        .value("OldCDS", DateGeneration::Rule::OldCDS)
        .value("CDS", DateGeneration::Rule::CDS)
        .value("CDS2015", DateGeneration::Rule::CDS2015)
        .export_values();

    py::enum_<BusinessDayConvention>(m, "BusinessDayConvention")
        .value("Following", BusinessDayConvention::Following)
        .value("ModifiedFollowing", BusinessDayConvention::ModifiedFollowing)
        .value("Preceding", BusinessDayConvention::Preceding)
        .value("ModifiedPreceding", BusinessDayConvention::ModifiedPreceding)
        .value("Unadjusted", BusinessDayConvention::Unadjusted)
        .value("HalfMonthModifiedFollowing", BusinessDayConvention::HalfMonthModifiedFollowing)
        .value("Nearest", BusinessDayConvention::Nearest)
        .export_values();

    py::enum_<TimeUnit>(m, "TimeUnit")
        .value("Days", TimeUnit::Days)
        .value("Weeks", TimeUnit::Weeks)
        .value("Months", TimeUnit::Months)
        .value("Years", TimeUnit::Years)
        .export_values();

    py::enum_<Side>(m, "Side").value("Recieve", Side::Recieve).value("Pay", Side::Pay).export_values();
}

#endif /* C9B676D2_7070_4A54_B98A_9F20D364C8C7 */
