
#include "currencyconversion.hpp"
#include "pricefixedinstrument.hpp"
#include "pricefloatinginstrument.hpp"
#include "senscalculation.hpp"
#include "test.hpp"
using namespace Atlas;

// void rateSens() {
//     Date startDate             = Date(1, Month::Aug, 2020);
//     Date endDate               = Date(1, Month::Aug, 2025);
//     Frequency paymentFrequency = Frequency::Semiannual;
//     double notional            = 100;
//     dual r                     = 0.03;

//     MarketStore<dual> store_;
//     FlatForwardStrategy<dual> curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
//     YieldTermStructure<dual> curve_(std::make_unique<FlatForwardStrategy<dual>>(curveStrategy));
//     RateIndex index("TEST", Frequency::Annual, Actual360());
//     store_.createCurveContext("TEST", curve_, index);

//     auto f = [&](const dual& r) {
//         InterestRate<dual> rate = InterestRate(r, Actual360(), Compounding::Simple, Frequency::Annual);
//         FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate);
//         // set curve context
//         instrument.discountCurveContext(store_.curveContext("TEST"));
//         Indexer<dual> indexer;
//         indexer.visit(instrument);
//         MarketRequest request;
//         indexer.setRequest(request);
//         StaticCurveModel<dual> model(request, store_);
//         MarketData<dual> marketData = model.simulate(instrument.startDate());

//         instrument.rate(r);
//         NPVCalculator<dual> npvCalculator(marketData);
//         npvCalculator.visit(instrument);
//         return npvCalculator.results();
//     };

//     dual sens = derivative(f, wrt(r), at(r)) * 0.01;
//     std::cout << "Sens: " << sens << std::endl;
// };

// void multithreadedSens() {
//     Date startDate             = Date(1, Month::Aug, 2020);
//     Date endDate               = Date(1, Month::Aug, 2025);
//     Frequency paymentFrequency = Frequency::Semiannual;
//     double notional            = 100;
//     dual r                     = 0.03;
//     InterestRate<dual> rate(r, Actual360(), Compounding::Simple, Frequency::Annual);

//     size_t numOps = 10000;
//     std::vector<FixedRateBulletInstrument<dual>> portfolio;
//     portfolio.reserve(numOps);

//     MarketStore<dual> mainStore_;
//     FlatForwardStrategy<dual> curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
//     YieldTermStructure<dual> curve_(std::make_unique<FlatForwardStrategy<dual>>(curveStrategy));
//     RateIndex index("TEST", Frequency::Annual, Actual360());
//     mainStore_.createCurveContext("TEST", curve_, index);
//     auto& context = mainStore_.curveContext("TEST");

//     // create portfolio and index portfolio
//     Indexer<dual> indexer;
//     {
//         std::cout << "Creating portfolio" << std::endl;
//         Timer t;

//         for (size_t i = 0; i < numOps; i++) {
//             FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate, context);
//             indexer.visit(instrument);
//             portfolio.push_back(instrument);
//         }
//     }

//     // slice portfolio
//     const size_t num_threads  = std::thread::hardware_concurrency();
//     const size_t segment_size = (portfolio.size() + num_threads - 1) / num_threads;
//     const size_t num_slices   = (portfolio.size() + segment_size - 1) / segment_size;
//     std::vector<std::vector<FixedRateBulletInstrument<dual>>> slices(num_slices);
//     {
//         std::cout << "Slicing portfolio" << std::endl;
//         Timer t;
//         for (size_t i = 0; i < num_slices; i++) {
//             auto start = portfolio.begin() + i * segment_size;
//             auto end   = std::min(start + segment_size, portfolio.end());
//             slices[i].assign(start, end);
//         }
//     }

//     // // generate required rates
//     MarketRequest request;
//     indexer.setRequest(request);

