// QuantLib
#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/termstructures/yield/flatforward.hpp>

// QLP
#include <qlp/detail/parsingmethods.hpp>

// python module
#include "makeobjectfromjson.hpp"
#include "schemas/depositschema.hpp"
#include "schemas/equalpaymentschema.hpp"
#include "schemas/fixedbulletschema.hpp"

// data
#include <atlas/data/marketdata.hpp>

// models
#include <atlas/models/staticcurvemodel.hpp>

// visitors
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>

// pybind11
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11_json/pybind11_json.hpp>

namespace py  = pybind11;
namespace QLP = QuantLibParser;
using json    = nlohmann::json;

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
    py::class_<Deposit>(m, "Deposit")
        .def("accept", py::overload_cast<Visitor&>(&Deposit::accept))
        .def("accept", py::overload_cast<ConstVisitor&>(&Deposit::accept, py::const_))
        .def("discountCurve", &Deposit::discountCurve)
        .def("leg", &Deposit::leg);

    py::class_<QLP::Schema<Deposit>>(m, "DepositSchema")
        .def(py::init<>())
        .def("isValid", &QLP::Schema<Deposit>::isValid)
        .def("validate", &QLP::Schema<Deposit>::validate)
        .def("addRequired", &QLP::Schema<Deposit>::addRequired)
        .def("removeRequired", &QLP::Schema<Deposit>::removeRequired)
        .def("makeObj", &QLP::Schema<Deposit>::makeObj<>);

    // equalpaymentproduct
    py::class_<EqualPaymentProduct>(m, "EqualPaymentProduct")
        .def("accept", py::overload_cast<Visitor&>(&EqualPaymentProduct::accept))
        .def("accept", py::overload_cast<ConstVisitor&>(&EqualPaymentProduct::accept, py::const_))
        .def("discountCurve", &EqualPaymentProduct::discountCurve)
        .def("leg", &EqualPaymentProduct::leg);

    py::class_<QLP::Schema<EqualPaymentProduct>>(m, "EqualPaymentProductSchema")
        .def(py::init<>())
        .def("isValid", &QLP::Schema<EqualPaymentProduct>::isValid)
        .def("validate", &QLP::Schema<EqualPaymentProduct>::validate)
        .def("addRequired", &QLP::Schema<EqualPaymentProduct>::addRequired)
        .def("removeRequired", &QLP::Schema<EqualPaymentProduct>::removeRequired)
        .def("makeObj", &QLP::Schema<EqualPaymentProduct>::makeObj<>);

    // fixedbulletproduct
    py::class_<FixedBulletProduct>(m, "FixedBulletProduct")
        .def("accept", py::overload_cast<Visitor&>(&FixedBulletProduct::accept))
        .def("accept", py::overload_cast<ConstVisitor&>(&FixedBulletProduct::accept, py::const_))
        .def("discountCurve", &FixedBulletProduct::discountCurve)
        .def("leg", &FixedBulletProduct::leg);

    py::class_<QLP::Schema<FixedBulletProduct>>(m, "FixedBulletProductSchema")
        .def(py::init<>())
        .def("isValid", &QLP::Schema<FixedBulletProduct>::isValid)
        .def("validate", &QLP::Schema<FixedBulletProduct>::validate)
        .def("addRequired", &QLP::Schema<FixedBulletProduct>::addRequired)
        .def("removeRequired", &QLP::Schema<FixedBulletProduct>::removeRequired)
        .def("makeObj", &QLP::Schema<FixedBulletProduct>::makeObj<>);

    // models
    py::class_<StaticCurveModel>(m, "StaticCurveModel")
        .def(py::init<const MarketRequest&>())
        .def("addDiscountCurve", &StaticCurveModel::addDiscountCurve)
        .def("addForecastCurve", &StaticCurveModel::addForecastCurve)
        .def("simulate", py::overload_cast<>(&StaticCurveModel::simulate, py::const_));

    // visitors
    py::class_<CashflowIndexer>(m, "CashflowIndexer")
        .def(py::init<>())
        .def("visit", py::overload_cast<Deposit&>(&CashflowIndexer::visit))
        .def("visit", py::overload_cast<EqualPaymentProduct&>(&CashflowIndexer::visit))
        .def("setRequest", &CashflowIndexer::setRequest);

    py::class_<CashflowProfiler>(m, "CashflowProfiler")
        .def(py::init<>())
        .def("visit", py::overload_cast<const Deposit&>(&CashflowProfiler::visit, py::const_))
        .def("visit", py::overload_cast<const EqualPaymentProduct&>(&CashflowProfiler::visit, py::const_))
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
        .def("clear", &CashflowProfiler::clear);

    py::class_<NPVCalculator>(m, "NPVCalculator")
        .def(py::init<const MarketData&>())
        .def("visit", py::overload_cast<Deposit&>(&NPVCalculator::visit))
        .def("visit", py::overload_cast<EqualPaymentProduct&>(&NPVCalculator::visit))
        .def("results", &NPVCalculator::results)
        .def("clear", &NPVCalculator::clear);

    py::class_<ParSolver>(m, "ParSolver")
        .def(py::init<const MarketData&>())
        .def("visit", py::overload_cast<const Deposit&>(&ParSolver::visit, py::const_))
        .def("visit", py::overload_cast<const EqualPaymentProduct&>(&ParSolver::visit, py::const_))
        .def("results", &ParSolver::results)
        .def("clear", &ParSolver::clear);
}