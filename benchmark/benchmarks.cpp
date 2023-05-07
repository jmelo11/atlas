#define ANKERL_NANOBENCH_IMPLEMENT

#include <ql/cashflows/couponpricer.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/iborcoupon.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/interestrate.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <algorithm>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/multithreading/BS_thread_pool.hpp>
#include <atlas/multithreading/threadpool.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <nanobench.h>
#include <execution>
using namespace Atlas;

template <typename NumType>
void sizes() {
    std::cout << "sizeof(Cashflow) = " << sizeof(Cashflow<NumType>) << std::endl;

    std::cout << "sizeof(FixedRateCoupon) = " << sizeof(FixedRateCoupon<NumType>) << std::endl;

    std::cout << "sizeof(FloatingRateCoupon) = " << sizeof(FloatingRateCoupon<NumType>) << std::endl;
}

template <typename NumType>
void bechmarkFixedRateCoupons() {
    // this benchmark tries to measure the time cost of creating and iterating over a vector of coupons
    // first test: 1000 coupons and compare quantlib against atlas

    // benchmark description
    ankerl::nanobench::Bench bench;
    if constexpr (std::is_same_v<NumType, double>) {
        bench.title("(double) Fixed Rate Coupon Creation and Iteration").warmup(100).relative(true);

    } else {
        bench.title("(dual) Fixed Rate Coupon Creation and Iteration").warmup(100).relative(true);
    }

    bench.performanceCounters(true);

    // parameters
    size_t numCoupons = 1000000;
    Date startDate(1, Month::Aug, 2020);
    QuantLib::Settings::instance().evaluationDate() = startDate;
    Date endDate(1, Month::September, 2020);
    double notional = 100;
    NumType rate    = 0.03;

    InterestRate<NumType> interestRate(rate, Actual360(), Compounding::Simple, Frequency::Annual);

    QuantLib::InterestRate qlInterestRate;
    if constexpr (std::is_same_v<NumType, double>) {
        qlInterestRate = QuantLib::InterestRate(rate, QuantLib::Actual360(), Compounding::Simple, Frequency::Annual);
    } else {
        qlInterestRate = QuantLib::InterestRate(val(rate), QuantLib::Actual360(), Compounding::Simple, Frequency::Annual);
    }
    // Atlas
    size_t numThreads = std::thread::hardware_concurrency();
    size_t sliceSize  = numCoupons / numThreads;
    // slices of coupons
    std::vector<std::vector<FixedRateCoupon<NumType>>> slices(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
        for (size_t j = 0; j < sliceSize; ++j) { slices[i].push_back(FixedRateCoupon<NumType>(startDate, endDate, notional, interestRate)); }
    }

    // slices of QL coupons
    std::vector<QuantLib::Leg> qlSlices(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
        for (size_t j = 0; j < sliceSize; ++j) {
            qlSlices[i].push_back(
                boost::make_shared<QuantLib::FixedRateCoupon>(endDate, notional, qlInterestRate, startDate, endDate, startDate, endDate));
        }
    }

    // benchmark
    bench.run("Amount: Atlas FixedRateCoupon", [&]() {
        NumType sum = 0;
        // iter over all slices
        for (auto& slice : slices) {
            for (size_t i = 0; i < slice.size(); ++i) { sum += slice[i].amount(); }
        }
    });

    ThreadPool* pool = ThreadPool::getInstance();
    bench.run("Parallel Amount: Atlas FixedRateCoupon", [&]() {
        NumType sum = 0;
        // parallel for each
        pool->start();
        std::vector<TaskHandle> futures;
        for (auto& slice : slices) {
            auto task = [&]() {
                NumType sum_ = 0.0;
                for (size_t i = 0; i < slice.size(); ++i) { sum_ += slice[i].amount(); }
                sum += sum_;
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    });

    BS::thread_pool pool2;
    bench.run("Parallel 2 Amount: Atlas FixedRateCoupon", [&]() {
        NumType sum = 0;
        // parallel for each

        BS::multi_future<bool> futures;
        for (auto& slice : slices) {
            auto task = [&]() {
                NumType sum_ = 0.0;
                for (size_t i = 0; i < slice.size(); ++i) { sum_ += slice[i].amount(); }
                sum += sum_;
                return true;
            };
            futures.push_back(pool2.submit(task));
        }
        futures.wait();
    });

    bench.run("Amount: QuantLib FixedRateCoupon", [&]() {
        NumType sum = 0;
        // iter over all slices
        for (auto& slice : qlSlices) {
            for (size_t i = 0; i < slice.size(); ++i) { sum += slice[i]->amount(); }
        }
    });

    bench.run("Parallel Amount: QuantLib FixedRateCoupon", [&]() {
        NumType sum = 0;
        // parallel for each
        pool->start();
        std::vector<TaskHandle> futures;
        for (auto& slice : qlSlices) {
            auto task = [&]() {
                for (size_t i = 0; i < slice.size(); ++i) { sum += slice[i]->amount(); }
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    });
}

template <typename NumType>
void bechmarkFloatingRateCoupons() {
    ankerl::nanobench::Bench bench;
    if (std::is_same_v<NumType, double>) {
        bench.title("(double) Floating Rate Coupon Creation and Iteration").warmup(100).relative(true);
    } else {
        bench.title("(dual) Floating Rate Coupon Creation and Iteration").warmup(100).relative(true);
    }

    bench.performanceCounters(true);

    size_t numCoupons = 100000;
    Date startDate(1, Month::Aug, 2020);
    Date endDate(1, Month::September, 2020);
    double notional   = 100;
    NumType spread    = 0.01;
    NumType rateValue = 0.03;
    // Atlas index and curve settings
    DayCounter dayCounter   = Actual360();
    Frequency frequency     = Frequency::Annual;
    Compounding compounding = Compounding::Simple;

    MarketStore<NumType> store_(startDate);
    FlatForwardStrategy<NumType> strategy(startDate, rateValue, dayCounter, compounding, frequency);
    YieldTermStructure<NumType> curve(std::make_unique<FlatForwardStrategy<NumType>>(strategy));
    RateIndex<NumType> index(startDate, frequency);
    store_.addCurve("TEST", curve, index);

    auto& indexContext = store_.rateIndexContext("TEST");

    // QuantLib index
    QuantLib::Handle<QuantLib::YieldTermStructure> curveHandle(boost::make_shared<QuantLib::FlatForward>(startDate, 0.03, Actual360()));
    boost::shared_ptr<QuantLib::IborIndex> qlIndex = boost::make_shared<QuantLib::Euribor6M>(curveHandle);
    qlIndex->addFixing(Date(31, Month::July, 2020), 0.03);

    size_t numThreads = std::thread::hardware_concurrency();
    size_t sliceSize  = numCoupons / numThreads;

    // Slice of QL coupons
    std::vector<QuantLib::Leg> qlSlices(numThreads);
    for (size_t i = 0; i < numThreads; i++) {
        for (size_t j = 0; j < sliceSize; j++) {
            qlSlices[i].push_back(boost::make_shared<QuantLib::IborCoupon>(endDate, notional, startDate, endDate, 0, qlIndex));
        }
    }

    // Slices of Atlas coupons
    std::vector<std::vector<FloatingRateCoupon<NumType>>> slices(numThreads);
    for (size_t i = 0; i < numThreads; ++i) {
        for (size_t j = 0; j < sliceSize; ++j) {
            slices[i].push_back(FloatingRateCoupon<NumType>(startDate, endDate, notional, spread, indexContext));
        }
    }

    // benchmark
    QuantLib::Volatility volatility = 0.0;
    QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
    vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
        new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::ModifiedFollowing, volatility, Actual365Fixed())));
    boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);
    pricer->setCapletVolatility(vol);

    bench.run("Amount: Atlas FloatingRateCoupon", [&]() {
        NumType sum = 0;
        // iter over all slices
        NumType fwd = 0.03;
        for (auto& slice : slices) {
            for (size_t i = 0; i < slice.size(); ++i) {
                slice[i].fixing(fwd);
                sum += slice[i].amount();
            }
        }
    });

    ThreadPool* pool = ThreadPool::getInstance();
    bench.run("Parallel Amount: Atlas FloatingRateCoupon", [&]() {
        // parallel for each
        pool->start();
        std::vector<TaskHandle> futures;
        for (auto& slice : slices) {
            auto task = [&]() {
                NumType sum = 0;
                NumType fwd = 0.03;
                for (size_t i = 0; i < slice.size(); ++i) {
                    slice[i].fixing(fwd);
                    sum += slice[i].amount();
                }
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    });

    bench.run("Amount: QuantLib FloatingRateCoupon", [&]() {
        double sum = 0;
        // iter over all slices
        for (auto& slice : qlSlices) {
            for (size_t i = 0; i < slice.size(); ++i) {
                auto c = boost::dynamic_pointer_cast<QuantLib::IborCoupon>(slice[i]);
                if (c) {
                    c->setPricer(pricer);
                    sum += c->amount();
                }
            }
        }
    });

    std::mutex m;
    bench.run("Parallel Amount: QuantLib FloatingRateCoupon", [&]() {
        pool->start();
        std::vector<TaskHandle> futures;
        for (auto& slice : qlSlices) {
            auto task = [&slice, &pricer, &m]() {
                double sum = 0;
                for (size_t i = 0; i < slice.size(); i++) {
                    auto c = boost::dynamic_pointer_cast<QuantLib::IborCoupon>(slice[i]);
                    if (c) {
                        {
                            std::unique_lock<std::mutex> lock(m);
                            c->setPricer(pricer);
                        }
                        sum += c->amount();
                    }
                }
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    });
}

template <typename NumType>
void pricingBenchmark() {
    // benchmark
    ankerl::nanobench::Bench bench;

    if constexpr (std::is_same_v<NumType, double>) {
        bench.title("(double) Pricing Instruments").warmup(200).relative(true);
    } else {
        bench.title("(dual) Pricing Instruments").warmup(200).relative(true);
    }

    bench.performanceCounters(true);

    // parameters
    size_t numInstruments = 10000;
    Date startDate(1, Month::Aug, 2020);
    QuantLib::Settings::instance().evaluationDate() = startDate;
    Date endDate(1, Month::September, 2025);
    Frequency paymentFreq = Frequency::Semiannual;
    double notional       = 100;
    NumType rateValue     = 0.03;
    InterestRate<NumType> interestRate(rateValue, Actual360(), Compounding::Simple, Frequency::Annual);

    QuantLib::InterestRate qlInterestRate;
    if constexpr (std::is_same_v<NumType, double>) {
        qlInterestRate = QuantLib::InterestRate(rateValue, QuantLib::Actual360(), Compounding::Simple, Frequency::Annual);
    } else {
        qlInterestRate = QuantLib::InterestRate(val(rateValue), QuantLib::Actual360(), Compounding::Simple, Frequency::Annual);
    }
    // market data
    DayCounter dayCounter   = Actual360();
    Frequency frequency     = Frequency::Annual;
    Compounding compounding = Compounding::Simple;
    MarketStore<NumType> mainStore_(startDate);
    FlatForwardStrategy<NumType> strategy(startDate, rateValue, dayCounter, compounding, frequency);
    YieldTermStructure<NumType> curve_(std::make_unique<FlatForwardStrategy<NumType>>(strategy));
    RateIndex<NumType> index(startDate, frequency);
    mainStore_.addCurve("TEST", curve_, index);

    auto& context = mainStore_.curveContext("TEST");

    // slices of atlas instruments
    size_t numThreads = std::thread::hardware_concurrency();
    size_t sliceSize  = numInstruments / numThreads;

    std::vector<std::vector<FixedRateBulletInstrument<NumType>>> slices;
    for (size_t i = 0; i < numThreads; ++i) {
        std::vector<FixedRateBulletInstrument<NumType>> slice;
        for (size_t j = 0; j < sliceSize; ++j) {
            slice.push_back(FixedRateBulletInstrument<NumType>(startDate, endDate, paymentFreq, notional, interestRate, context));
        }
        slices.push_back(slice);
    }

    ThreadPool* pool = ThreadPool::getInstance();
    bench.run("Price: Atlas FixedRateBulletInstrument", [&]() {
        pool->start(0);

        std::vector<TaskHandle> futures;
        NumType npv = 0;
        for (auto& slice : slices) {
            auto task = [&]() {
                MarketStore<NumType> store(startDate);
                store.cloneFromStore(mainStore_);

                Indexer<NumType> indexer;
                for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(indexer); }

                MarketRequest request = indexer.request();
                SpotMarketDataModel<NumType> model(request, store);

                MarketData<NumType> marketData = model.marketData();

                NPVCalculator<NumType> calculator(marketData);
                for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(calculator); }
                npv += calculator.results();
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    });

    bench.run("Parallel Price: Atlas FixedRateBulletInstrument", [&]() {
        pool->start();
        std::vector<TaskHandle> futures;
        NumType npv = 0;
        for (auto& slice : slices) {
            auto task = [&]() {
                // std::cout << pool->threadNum() << std::endl;

                MarketStore<NumType> store(startDate);
                store.cloneFromStore(mainStore_);

                Indexer<NumType> indexer;
                for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(indexer); }

                MarketRequest request = indexer.request();
                SpotMarketDataModel<NumType> model(request, store);
                MarketData<NumType> marketData = model.marketData();
                NPVCalculator<NumType> calculator(marketData);
                for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(calculator); }
                npv += calculator.results();
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }

        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    });

    BS::thread_pool pool2;
    bench.run("Parallel Price: Atlas FixedRateBulletInstrument", [&]() {
        NumType npv = 0;
        BS::multi_future<bool> futures;
        for (auto& slice : slices) {
            auto task = [&]() {
                // std::cout << pool->threadNum() << std::endl;

                MarketStore<NumType> store(startDate);
                store.cloneFromStore(mainStore_);

                Indexer<NumType> indexer;
                for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(indexer); }

                MarketRequest request = indexer.request();
                SpotMarketDataModel<NumType> model(request, store);
                MarketData<NumType> marketData = model.marketData();
                NPVCalculator<NumType> calculator(marketData);
                for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(calculator); }
                npv += calculator.results();
                return true;
            };
            futures.push_back(pool2.submit(task));
        }
        futures.wait();
    });

    // slices of quantlib instruments

    std::vector<std::vector<QuantLib::FixedRateBond>> qlSlices;
    Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFreq);
    for (size_t i = 0; i < numThreads; ++i) {
        std::vector<QuantLib::FixedRateBond> slice;
        for (size_t j = 0; j < sliceSize; ++j) { slice.push_back(QuantLib::FixedRateBond(0, notional, schedule, {qlInterestRate})); }
        qlSlices.push_back(slice);
    }

    QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
    boost::shared_ptr<QuantLib::YieldTermStructure> curve(
        new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    discountingTermStructure.linkTo(curve);
    boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));

    bench.run("Pricing: QuantLib FixedRateBulletInstrument", [&]() {
        pool->start(0);
        std::vector<TaskHandle> futures;
        double npv = 0;
        for (auto& slice : qlSlices) {
            auto task = [&]() {
                double npv_ = 0;
                for (auto& qlBond : slice) {
                    qlBond.setPricingEngine(bondEngine);
                    npv_ += qlBond.NPV();
                }
                npv += npv_;
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
    });

    bench.run("Parallel Pricing: QuantLib FixedRateBulletInstrument", [&]() {
        pool->start();
        std::vector<TaskHandle> futures;
        double npv = 0;
        for (auto& slice : qlSlices) {
            auto task = [&]() {
                double npv_ = 0;
                for (auto& qlBond : slice) {
                    qlBond.setPricingEngine(bondEngine);
                    npv_ += qlBond.NPV();
                }
                npv += npv_;
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
    });
}

template <typename NumType>
void pricingBenchmark2() {
    ankerl::nanobench::Bench bench;

    if constexpr (std::is_same_v<NumType, double>) {
        bench.title("(double) Pricing 2 Instruments").warmup(200).relative(true);
    } else {
        bench.title("(dual) Pricing 2 Instruments").warmup(200).relative(true);
    }
    bench.performanceCounters(true);

    // parameters
    size_t numInstruments = 10000;
    Date startDate(1, Month::Aug, 2020);
    QuantLib::Settings::instance().evaluationDate() = startDate;
    Date endDate(1, Month::September, 2025);
    Frequency paymentFreq = Frequency::Semiannual;
    double notional       = 100;
    NumType rateValue     = 0.03;

    DayCounter dayCounter   = Actual360();
    Frequency frequency     = Frequency::Annual;
    Compounding compounding = Compounding::Simple;
    MarketStore<NumType> mainStore_(startDate);
    FlatForwardStrategy<NumType> strategy(startDate, rateValue, dayCounter, compounding, frequency);
    YieldTermStructure<NumType> curve_(std::make_unique<FlatForwardStrategy<NumType>>(strategy));
    RateIndex<NumType> index(startDate, frequency);
    mainStore_.addCurve("TEST", curve_, index);

    auto& context = mainStore_.curveContext("TEST");

    std::vector<FixedRateBulletInstrument<NumType>> instruments;
    for (size_t i = 0; i < numInstruments; ++i) {
        InterestRate<NumType> interestRate(rateValue, Actual360(), Compounding::Simple, Frequency::Annual);
        instruments.push_back(FixedRateBulletInstrument<NumType>(startDate, endDate, paymentFreq, notional, interestRate, context));
    }
    Indexer<NumType> indexer;
    bench.run("Indexer using for_each", [&]() {
    std::for_each(instruments.begin(), instruments.end(), [&](auto& instrument) { instrument.accept(indexer); });
    });

    indexer.clear();
    bench.run("Indexer using for_each (parallel)", [&]() {
    std::for_each(std::execution::par, instruments.begin(), instruments.end(), [&](auto& instrument) { instrument.accept(indexer); });
    });

    MarketRequest request = indexer.request();
    SpotMarketDataModel<NumType> model(request, mainStore_);
    MarketData<NumType> marketData = model.marketData();
    NPVCalculator<NumType> calculator(marketData);

    bench.run("NPVCalculator using for_each", [&]() {
    std::for_each(instruments.begin(), instruments.end(), [&](auto& instrument) { instrument.accept(calculator); });
    });

    if constexpr (std::is_same_v<NumType, double>) {
        std::cout << "NPV: " << calculator.results() << std::endl;
    } else {
        std::cout << "NPV: " << val(calculator.results()) << std::endl;
    }

    calculator.clear();
    bench.run("NPVCalculator using for_each (parallel)", [&]() {
    std::for_each(std::execution::par, instruments.begin(), instruments.end(), [&](auto& instrument) { instrument.accept(calculator); });
    });
    
    if constexpr (std::is_same_v<NumType, double>) {
        std::cout << "NPV: " << calculator.results() << std::endl;
    } else {
        std::cout << "NPV: " << val(calculator.results()) << std::endl;
    }
}

int main() {
    bechmarkFixedRateCoupons<double>();
    //bechmarkFloatingRateCoupons<double>();
    pricingBenchmark<double>();
    pricingBenchmark2<double>();

    bechmarkFixedRateCoupons<dual>();
    //bechmarkFloatingRateCoupons<dual>();
    pricingBenchmark<dual>();
    pricingBenchmark2<dual>();
}