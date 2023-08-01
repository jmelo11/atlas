#ifndef FCE577B2_2A29_4C65_B543_FD50C43DB8D6
#define FCE577B2_2A29_4C65_B543_FD50C43DB8D6

#include "config.hpp"
#include <atlas/data/marketdata.hpp>
#include <atlas/data/marketrequest.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

void py_market(py::module& m) {
    // store
    py::class_<MarketStore<NumType>>(m, "MarketStore")
        .def(py::init<const Date&, Currency>(), py::arg("refDate"), py::arg("localCcy"))
        .def("curveManager", py::overload_cast<>(&MarketStore<NumType>::curveManager, py::const_), py::return_value_policy::reference)
        .def("curveManager", py::overload_cast<>(&MarketStore<NumType>::curveManager), py::return_value_policy::reference)
        .def("fxManager", py::overload_cast<>(&MarketStore<NumType>::fxManager, py::const_), py::return_value_policy::reference)
        .def("fxManager", py::overload_cast<>(&MarketStore<NumType>::fxManager), py::return_value_policy::reference)
        .def("refDate", &MarketStore<NumType>::refDate)
        .def("localCurrency", &MarketStore<NumType>::localCurrency);

    // curve manager
    py::class_<YieldTermStructureManager<NumType>>(m, "YieldTermStructureManager")
        .def(py::init<const Date&, const Currency&>(), py::arg("refDate"), py::arg("localCurrency"))
        .def("addCurveContext", &YieldTermStructureManager<NumType>::addCurveContext, py::arg("name"), py::arg("curve"),
             py::arg("index") = InterestRateIndex<NumType>(), py::arg("ccy") = Currency(), py::arg("isRiskFree") = false)
        .def("localCurrency", &YieldTermStructureManager<NumType>::localCurrency)
        .def("refDate", &YieldTermStructureManager<NumType>::refDate)
        .def("summary", &YieldTermStructureManager<NumType>::summary);

    py::class_<CurveContext<NumType>>(m, "CurveContext")
        .def("curve", &CurveContext<NumType>::curve)
        .def("idx", &CurveContext<NumType>::idx)
        .def("index", &CurveContext<NumType>::index)
        .def("name", &CurveContext<NumType>::name);

    // fx manager
    py::class_<ExchangeRateManager<NumType>>(m, "ExchangeRateManager")
        .def(py::init<const Date&, const Currency&>(), py::arg("refDate"), py::arg("localCurrency"))
        .def("addExchangeRate", &ExchangeRateManager<NumType>::addExchangeRate, py::arg("weak"), py::arg("strong"), py::arg("spotRate"))
        .def("localCurrency", &ExchangeRateManager<NumType>::localCurrency)
        .def("refDate", &ExchangeRateManager<NumType>::refDate)
        .def("summary", &ExchangeRateManager<NumType>::summary);

    // market request & market data

    py::class_<MarketData<NumType>>(m, "MarketData")
        .def_readonly("refDate", &MarketData<NumType>::refDate)
        .def_readonly("dfs", &MarketData<NumType>::dfs)
        .def_readonly("fwds", &MarketData<NumType>::fwds)
        .def_readonly("fxs", &MarketData<NumType>::fxs);

    py::class_<MarketRequest>(m, "MarketRequest")
        .def_readonly("dfs", &MarketRequest::dfs)
        .def_readonly("fwd", &MarketRequest::fwds)
        .def_readonly("fxs", &MarketRequest::fxs);

    py::class_<MarketRequest::ForwardRate>(m, "ForwardRateRequest").def("__str__", [](const MarketRequest::ForwardRate& r) {
        std::string start = r.startDate_ == Date() ? "null" : parseDate(r.startDate_);
        std::string end   = r.endDate_ == Date() ? "null" : parseDate(r.endDate_);
        std::string rep = "ForwardRateRequest(startDate=" + start + ", endDate=" + end +
                          ", idx=" + std::to_string(r.curve_) + ")";
        return rep;
    });

    py::class_<MarketRequest::DiscountFactor>(m, "DiscountFactorRequest").def("__str__", [](const MarketRequest::DiscountFactor& r) {
        std::string date = r.date_ == Date() ? "null" : parseDate(r.date_);
        std::string rep = "DiscountFactorRequest(date=" + date + ", idx=" + std::to_string(r.curve_) + ")";
        return rep;
    });

    py::class_<MarketRequest::ExchangeRate>(m, "ExchangeRateRequest").def("__str__", [](const MarketRequest::ExchangeRate& r) {
        std::string date = r.date_ == Date() ? "null" : parseDate(r.date_);
        std::string rep =
            "ExchangeRateRequest(ccy1=" + std::to_string(r.ccy1_) + ", ccy2=" + std::to_string(r.ccy2_) + ", date=" + date + ")";
        return rep;
    });
}

#endif /* FCE577B2_2A29_4C65_B543_FD50C43DB8D6 */
