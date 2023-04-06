
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/multithreading/threadpool.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <cppad/cppad.hpp>
#include <iostream>
#include <numeric>

using namespace Atlas;

using NumType = double;

void pricingFixedRateInstruments() {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    NumType r                  = 0.03;
    InterestRate<NumType> rate(r, Actual360(), Compounding::Compounded, Frequency::Annual);
    FixedRateBulletInstrument<NumType> instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curveStrategy, index);

    // set curve context
    instrument.discountCurveContex(store_.at("TEST"));

    Indexer<NumType> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<NumType> model(request);

    MarketData<NumType> marketData = model.simulate(startDate);

    NPVCalculator<NumType> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

void pricingFloatingRateInstruments() {
    // Create a fixed rate instrument
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.03;

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    const auto& context       = store_.at("TEST");

    FloatingRateBulletInstrument<double> instrument(startDate, endDate, notional, spread, context);
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<double> model(request);

    MarketData<double> marketData = model.simulate(startDate);

    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

void rateSens() {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2025);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    dual r                     = 0.03;

    CurveContextStore& store_ = CurveContextStore::instance();

    auto f = [&](const dual& r) {
        InterestRate<dual> rate = InterestRate(r, Actual360(), Compounding::Simple, Frequency::Annual);
        FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate);
        // set curve context
        instrument.discountCurveContex(store_.at("TEST"));
        Indexer<dual> indexer;
        indexer.visit(instrument);
        MarketRequest request;
        indexer.setRequest(request);
        StaticCurveModel<dual> model(request);
        MarketData<dual> marketData = model.simulate(instrument.startDate());

        instrument.rate(r);
        NPVCalculator<dual> npvCalculator(marketData);
        npvCalculator.visit(instrument);
        return npvCalculator.results();
    };

    dual sens = derivative(f, wrt(r), at(r)) * 0.01;
    std::cout << "Sens: " << sens << std::endl;
}

void multithreadedSens() {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2025);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    dual r                     = 0.03;
    InterestRate<dual> rate(r, Actual360(), Compounding::Simple, Frequency::Annual);
    CurveContextStore& store_ = CurveContextStore::instance();
    auto& context             = store_.at("TEST");
    std::vector<FixedRateBulletInstrument<dual>> portfolio;
    size_t numOps = 1000000;

    // create portfolio and index portfolio
    Indexer<dual> indexer;
    for (size_t i = 0; i < numOps; i++) {
        FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate, context);
        indexer.visit(instrument);
        portfolio.push_back(instrument);
    }

    // slice portfolio
    size_t opsPerSlice = 100;
    size_t numSlices   = numOps / opsPerSlice;
    std::vector<std::vector<FixedRateBulletInstrument<dual>>> slices;
    for (size_t i = 0; i < numSlices; i++) {
        std::vector<FixedRateBulletInstrument<dual>> slice;
        for (size_t j = 0; j < opsPerSlice; j++) { slice.push_back(portfolio[i * opsPerSlice + j]); }
        slices.push_back(slice);
    }

    // generate required rates
    MarketRequest request;
    indexer.setRequest(request);
    StaticCurveModel<dual> model(request);
    MarketData<dual> marketData = model.simulate(startDate);

    auto npv = [&](dual r) {
        // set curve context
        ThreadPool* pool = ThreadPool::getInstance();
        pool->start();
        std::vector<TaskHandle> futures;
        dual npv = 0.0;
        for (auto& slice : slices) {
            auto task = [&]() {
                NPVCalculator<dual> npvCalculator(marketData);
                for (auto& instrument : slice) {
                    instrument.rate(r);
                    npvCalculator.visit(instrument);
                }
                npv += npvCalculator.results();
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }

        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
        return npv;
    };

    dual sens = derivative(npv, wrt(r), at(r)) * 0.01;
    std::cout << "Multicore sens: " << sens << std::endl;
}

int main() {
    pricingFixedRateInstruments();
    pricingFloatingRateInstruments();
    rateSens();
    multithreadedSens();
    return 0;
}