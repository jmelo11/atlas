
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/multithreading/threadpool.hpp>
#include <atlas/others/auxfuncs.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <cppad/cppad.hpp>
#include <iostream>
#include <numeric>
#include <thread>

using namespace Atlas;

using NumType = double;

void pricingFixedRateInstruments(MarketStore& store_) {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    NumType r                  = 0.03;
    InterestRate<NumType> rate(r, Actual360(), Compounding::Compounded, Frequency::Annual);
    FixedRateBulletInstrument<NumType> instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    instrument.discountCurveContex(store_.curveContext("TEST"));

    Indexer<NumType> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<NumType> model(request, store_);

    MarketData<NumType> marketData = model.simulate(startDate);

    NPVCalculator<NumType> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

void pricingFloatingRateInstruments(MarketStore& store_) {
    // Create a fixed rate instrument
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.03;

    // Create a curve context store

    const auto& context = store_.curveContext("TEST");

    FloatingRateBulletInstrument<double> instrument(startDate, endDate, notional, spread, context);
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<double> model(request, store_);

    MarketData<double> marketData = model.simulate(startDate);

    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

void rateSens(MarketStore& store_) {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2025);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    dual r                     = 0.03;

    auto f = [&](const dual& r) {
        InterestRate<dual> rate = InterestRate(r, Actual360(), Compounding::Simple, Frequency::Annual);
        FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate);
        // set curve context
        instrument.discountCurveContex(store_.curveContext("TEST"));
        Indexer<dual> indexer;
        indexer.visit(instrument);
        MarketRequest request;
        indexer.setRequest(request);
        StaticCurveModel<dual> model(request, store_);
        MarketData<dual> marketData = model.simulate(instrument.startDate());

        instrument.rate(r);
        NPVCalculator<dual> npvCalculator(marketData);
        npvCalculator.visit(instrument);
        return npvCalculator.results();
    };

    dual sens = derivative(f, wrt(r), at(r)) * 0.01;
    std::cout << "Sens: " << sens << std::endl;
}

void multithreadedSens(MarketStore& store_) {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2025);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    dual r                     = 0.03;
    InterestRate<dual> rate(r, Actual360(), Compounding::Simple, Frequency::Annual);

    auto& context = store_.curveContext("TEST");
    size_t numOps = 1000000;
    std::vector<FixedRateBulletInstrument<dual>> portfolio;
    portfolio.reserve(numOps);

    // create portfolio and index portfolio
    Indexer<dual> indexer;
    {
        std::cout << "Creating portfolio" << std::endl;
        Timer t;

        for (size_t i = 0; i < numOps; i++) {
            FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate, context);
            indexer.visit(instrument);
            portfolio.push_back(instrument);
        }
    }

    // slice portfolio
    const size_t num_threads  = std::thread::hardware_concurrency();
    const size_t segment_size = (portfolio.size() + num_threads - 1) / num_threads;
    const size_t num_slices   = (portfolio.size() + segment_size - 1) / segment_size;
    std::vector<std::vector<FixedRateBulletInstrument<dual>>> slices(num_slices);
    {
        std::cout << "Slicing portfolio" << std::endl;
        Timer t;
        for (size_t i = 0; i < num_slices; i++) {
            auto start = portfolio.begin() + i * segment_size;
            auto end   = std::min(start + segment_size, portfolio.end());
            slices[i].assign(start, end);
        }
    }

    // // generate required rates
    MarketRequest request;
    indexer.setRequest(request);
    // StaticCurveModel<dual> model(request, store_);
    // MarketData<dual> marketData;
    // {
    //     std::cout << "Simulating market data" << std::endl;
    //     Timer t;
    // }

    auto npv = [&](dual r) {
        // set curve context
        ThreadPool* pool = ThreadPool::getInstance();
        pool->start();
        std::vector<TaskHandle> futures;
        dual npv = 0.0;
        for (auto& slice : slices) {
            auto task = [&]() {
                StaticCurveModel<dual> model(request, store_);
                MarketData<dual> marketData;
                marketData = model.simulate(startDate);
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

    {
        std::cout << "Calculating portfolio NPV" << std::endl;
        Timer t;
        dual npv_ = npv(r);
    }

    dual sens;
    {
        std::cout << "Calculating portfolio rate sensitivity" << std::endl;
        Timer t;
        sens = derivative(npv, wrt(r), at(r)) * 0.01;
    }
    std::cout << "Multicore sens: " << sens << std::endl;
}

int main() {
    Date evalDate = Date(1, Month::Aug, 2020);
    MarketStore store_;
    FlatForwardStrategy curveStrategy(evalDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curveStrategy, index);

    // pricingFixedRateInstruments(store_);
    // pricingFloatingRateInstruments(store_);
    // rateSens(store_);
    multithreadedSens(store_);
    return 0;
}