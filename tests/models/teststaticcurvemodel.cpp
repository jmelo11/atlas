#include "../pch.hpp"
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/cashflowindexer.hpp>

using namespace Atlas;

TEST(StaticCurveModel, FixedRateInstrument) {
    Date startDate(1, Month::Aug, 2020);
    Date endDate(1, Month::Aug, 2021);
    double notional = 100;
    InterestRate rate(0.05, Actual360(), FreSimple, Annual);
    Deposit prod(startDate, endDate, notional, rate);

    MarketRequest request;
    CashflowIndexer indexer;
    indexer.visit(prod);
    indexer.setRequest(request);

    // curve
    CurveMap discounts;
    discounts["undefined"] = std::make_shared<FlatForward>(startDate, rate, Actual360(), Simple, Annual);

    StaticCurveModel model(request, discounts);

    std::vector<Date> evalDates = {startDate};
    Scenario scenario;
    model.simulate(evalDates, scenario);

    EXPECT_EQ(scenario.size(), 1);
    EXPECT_EQ(scenario[0].dfs.size(), 3);
    EXPECT_FLOAT_EQ(scenario[0].dfs[0], 1);
    EXPECT_FLOAT_EQ(scenario[0].dfs[1], 1 / rate.compoundFactor(startDate, endDate));
}

TEST(StaticCurveModel, FloatingRateInstrument) {
    Date startDate(1, Month::Aug, 2020);
    Date endDate(1, Month::Aug, 2025);

    double notional = 100;
    double spread   = 0.0;

    LIBOR12M index;
    FloatingRateBulletProduct prod(startDate, endDate, notional, spread, index);

    MarketRequest request;
    CashflowIndexer indexer;
    indexer.visit(prod);
    indexer.setRequest(request);

    // curve
    double marketRate = 0.05;
    CurveMap discounts;
    discounts["undefined"] = std::make_unique<FlatForward>(startDate, marketRate, Actual360());

    StaticCurveModel model(request, discounts);
    model.addForecastCurve(index.name(), std::make_unique<FlatForward>(startDate, marketRate, Actual360()));

    std::vector<Date> evalDates = {startDate};
    Scenario scenario;
    model.simulate(evalDates, scenario);

    EXPECT_EQ(scenario.size(), 1);
    EXPECT_EQ(scenario[0].dfs.size(), 7);
    EXPECT_EQ(scenario[0].fwds.size(), 5);
}