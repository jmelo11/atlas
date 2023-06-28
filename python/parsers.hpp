#ifndef D88FAB7D_1265_4198_A85D_4E4BF9A0B34A
#define D88FAB7D_1265_4198_A85D_4E4BF9A0B34A

#include <atlas/parsers/parsingmethods.hpp>
#include <pybind11/pybind11.h>

void py_parsers(py::module& m) {
    m.def("parseDate", py::overload_cast<const std::string&, const std::string&>(&parseDate), py::arg("date"), py::arg("format") = "%Y-%m-%d");
    m.def("parseCurrency", py::overload_cast<const std::string&>(&parseCurrency));
    m.def("parsePeriod", py::overload_cast<const std::string&>(&parsePeriod));
    m.def("parseDayCounter", py::overload_cast<const std::string&>(&parseDayCounter));
    m.def("parseCalendar", py::overload_cast<const std::string&>(&parseCalendar));
    m.def("parseConvention", py::overload_cast<const std::string&>(&parseBusinessDayConvention));
    m.def("parseFrequency", py::overload_cast<const std::string&>(&parseFrequency));
    m.def("parseCompounding", py::overload_cast<const std::string&>(&parseCompounding));
    m.def("parseTimeUnit", py::overload_cast<const std::string&>(&parseTimeUnit));
}

#endif /* D88FAB7D_1265_4198_A85D_4E4BF9A0B34A */
