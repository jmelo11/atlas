#ifndef D3CB0046_6A6F_4723_B9B6_F86E50B68765
#define D3CB0046_6A6F_4723_B9B6_F86E50B68765

// pybind11
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

// fundation
#include <atlas/atlasconfig.hpp>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {
    inline std::string dateToStr(const Date& date) {
        if (date == Date()) return "None";
        std::string d, m, y;
        if (date.dayOfMonth() < 10) {
            d = "0" + std::to_string(date.dayOfMonth());
        } else {
            d = std::to_string(date.dayOfMonth());
        }

        if (date.month() < 10) {
            m = "0" + std::to_string(date.month());
        } else {
            m = std::to_string(date.month());
        }
        y = std::to_string(date.year());
        return y + "-" + m + "-" + d;
    }
}  // namespace Aux

void py_datetime(py::module& m) {
    py::class_<Date>(m, "Date")
        .def(py::init<int, Month, int>())
        .def(py::init<const Date&>())
        .def("serialNumber", &Date::serialNumber)
        .def("weekday", &Date::weekday)
        .def("dayOfMonth", &Date::dayOfMonth)
        .def("month", &Date::month)
        .def("year", &Date::year)
        .def("__str__", [](const Date& d) { return Aux::dateToStr(d); })
        .def("__repr__", [](const Date& d) { return Aux::dateToStr(d); })
        .def(py::self + int())
        .def(py::self - int())
        .def(py::self - py::self)
        .def(py::self += int())
        .def(py::self -= int())
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def(py::self < py::self)
        .def(py::self <= py::self)
        .def(py::self > py::self)
        .def(py::self >= py::self)
        .def("__hash__", [](const Date& d) { return std::hash<Date>()(d); });

    py::class_<Schedule>(m, "Schedule")
        .def(py::init<const Date&, const Date&, const Period&, const Calendar&, BusinessDayConvention, BusinessDayConvention, DateGeneration::Rule,
                      bool>())
        .def("size", &Schedule::size)
        .def("dates", &Schedule::dates);

    py::class_<MakeSchedule>(m, "MakeSchedule")
        .def(py::init<>())
        .def("from", &MakeSchedule::from)
        .def("to", &MakeSchedule::to)
        .def("withTenor", &MakeSchedule::withTenor)
        .def("withCalendar", &MakeSchedule::withCalendar)
        .def("withConvention", &MakeSchedule::withConvention)
        .def("withTerminationDateConvention", &MakeSchedule::withTerminationDateConvention)
        .def("withRule", &MakeSchedule::withRule)
        .def("endOfMonth", &MakeSchedule::endOfMonth)
        .def("forwards", &MakeSchedule::forwards)
        .def("backwards", &MakeSchedule::backwards)
        .def("endOfMonth", &MakeSchedule::endOfMonth);

    py::class_<Period>(m, "Period").def(py::init<int, TimeUnit>());

    // dayCounters
    py::class_<DayCounter>(m, "DayCounter").def("__str__", &DayCounter::name);

    py::class_<Actual360, DayCounter>(m, "Actual360")
        .def(py::init<>())
        .def("dayCount", &Actual360::dayCount)
        .def("yearFraction", &Actual360::yearFraction);

    py::class_<Actual365Fixed, DayCounter>(m, "Actual365")
        .def(py::init<>())

        .def("dayCount", &Actual365Fixed::dayCount)
        .def("yearFraction", &Actual365Fixed::yearFraction);

    py::class_<Thirty360, DayCounter>(m, "Thirty360")
        .def(py::init([]() { return Thirty360(Thirty360::BondBasis); }))
        .def("dayCount", &Thirty360::dayCount)
        .def("yearFraction", &Thirty360::yearFraction);
}

#endif /* D3CB0046_6A6F_4723_B9B6_F86E50B68765 */
