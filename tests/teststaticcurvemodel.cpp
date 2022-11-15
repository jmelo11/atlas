#include "pch.hpp"
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/cashflowindexer.hpp>

using namespace Atlas;

TEST(StaticCurveModel, Models) {
    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2021);
    double notional = 100;
    QuantLib::InterestRate rate(0.05, QuantLib::Actual360(), QuantLib::Simple, QuantLib::Annual);
    Deposit prod(startDate, endDate, notional, rate);

    MarketRequest request;
    CashflowIndexer indexer;
    indexer.visit(prod);
    indexer.setRequest(request);

    // curve
    std::vector<double> times = {0, 30};
    std::vector<double> rates = {0.05, 0.05};
    ZeroRateCurve<QuantLib::Linear> curve(startDate, times, rates);

    CurveMap discounts;
    discounts["undefined"] = std::make_unique<ZeroRateCurve<QuantLib::Linear>>(curve);

    StaticCurveModel model(request, discounts);

    std::vector<QuantLib::Date> evalDates = {startDate};
    Scenario scenario;
    model.simulate(evalDates, scenario);

    EXPECT_EQ(scenario.size(), 1);
    EXPECT_EQ(scenario[0].dfs.size(), 3);
    EXPECT_FLOAT_EQ(scenario[0].dfs[0], 1);
    EXPECT_FLOAT_EQ(scenario[0].dfs[1], 1 / rate.compoundFactor(startDate, endDate));
}