// modules
#include "aad.hpp"
#include "couponlegs.hpp"
#include "currencies.hpp"
#include "datetime.hpp"
#include "enums.hpp"
#include "instruments.hpp"
#include "visitors.hpp"

// fundation
#include <atlas/atlasdefine.hpp>
#include <atlas/fundation/context.hpp>
// data
#include <atlas/data/marketdata.hpp>

// models
#include <atlas/models/spotmarketdatamodel.hpp>

// curves
#include <atlas/rates/yieldtermstructure.hpp>
#include <atlas/rates/yieldtermstructure/discountcurve.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/rates/yieldtermstructure/zeroratecurve.hpp>

// othres
#include <atlas/others/interpolations/loglinearinterpolation.hpp>

namespace py = pybind11;
using namespace Atlas;

namespace Aux {

    class PyModel : public Model<dual> {
        using Model<dual>::Model;

        MarketData<dual> marketData(const Date& evalDate = Date()) const override {
            PYBIND11_OVERRIDE_PURE(MarketData<dual>, /* Return type */
                                   Model<dual>,      /* Parent class */
                                   marketData,       /* Name of function in C++ (must match Python name) */
                                   evalDate          /* Argument(s) */
            );
        };
    };

};  // namespace Aux

PYBIND11_MODULE(Atlas, m) {
    m.doc() = "Atlas";  // optional module docstring

    py_aad(m);
    py_enums(m);
    py_datetime(m);
    py_currencies(m);

    // classes
    py::class_<InterestRate<dual>>(m, "InterestRate")
        .def(py::init<>())
        .def(py::init<dual, DayCounter, Compounding, Frequency>(), py::arg("rate"), py::arg("dc") = Actual360(),
             py::arg("comp") = Compounding::Simple, py::arg("freq") = Frequency::Annual)
        .def("rate", &InterestRate<dual>::rate)
        .def("compoundFactor", py::overload_cast<double>(&InterestRate<dual>::compoundFactor, py::const_))
        .def("compoundFactor", py::overload_cast<const Date&, const Date&, const Date&, const Date&>(&InterestRate<dual>::compoundFactor, py::const_))
        .def("discountFactor", py::overload_cast<const Date&, const Date&, const Date&, const Date&>(&InterestRate<dual>::discountFactor, py::const_))
        .def("discountFactor", py::overload_cast<double>(&InterestRate<dual>::discountFactor, py::const_))
        .def("__str__", [](const InterestRate<dual>& r) {
            std::string msg = "InterestRate(" + std::to_string(val(r.rate())) + ", " + r.dayCounter().name() + ", " +
                              std::to_string(r.compounding()) + ", " + std::to_string(r.frequency()) + ")";
            return msg;
        });

    // Context
    py::class_<Context<YieldTermStructure<dual>>>(m, "CurveContext")
        .def("object", &Context<YieldTermStructure<dual>>::object)
        .def("idx", &Context<YieldTermStructure<dual>>::idx);

    py::class_<Context<RateIndex<dual>>>(m, "RateIndexContext")
        .def("object", &Context<RateIndex<dual>>::object)
        .def("idx", &Context<RateIndex<dual>>::idx);

    py::class_<MarketStore<dual>>(m, "MarketStore")
        .def(py::init<const Date&, Currency>(), py::arg("refDate"), py::arg("localCcy"))
        .def("addCurve", &MarketStore<dual>::addCurve)
        .def("curveContext", py::overload_cast<const std::string&>(&MarketStore<dual>::curveContext, py::const_), "Get a curve context by name")
        .def("curveContext", py::overload_cast<size_t>(&MarketStore<dual>::curveContext, py::const_), "Get a curve context by index")
        .def("rateIndexContext", py::overload_cast<const std::string&>(&MarketStore<dual>::rateIndexContext, py::const_),
             "Get a rate index context by name")
        .def("rateIndexContext", py::overload_cast<size_t>(&MarketStore<dual>::rateIndexContext, py::const_), "Get a rate index context by index")
        .def("cloneFromStore", &MarketStore<dual>::cloneFromStore)
        .def("addExchangeRate", &MarketStore<dual>::addExchangeRate)
        .def("exchange", py::overload_cast<const Currency&, const Currency&>(&MarketStore<dual>::exchange, py::const_),
             "Get the exchange rate between two currencies")
        .def("riskFreeCurveIdx", py::overload_cast<const Currency&>(&MarketStore<dual>::riskFreeCurveIdx, py::const_),
             "Set the risk free curve index for a currency");

    py::class_<YieldTermStructureStrategy<dual>, std::shared_ptr<YieldTermStructureStrategy<dual>>>(m, "YieldTermStructureStrategy")
        .def("discount", py::overload_cast<const Date&>(&YieldTermStructureStrategy<dual>::discount, py::const_))
        .def("discount", py::overload_cast<double>(&YieldTermStructureStrategy<dual>::discount, py::const_))
        .def("forwardRate", &YieldTermStructureStrategy<dual>::forwardRate)
        .def("refDate", &YieldTermStructureStrategy<dual>::refDate);

    py::class_<YieldTermStructure<dual>>(m, "YieldTermStructure")
        .def(py::init([](std::shared_ptr<YieldTermStructureStrategy<dual>> strat) { return YieldTermStructure<dual>(strat->clone()); }))
        .def("discount", py::overload_cast<const Date&>(&YieldTermStructure<dual>::discount, py::const_))
        .def("discount", py::overload_cast<double>(&YieldTermStructure<dual>::discount, py::const_))
        .def("forwardRate", &YieldTermStructure<dual>::forwardRate)
        .def("refDate", &YieldTermStructure<dual>::refDate);

    py::class_<DiscountStrategy<dual, LogLinearInterpolator<dual>>, YieldTermStructureStrategy<dual>,
               std::shared_ptr<DiscountStrategy<dual, LogLinearInterpolator<dual>>>>(m, "DiscountLogLinearStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<dual>&, const DayCounter&>(), py::arg("dates"), py::arg("discounts"),
             py::arg("dayCounter") = Actual360())
        .def(py::init<const DiscountStrategy<dual, LogLinearInterpolator<dual>>&>())
        .def("enableExtrapolation", &DiscountStrategy<dual, LogLinearInterpolator<dual>>::enableExtrapolation);

    py::class_<FlatForwardStrategy<dual>, YieldTermStructureStrategy<dual>, std::shared_ptr<FlatForwardStrategy<dual>>>(m, "FlatForwardStrategy")
        .def(py::init<const Date&, dual, const DayCounter&, Compounding, Frequency>(), py::arg("refDate"), py::arg("rate"),
             py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple, py::arg("frequency") = Frequency::Annual)
        .def(py::init<const FlatForwardStrategy<dual>&>());

    py::class_<ZeroRateStrategy<dual, LinearInterpolator<dual>>, YieldTermStructureStrategy<dual>,
               std::shared_ptr<ZeroRateStrategy<dual, LinearInterpolator<dual>>>>(m, "ZeroRateLinearStrategy")
        .def(py::init<const std::vector<Date>&, const std::vector<dual>&, const DayCounter&, Compounding, Frequency>(), py::arg("dates"),
             py::arg("zeroRates"), py::arg("dayCounter") = Actual360(), py::arg("compounding") = Compounding::Simple,
             py::arg("frequency") = Frequency::Annual)
        .def(py::init<const ZeroRateStrategy<dual, LinearInterpolator<dual>>&>())
        .def("enableExtrapolation", &ZeroRateStrategy<dual, LinearInterpolator<dual>>::enableExtrapolation);

    // RateIndex
    py::class_<RateIndex<dual>>(m, "RateIndex")
        .def(py::init<const Date&, Frequency, DayCounter, Frequency, Compounding>())
        .def("addFixing", &RateIndex<dual>::addFixing)
        .def("getFixing", &RateIndex<dual>::getFixing);

    // MarketData & MarketRequest

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

    // indexable
    py::class_<Indexable>(m, "Indexable")
        .def("dfIdx", py::overload_cast<>(&Indexable::dfIdx, py::const_))
        .def("dfIdx", py::overload_cast<size_t>(&Indexable::dfIdx))
        .def("fwdIdx", py::overload_cast<>(&Indexable::fwdIdx, py::const_))
        .def("fwdIdx", py::overload_cast<size_t>(&Indexable::fwdIdx))
        .def("fxIdx", py::overload_cast<>(&Indexable::fxIdx, py::const_))
        .def("fxIdx", py::overload_cast<size_t>(&Indexable::fxIdx));

    py_couponlegs(m);
    py_instruments(m);
    py_visitors(m);

    // Models
    py::class_<Model<dual>, Aux::PyModel>(m, "Model").def("marketData", &Model<dual>::marketData);

    py::class_<SpotMarketDataModel<dual>, Model<dual>>(m, "SpotMarketDataModel")
        .def(py::init<const MarketRequest&, const MarketStore<dual>&>(), py::arg("marketRequest"), py::arg("store"));
}