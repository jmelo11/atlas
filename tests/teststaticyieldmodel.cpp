#include "pch.hpp"
#include <atlas/models/staticyieldmodel.hpp>
#include <atlas/visitors/cashflowindexer.hpp>

using namespace Atlas;
namespace QL = QuantLib;


TEST(StaticYieldModel, Deposit) {
    QL::Date startDate(1, QL::Month::Aug, 2020);
    QL::Date endDate(1, QL::Month::Aug, 2021);
    double notional = 100;
    QL::InterestRate rate(0.05, QL::Actual360(), QL::Simple, QL::Annual);
    Deposit prod(startDate, endDate, notional, rate);

    MarketRequest request;
    CashflowIndexer indexer;
    indexer.visit(prod);
    indexer.setRequest(request);

    StaticYieldModel model(request, rate);

    std::vector<QL::Date> evalDates = {startDate};
    Scenario scenario;
    model.simulate(evalDates, scenario);

    EXPECT_EQ(scenario.size(), 1);
    EXPECT_EQ(scenario[0].dfs.size(), 3);
    EXPECT_FLOAT_EQ(scenario[0].dfs[0], 1);
    EXPECT_FLOAT_EQ(scenario[0].dfs[1], rate.discountFactor(startDate, endDate));
}