//     auto npv = [&](dual r) {
//         // set curve context
//         ThreadPool* pool = ThreadPool::getInstance();
//         pool->start(0);
//         std::vector<TaskHandle> futures;
//         dual npv = 0.0;
//         for (auto& slice : slices) {
//             auto task = [&]() {
//                 MarketStore<dual> store_;
//                 store_.cloneFromStore(mainStore_);

//                 StaticCurveModel<dual> model(request, store_);
//                 MarketData<dual> marketData;
//                 marketData = model.simulate(startDate);
//                 NPVCalculator<dual> npvCalculator(marketData);

//                 for (auto& instrument : slice) {
//                     instrument.rate(r);
//                     npvCalculator.visit(instrument);
//                 }
//                 npv += npvCalculator.results();
//                 return true;
//             };
//             futures.push_back(pool->spawnTask(task));
//         }

//         for (auto& future : futures) { pool->activeWait(future); }
//         pool->stop();
//         return npv;
//     };

//     dual npv_;
//     {
//         std::cout << "Calculating portfolio NPV" << std::endl;
//         Timer t;
//         npv_ = npv(r);
//     }

//     // dual sens;
//     // {
//     //     std::cout << "Calculating portfolio rate sensitivity" << std::endl;
//     //     Timer t;
//     //     sens = derivative(npv, wrt(r), at(r)) * 0.01;
//     // }
//     // std::cout << "Multicore sens: " << sens << std::endl;
//     // std::cout << "NPV: " << npv_ << std::endl;
// };

// void curveSens() {
//     Date evalDate = Date(1, Month::Aug, 2020);
//     std::vector<dual> rates;
//     std::vector<Date> dates;
//     for (size_t i = 0; i < 20; i++) {
//         rates.push_back(0.03);
//         dates.push_back(evalDate + Period(i * 6, TimeUnit::Months));
//     }

//     Date startDate             = Date(1, Month::Aug, 2020);
//     Date endDate               = Date(1, Month::Aug, 2025);
//     Frequency paymentFrequency = Frequency::Semiannual;
//     double notional            = 100;
//     dual couponR               = 0.03;
//     InterestRate<dual> coupon(couponR, Thirty360(Thirty360::BondBasis), Compounding::Simple, Frequency::Annual);

//     auto& context = mainStore.curveContext("TEST");
//     FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, coupon, context);

//     auto f = [&](const std::vector<dual>& rates) {
//         MarketStore<dual> mainStore;
//         YieldTermStructure<dual> curve(std::make_unique<ZeroRateStrategy<dual, LogLinearInterpolator<dual>>>(dates, rates, Actual360()));
//         RateIndex index("TEST", Frequency::Annual, Actual360());
//         mainStore.createCurveContext("TEST", curve, index);

//         instrument.discountCurveContext(context);

//         Indexer<dual> indexer;
//         indexer.visit(instrument);
//         MarketRequest request = indexer.request();
//         StaticCurveModel<dual> model(request, mainStore);
//         MarketData<dual> marketData = model.simulate(startDate);

//         NPVCalculator<dual> npvCalculator(marketData);
//         npvCalculator.visit(instrument);
//         return npvCalculator.results();
//     };
//     Currency clp = CLP();
//     dual npv;
//     auto sens = gradient(f, wrt(rates), at(rates), npv);
//     std::cout << "\n ---- NPV ---- " << std::endl;
//     std::cout << std::setprecision(4) << std::fixed << clp.rounding()(val(npv)) << std::endl;
//     std::cout << " ---- Sens ----  " << std::endl;
//     for (size_t i = 0; i < rates.size(); i++) {
//         if (sens[i] != 0.0) { std::cout << std::setprecision(4) << std::fixed << dates[i] << ": " << clp.rounding()(sens[i] * 0.01) << std::endl; }
//     }
// };

int main() {
    // pricingFixedRateInstruments();
    // pricingFloatingRateInstruments();
    // currencyConversion();
    // int n        = 1000000;
    // double x     = 0.0;
    // double x_adj = 0.0;
    // repeated_sin_checkpointed(n, x, x_adj);
    sensCalculation();
    return 0;
}