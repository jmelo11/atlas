#ifndef FCE577B2_2A29_4C65_B543_FD50C43DB8D6
#define FCE577B2_2A29_4C65_B543_FD50C43DB8D6

#include <atlas/data/marketdata.hpp>
#include <atlas/data/marketrequest.hpp>
#include <atlas/fundation/marketstore.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace Atlas;

void py_market(py::module& m) {
    py::class_<MarketStore<dual>>(m, "MarketStore")
        .def(py::init<const Date&, Currency>(), py::arg("refDate"), py::arg("localCcy"))
        .def("addCurve", &MarketStore<dual>::addCurve, py::arg("name"), py::arg("curve"), py::arg("index"), py::arg("riskFreeCcy") = Currency())
        .def("curveContext", py::overload_cast<const std::string&>(&MarketStore<dual>::curveContext, py::const_))
        .def("curveContext", py::overload_cast<size_t>(&MarketStore<dual>::curveContext, py::const_))
        .def("rateIndexContext", py::overload_cast<const std::string&>(&MarketStore<dual>::rateIndexContext, py::const_))
        .def("rateIndexContext", py::overload_cast<size_t>(&MarketStore<dual>::rateIndexContext, py::const_))
        .def("cloneFromStore", &MarketStore<dual>::cloneFromStore)
        .def("addExchangeRate", &MarketStore<dual>::addExchangeRate)
        .def("exchange", py::overload_cast<const Currency&, const Currency&>(&MarketStore<dual>::exchange, py::const_))
        .def("riskFreeCurveIdx", py::overload_cast<const Currency&>(&MarketStore<dual>::riskFreeCurveIdx, py::const_));

    py::class_<MarketRequest::ForwardRate>(m, "ForwardRateRequest").def("__str__", [](const MarketRequest::ForwardRate& r) {
        std::string rep = "ForwardRateRequest(startDate=" + Aux::dateToStr(r.startDate_) + ", endDate=" + Aux::dateToStr(r.endDate_) +
                          ", idx=" + std::to_string(r.curve_) + ")";
        return rep;
    });

    py::class_<MarketRequest::DiscountFactor>(m, "DiscountFactorRequest").def("__str__", [](const MarketRequest::DiscountFactor& r) {
        std::string rep = "DiscountFactorRequest(date=" + Aux::dateToStr(r.date_) + ", idx=" + std::to_string(r.curve_) + ")";
        return rep;
    });

    py::class_<MarketRequest::ExchangeRate>(m, "ExchangeRateRequest").def("__str__", [](const MarketRequest::ExchangeRate& r) {
        std::string rep =
            "ExchangeRateRequest(ccy1=" + std::to_string(r.ccy1_) + ", ccy2=" + std::to_string(r.ccy2_) + ", date=" + Aux::dateToStr(r.date_) + ")";
        return rep;
    });

    py::class_<MarketData<dual>>(m, "MarketData")
        .def_readonly("refDate", &MarketData<dual>::refDate)
        .def_readonly("dfs", &MarketData<dual>::dfs)
        .def_readonly("fwds", &MarketData<dual>::fwds)
        .def_readonly("fxs", &MarketData<dual>::fxs);

    py::class_<MarketRequest>(m, "MarketRequest")
        .def_readonly("dfs", &MarketRequest::dfs)
        .def_readonly("fwd", &MarketRequest::fwds)
        .def_readonly("fxs", &MarketRequest::fxs);
}

#endif /* FCE577B2_2A29_4C65_B543_FD50C43DB8D6 */
