#include "pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <atlas/visitors/parsolver.hpp>

using namespace Atlas;
namespace QL = QuantLib;

TEST(FullProcess, TestProcess) {
    QL::Date startDate(1, QL::Month::Aug, 2020);
    QL::Date endDate(1, QL::Month::Aug, 2021);
    double notional = 100;
    QL::InterestRate rate(0.05, QL::Actual360(), QL::Simple, QL::Annual);
    Deposit prod(startDate, endDate, notional, rate);

    MarketRequest request;
    CashflowIndexer indexer;
    indexer.visit(prod);
    indexer.setRequest(request);

    // curve
    double marketRate = 0.05;
    CurveMap discounts;
    discounts["undefined"] =
        std::make_unique<QL::FlatForward>(startDate, marketRate, QL::Actual360(), QL::Simple, QL::Annual);
    StaticCurveModel model(request, discounts);

    std::vector<QL::Date> evalDates = {startDate};
    Scenario scenario;
    model.simulate(evalDates, scenario);

    NPVCalculator npvCalculator(scenario.at(0));
    prod.accept(npvCalculator);
    EXPECT_FLOAT_EQ(npvCalculator.results(), 100);

    ParSolver parSolver(scenario.at(0));
    prod.accept(parSolver);
    EXPECT_FLOAT_EQ(parSolver.results(), 0.05);
}