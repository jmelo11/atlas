// QuantLib
#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/termstructures/yield/flatforward.hpp>

// QLP
#include <qlp/detail/parsingmethods.hpp>

// python module
#include "makeobjectfromjson.hpp"
#include "schemas/customfixedrateschema.hpp"
#include "schemas/customfloatingrateschema.hpp"
#include "schemas/depositschema.hpp"
#include "schemas/equalpaymentschema.hpp"
#include "schemas/fixedratebulletschema.hpp"
#include "schemas/floatingratebulletschema.hpp"
#include "schemas/rateindexschema.hpp"
// data
#include <atlas/data/marketdata.hpp>

// models
#include <atlas/models/staticcurvemodel.hpp>

// visitors
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>
#include <atlas/visitors/durationcalculator.hpp>

// pybind11
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>

namespace py  = pybind11;
namespace QLP = QuantLibParser;
using json    = nlohmann::json;

#define Product(name)                                                               \
    py::class_<name>(m, #name)                                                      \
        .def("accept", py::overload_cast<Visitor&>(&name::accept))                  \
        .def("accept", py::overload_cast<ConstVisitor&>(&name::accept, py::const_)) \
        .def("discountCurve", &name::discountCurve)                                 \
        .def("leg", &name::leg)

#define FixedProduct(name) Product(name).def("rate", py::overload_cast<double>(&name::rate))

#define FloatingProduct(name) Product(name).def("spread", py::overload_cast<double>(&name::spread)).def("forecastCurve", &name::forecastCurve)

#define VisitProducts(name)                                                     \
    def("visit", py::overload_cast<FloatingRateBulletProduct&>(&name::visit))   \
        .def("visit", py::overload_cast<EqualPaymentProduct&>(&name::visit))    \
        .def("visit", py::overload_cast<FixedRateBulletProduct&>(&name::visit)) \
        .def("visit", py::overload_cast<Deposit&>(&name::visit))                \
        .def("visit", py::overload_cast<CustomFixedRateProduct&>(&name::visit)) \
        .def("visit", py::overload_cast<CustomFloatingRateProduct&>(&name::visit))

#define ConstVisitProducts(name)                                                                  \
    def("visit", py::overload_cast<const FloatingRateBulletProduct&>(&name::visit, py::const_))   \
        .def("visit", py::overload_cast<const EqualPaymentProduct&>(&name::visit, py::const_))    \
        .def("visit", py::overload_cast<const FixedRateBulletProduct&>(&name::visit, py::const_)) \
        .def("visit", py::overload_cast<const Deposit&>(&name::visit, py::const_))                \
        .def("visit", py::overload_cast<const CustomFixedRateProduct&>(&name::visit, py::const_)) \
        .def("visit", py::overload_cast<const CustomFloatingRateProduct&>(&name::visit, py::const_))

#define SchemaWithoutMaker(name)                                           \
    py::class_<QLP::Schema<name>>(m, #name "Schema")                       \
        .def(py::init<>())                                                 \
        .def("validate", &QLP::Schema<name>::validate)                     \
        .def("isValid", &QLP::Schema<name>::isValid)                       \
        .def("schema", &QLP::Schema<name>::schema)                         \
        .def("addDefaultValue", &QLP::Schema<name>::addDefaultValue)       \
        .def("removeDefaultValue", &QLP::Schema<name>::removeDefaultValue) \
        .def("schema", &QLP::Schema<name>::schema)                         \
        .def("addRequired", &QLP::Schema<name>::addRequired)               \
        .def("removeRequired", &QLP::Schema<name>::removeRequired)

#define SchemaWithMaker(name) SchemaWithoutMaker(name).def("makeObj", &QLP::Schema<name>::makeObj<>)

PYBIND11_MODULE(Atlas, m) {
    m.doc() = "Atlas";  // optional module docstring

    // curves
    py::class_<QuantLib::YieldTermStructure, std::shared_ptr<QuantLib::YieldTermStructure>>(m, "YieldTermStructure");
    py::class_<QuantLib::DiscountCurve, std::shared_ptr<QuantLib::DiscountCurve>, QuantLib::YieldTermStructure>(m, "DiscountCurve");
    py::class_<QuantLib::FlatForward, std::shared_ptr<QuantLib::FlatForward>, QuantLib::YieldTermStructure>(m, "FlatForwardCurve");

    // market data
    py::class_<MarketRequest>(m, "MarketRequest").def(py::init<>());
    py::class_<MarketData>(m, "MarketData").def(py::init<>()).def_readwrite("dfs", &MarketData::dfs).def_readwrite("fwds", &MarketData::fwds);

    // makers
    m.def("makeFlatForwardCurve", &makeObjectFromJson<QuantLib::FlatForward>);
    m.def("makeDiscountCurve", &makeObjectFromJson<QuantLib::DiscountCurve>);

    // cashflows
    py::class_<Redemption>(m, "Redemption")
        .def("amount", &Redemption::amount)
        .def("paymentDate", [](const Redemption& self) { return QLP::parseDate(self.paymentDate()); })
        .def("dfIdx", py::overload_cast<>(&Redemption::dfIdx, py::const_));

    py::class_<FixedRateCoupon>(m, "FixedRateCoupon")
        .def("amount", &FixedRateCoupon::amount)
        .def("paymentDate", [](const FixedRateCoupon& self) { return QLP::parseDate(self.paymentDate()); })
        .def("dfIdx", py::overload_cast<>(&FixedRateCoupon::dfIdx, py::const_));

    py::class_<FloatingRateCoupon>(m, "FloatingRateCoupon")
        .def("amount", &FloatingRateCoupon::amount)
        .def("paymentDate", [](const FloatingRateCoupon& self) { return QLP::parseDate(self.paymentDate()); })
        .def("dfIdx", py::overload_cast<>(&FloatingRateCoupon::dfIdx, py::const_));

    py::class_<FixedRateLeg>(m, "FixedRateLeg")
        .def(py::init<>())
        .def("coupons", &FixedRateLeg::coupons)
        .def("redemptions", &FixedRateLeg::redemptions);

    py::class_<FloatingRateLeg>(m, "FloatingRateLeg")
        .def(py::init<>())
        .def("coupons", &FloatingRateLeg::coupons)
        .def("redemptions", &FloatingRateLeg::redemptions);

    // instruments
    // deposit
    FixedProduct(Deposit);
    SchemaWithMaker(Deposit);

    // equalpaymentproduct
    FixedProduct(EqualPaymentProduct);
    SchemaWithMaker(EqualPaymentProduct);

    // fixedratebulletproduct
    FixedProduct(FixedRateBulletProduct);
    SchemaWithMaker(FixedRateBulletProduct);

    // floatingbulletproduct
    FloatingProduct(FloatingRateBulletProduct);
    SchemaWithMaker(FloatingRateBulletProduct);

    // customfloatingrateproduct
    FloatingProduct(CustomFloatingRateProduct);
    SchemaWithMaker(CustomFloatingRateProduct);

    // customfixedproduct
    FixedProduct(CustomFixedRateProduct);
    SchemaWithMaker(CustomFixedRateProduct);

    // models
    py::class_<StaticCurveModel>(m, "StaticCurveModel")
        .def(py::init<const MarketRequest&>())
        .def("addDiscountCurve", &StaticCurveModel::addDiscountCurve)
        .def("addForecastCurve", &StaticCurveModel::addForecastCurve)
        .def("simulate", py::overload_cast<>(&StaticCurveModel::simulate, py::const_));

    // visitors
    py::class_<CashflowIndexer>(m, "CashflowIndexer")
        .def(py::init<>())
        .def("setRequest", &CashflowIndexer::setRequest)
        .VisitProducts(CashflowIndexer);

    py::class_<CashflowProfiler>(m, "CashflowProfiler")
        .def(py::init<>())
        .def("interests",
             [](const CashflowProfiler& self) {
                 const auto& cashflows = self.interests();
                 json result           = json::array();
                 for (auto& cashflow : cashflows) {
                     json tmp = {{"DATE", QLP::parseDate(cashflow.first)}, {"VALUE", cashflow.second}};
                     result.push_back(tmp);
                 };
                 return result;
             })
        .def("redemptions",
             [](const CashflowProfiler& self) {
                 const auto& cashflows = self.redemptions();
                 json result           = json::array();
                 for (auto& cashflow : cashflows) {
                     json tmp = {{"DATE", QLP::parseDate(cashflow.first)}, {"VALUE", cashflow.second}};
                     result.push_back(tmp);
                 };
                 return result;
             })
        .def("clear", &CashflowProfiler::clear)
        .ConstVisitProducts(CashflowProfiler);

    py::class_<NPVCalculator>(m, "NPVCalculator")
        .def(py::init<const MarketData&>())
        .def("results", &NPVCalculator::results)
        .def("clear", &NPVCalculator::clear)
        .VisitProducts(NPVCalculator);

    py::class_<ParSolver>(m, "ParSolver")
        .def(py::init<const MarketData&>())
        .def("results", &ParSolver::results)
        .def("clear", &ParSolver::clear)
        .ConstVisitProducts(ParSolver);

    py::class_<DurationCalculator>(m, "YieldSensCalculator")
        .def(py::init([](const std::string& refDate, const MarketData& marketData) {            
            QuantLib::Date refDate_      = QLP::parse<QuantLib::Date>(refDate);
            return DurationCalculator(refDate_, marketData);
        }))
        .def("results", &DurationCalculator::results)
        .ConstVisitProducts(DurationCalculator);
}