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
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/multithreading/threadpool.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <nanobench.h>
using namespace Atlas;

template <typename NumType>
void sizes() {
    std::cout << "sizeof(Cashflow) = " << sizeof(Cashflow<NumType>) << std::endl;

    std::cout << "sizeof(FixedRateCoupon) = " << sizeof(FixedRateCoupon<NumType>) << std::endl;

    std::cout << "sizeof(FloatingRateCoupon) = " << sizeof(FloatingRateCoupon<NumType>) << std::endl;
}

template <typename NumType>
void bechmarkFixedRateCoupons(MarketStore& store_) {
    // this benchmark tries to measure the time cost of creating and iterating over a vector of coupons
    // first test: 1000 coupons and compare quantlib against atlas

    // benchmark description
    ankerl::nanobench::Bench().title("Fixed Rate Coupon Creation and Iteration");

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
        qlInterestRate = QuantLib::InterestRate(rate.val, QuantLib::Actual360(), Compounding::Simple, Frequency::Annual);
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
    ankerl::nanobench::Bench().run("Amount: Atlas FixedRateCoupon", [&]() {
        NumType sum = 0;
        // iter over all slices
        for (auto& slice : slices) {
            for (size_t i = 0; i < slice.size(); ++i) { sum += slice[i].amount(); }
        }
    });

    ThreadPool* pool = ThreadPool::getInstance();
    ankerl::nanobench::Bench().run("Parallel Amount: Atlas FixedRateCoupon", [&]() {
        NumType sum = 0;
        // parallel for each
        pool->start();
        std::vector<TaskHandle> futures;
        for (auto& slice : slices) {
            auto task = [&]() {
                for (size_t i = 0; i < slice.size(); ++i) { sum += slice[i].amount(); }
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();
    });

    ankerl::nanobench::Bench().run("Amount: QuantLib FixedRateCoupon", [&]() {
        NumType sum = 0;
        // iter over all slices
        for (auto& slice : qlSlices) {
            for (size_t i = 0; i < slice.size(); ++i) { sum += slice[i]->amount(); }
        }
    });

    ankerl::nanobench::Bench().run("Parallel Amount: QuantLib FixedRateCoupon", [&]() {
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
void bechmarkFloatingRateCoupons(MarketStore& store_) {
    ankerl::nanobench::Bench().title("Floating Rate Coupon Creation and Iteration");

    size_t numCoupons = 100000;
    Date startDate(1, Month::Aug, 2020);
    Date endDate(1, Month::September, 2020);
    double notional = 100;
    NumType spread  = 0.01;

    // Atlas index and curve settings
    auto& context = store_.curveContext("TEST");

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
        for (size_t j = 0; j < sliceSize; ++j) { slices[i].push_back(FloatingRateCoupon<NumType>(startDate, endDate, notional, spread, context)); }
    }

    // benchmark
    QuantLib::Volatility volatility = 0.0;
    QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
    vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
        new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::ModifiedFollowing, volatility, Actual365Fixed())));
    boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);
    pricer->setCapletVolatility(vol);

    ankerl::nanobench::Bench().run("Amount: Atlas FloatingRateCoupon", [&]() {
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
    ankerl::nanobench::Bench().run("Parallel Amount: Atlas FloatingRateCoupon", [&]() {
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

    ankerl::nanobench::Bench().run("Amount: QuantLib FloatingRateCoupon", [&]() {
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
    ankerl::nanobench::Bench().run("Parallel Amount: QuantLib FloatingRateCoupon", [&]() {
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
void pricingBenchmark(MarketStore& mainStore_) {
    ankerl::nanobench::Bench().title("Fixed Rate Coupon Creation and Iteration");

    // parameters
    size_t numInstruments = 10000;
    Date startDate(1, Month::Aug, 2020);
    QuantLib::Settings::instance().evaluationDate() = startDate;
    Date endDate(1, Month::September, 2025);
    Frequency paymentFreq = Frequency::Semiannual;
    double notional       = 100;
    NumType rate          = 0.03;
    InterestRate<NumType> interestRate(rate, Actual360(), Compounding::Simple, Frequency::Annual);

    QuantLib::InterestRate qlInterestRate;
    if constexpr (std::is_same_v<NumType, double>) {
        qlInterestRate = QuantLib::InterestRate(rate, QuantLib::Actual360(), Compounding::Simple, Frequency::Annual);
    } else {
        qlInterestRate = QuantLib::InterestRate(rate.val, QuantLib::Actual360(), Compounding::Simple, Frequency::Annual);
    }

    // slices of atlas instruments
    size_t numThreads = std::thread::hardware_concurrency();
    size_t sliceSize  = numInstruments / numThreads;

    std::vector<std::vector<FixedRateBulletInstrument<NumType>>> slices;
    for (size_t i = 0; i < numThreads; ++i) {
        std::vector<FixedRateBulletInstrument<NumType>> slice;
        for (size_t j = 0; j < sliceSize; ++j) {
            slice.push_back(
                FixedRateBulletInstrument<NumType>(startDate, endDate, paymentFreq, notional, interestRate, mainStore_.curveContext("TEST")));
        }
        slices.push_back(slice);
    }

    ThreadPool* pool = ThreadPool::getInstance();
    // ankerl::nanobench::Bench().run("Price: Atlas FixedRateBulletInstrument", [&]() {
    //     pool->start(0);

    //     std::vector<TaskHandle> futures;
    //     NumType npv = 0;
    //     for (auto& slice : slices) {
    //         auto task = [&]() {
    //             // std::cout << pool->threadNum() << std::endl;
    //             Date evalDate(1, Month::Aug, 2020);
    //             MarketStore store;
    //             store.copyFromStore(mainStore_);

    //             Indexer<NumType> indexer;
    //             for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(indexer); }

    //             MarketRequest request = indexer.request();
    //             StaticCurveModel<NumType> model(request, store);

    //             MarketData<NumType> marketData = model.simulate(evalDate);

    //             NPVCalculator<NumType> calculator(marketData);
    //             for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(calculator); }
    //             npv += calculator.results();
    //             return true;
    //         };
    //         futures.push_back(pool->spawnTask(task));
    //     }
    //     for (auto& future : futures) { pool->activeWait(future); }
    //     pool->stop();
    // });

    ankerl::nanobench::Bench().run("Parallel Price: Atlas FixedRateBulletInstrument", [&]() {
        pool->start();

        std::vector<TaskHandle> futures;
        NumType npv = 0;
        for (auto& slice : slices) {
            auto task = [&]() {
                // std::cout << pool->threadNum() << std::endl;

                MarketStore store;
                store.copyFromStore(mainStore_);

                Indexer<NumType> indexer;
                for (size_t i = 0; i < slice.size(); i++) { slice[i].accept(indexer); }

                MarketRequest request = indexer.request();
                StaticCurveModel<NumType> model(request, store);

                MarketData<NumType> marketData = model.simulate(startDate);

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

    // // slices of quantlib instruments

    // std::vector<std::vector<QuantLib::FixedRateBond>> qlSlices;
    // Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFreq);
    // for (size_t i = 0; i < numThreads; ++i) {
    //     std::vector<QuantLib::FixedRateBond> slice;
    //     for (size_t j = 0; j < sliceSize; ++j) { slice.push_back(QuantLib::FixedRateBond(0, notional, schedule, {qlInterestRate})); }
    //     qlSlices.push_back(slice);
    // }

    // QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
    // boost::shared_ptr<QuantLib::YieldTermStructure> curve(
    //     new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
    // discountingTermStructure.linkTo(curve);
    // boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));

    // ankerl::nanobench::Bench().run("Pricing: QuantLib FixedRateBulletInstrument", [&]() {
    //     pool->start(0);
    //     std::vector<TaskHandle> futures;
    //     double npv = 0;
    //     for (auto& slice : qlSlices) {
    //         auto task = [&]() {
    //             double npv_ = 0;
    //             for (auto& qlBond : slice) {
    //                 qlBond.setPricingEngine(bondEngine);
    //                 npv_ += qlBond.NPV();
    //             }
    //             npv += npv_;
    //             return true;
    //         };
    //         futures.push_back(pool->spawnTask(task));
    //     }
    //     for (auto& future : futures) { pool->activeWait(future); }
    // });

    // ankerl::nanobench::Bench().run("Parallel Pricing: QuantLib FixedRateBulletInstrument", [&]() {
    //     pool->start();
    //     std::vector<TaskHandle> futures;
    //     double npv = 0;
    //     for (auto& slice : qlSlices) {
    //         auto task = [&]() {
    //             double npv_ = 0;
    //             for (auto& qlBond : slice) {
    //                 qlBond.setPricingEngine(bondEngine);
    //                 npv_ += qlBond.NPV();
    //             }
    //             npv += npv_;
    //             return true;
    //         };
    //         futures.push_back(pool->spawnTask(task));
    //     }
    //     for (auto& future : futures) { pool->activeWait(future); }
    // });
}

int main() {
    Date startDate(1, Month::Aug, 2020);
    MarketStore store_;
    FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curveStrategy, index);

    // bechmarkFixedRateCoupons<double>(store_);
    // bechmarkFloatingRateCoupons<double>(store_);
    pricingBenchmark<double>(store_);

    // bechmarkFixedRateCoupons<dual>(store_);
    // bechmarkFloatingRateCoupons<dual>(store_);
    // pricingBenchmark<dual>(store_);
}