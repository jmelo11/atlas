#define ANKERL_NANOBENCH_IMPLEMENT

#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <nanobench.h>
#include <atlas/aad/aad.hpp>

using namespace Atlas;

void benchmarkTest() {

    double yield = 0.03;
    QuantLib::InterestRate(yield, QuantLib::Actual360(), QuantLib::Compounded, QuantLib::Annual);


    QuantLib::Date startDate(1, QuantLib::Month::Aug, 2020);
    QuantLib::Date endDate(1, QuantLib::Month::Aug, 2025);

    double notional = 100;
    double spread   = 0.0;

    LIBOR12M index;

    std::vector<FloatingRateBulletProduct> prods;
    size_t numProds = 1000;
    for (size_t i = 0; i < numProds; ++i) {
        prods.push_back(FloatingRateBulletProduct(startDate, endDate, notional, spread, index));
    }

    MarketRequest request;
    CashflowIndexer indexer;

    ankerl::nanobench::Bench().run("Indexing", [&]() {
        for (auto& prod : prods) { indexer.visit(prod); }
    });

    indexer.setRequest(request);

    // curve
    double marketRate = 0.03;
    QuantLib::FlatForward curve(startDate, marketRate, QuantLib::Actual360());

    CurveMap discounts;
    discounts["undefined"] = std::make_unique<QuantLib::FlatForward>(curve);
    CurveMap forwards;
    forwards["undefined"] = std::make_unique<QuantLib::FlatForward>(curve);

    StaticCurveModel model(request, discounts, forwards);

    std::vector<QuantLib::Date> evalDates = {startDate};
    Scenario scenario;

    ankerl::nanobench::Bench().run("Simulation", [&]() { model.simulate(evalDates, scenario); });

    MarketData marketData = scenario.at(0);
    NPVCalculator npvCalc(marketData);
    for (auto& prod : prods) { npvCalc.visit(prod); }

    ankerl::nanobench::Bench().run("NPV Calculation", [&]() {});
    double npv = npvCalc.results();
}

int main() {
    benchmarkTest();
}