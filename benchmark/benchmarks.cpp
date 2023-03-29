#define ANKERL_NANOBENCH_IMPLEMENT

#include <ql/cashflows/couponpricer.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/iborcoupon.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
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

void sizes() {
    std::cout << "sizeof(Cashflow) = " << sizeof(Cashflow) << std::endl;

    std::cout << "sizeof(FixedRateCoupon) = " << sizeof(FixedRateCoupon) << std::endl;

    std::cout << "sizeof(FloatingRateCoupon) = " << sizeof(FloatingRateCoupon) << std::endl;
}

void bechmarkFixedRateCoupons() {
    // this benchmark tries to measure the time cost of creating and iterating over a vector of coupons
    // first test: 1000 coupons and compare quantlib against atlas

    // benchmark description
    ankerl::nanobench::Bench().title("Fixed Rate Coupon Creation and Iteration");

    // parameters
    size_t numCoupons = 10000;
    Date startDate(1, Month::Aug, 2020);
    QuantLib::Settings::instance().evaluationDate() = startDate;
    Date endDate(1, Month::September, 2020);
    double notional = 100;
    double rate     = 0.03;
    InterestRate interestRate(rate, Actual360(), Compounding::Simple, Frequency::Annual);

    // Atlas
    std::vector<FixedRateCoupon> coupons;
    for (size_t i = 0; i < numCoupons; ++i) { coupons.push_back(FixedRateCoupon(startDate, endDate, notional, interestRate)); }

    // QuantLib
    QuantLib::Leg qlCoupons;
    for (size_t i = 0; i < numCoupons; ++i) {
        qlCoupons.push_back(boost::make_shared<QuantLib::FixedRateCoupon>(endDate, notional, interestRate, startDate, endDate));
    }

    // benchmark
    ankerl::nanobench::Bench().run("Amount: Atlas FixedRateCoupon", [&]() {
        adouble sum = 0;
        for (size_t i = 0; i < numCoupons; ++i) { sum += coupons[i].amount(); }
    });

    ankerl::nanobench::Bench().run("Amount: QuantLib FixedRateCoupon", [&]() {
        adouble sum = 0;
        for (size_t i = 0; i < numCoupons; ++i) { sum += qlCoupons[i]->amount(); }
    });

    ankerl::nanobench::Bench().run("Parallel Amount: QuantLib FixedRateCoupon", [&]() {
        adouble sum = 0;
// parallel for each
#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < numCoupons; ++i) { sum += qlCoupons[i]->amount(); }
    });

    ankerl::nanobench::Bench().run("Parallel Amount: Atlas FixedRateCoupon", [&]() {
        adouble sum = 0;
// parallel for each
#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < numCoupons; ++i) { sum += coupons[i].amount(); }
    });
}

void bechmarkFloatingRateCoupons() {
    ankerl::nanobench::Bench().title("Floating Rate Coupon Creation and Iteration");

    size_t numCoupons = 10000;
    Date startDate(1, Month::Aug, 2020);
    Date endDate(1, Month::September, 2020);
    double notional = 100;
    double spread   = 0.01;

    // Atlas index and curve settings
    CurveContextStore& store_ = CurveContextStore::instance();
    FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curveStrategy, index);
    auto& context = store_.at("TEST");

    // QuantLib index
    QuantLib::Handle<QuantLib::YieldTermStructure> curveHandle(boost::make_shared<QuantLib::FlatForward>(startDate, 0.03, Actual360()));
    boost::shared_ptr<QuantLib::IborIndex> qlIndex = boost::make_shared<QuantLib::Euribor6M>(curveHandle);
    qlIndex->addFixing(Date(31, Month::July, 2020), 0.03);
    QuantLib::Leg qlCoupons;
    for (size_t i = 0; i < numCoupons; ++i) {
        qlCoupons.push_back(boost::make_shared<QuantLib::IborCoupon>(endDate, notional, startDate, endDate, 0, qlIndex));
    }

    // Atlas
    std::vector<FloatingRateCoupon> coupons;
    for (size_t i = 0; i < numCoupons; ++i) { coupons.push_back(FloatingRateCoupon(startDate, endDate, notional, spread, context)); }

    // benchmark
    QuantLib::Volatility volatility = 0.0;
    QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
    vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
        new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::ModifiedFollowing, volatility, Actual365Fixed())));
    boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);
    pricer->setCapletVolatility(vol);

    ankerl::nanobench::Bench().run("Amount: Atlas FloatingRateCoupon", [&]() {
        adouble sum = 0;
        for (size_t i = 0; i < numCoupons; ++i) {
            coupons[i].fixing(0.03);
            sum += coupons[i].amount();
        }
    });

    ankerl::nanobench::Bench().run("Amount: QuantLib FloatingRateCoupon", [&]() {
        double sum = 0;
        for (size_t i = 0; i < numCoupons; ++i) {
            auto c = boost::dynamic_pointer_cast<QuantLib::IborCoupon>(qlCoupons[i]);
            if (c) {
                c->setPricer(pricer);
                sum += c->amount();
            }
        }
    });

    ankerl::nanobench::Bench().run("Parallel Amount: QuantLib FloatingRateCoupon", [&]() {
        double sum = 0;
        // parallel for each
#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < numCoupons; ++i) {
            auto c = boost::dynamic_pointer_cast<QuantLib::IborCoupon>(qlCoupons[i]);
            if (c) {
                c->setPricer(pricer);
                sum += c->nominal();
            }
        }
    });

    ankerl::nanobench::Bench().run("Parallel Amount: Atlas FloatingRateCoupon", [&]() {
        adouble sum = 0;
        // parallel for each
#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < numCoupons; ++i) {
            coupons[i].fixing(0.03);
            sum += coupons[i].amount();
        }
    });
}

void pricingBenchmark() {
    ankerl::nanobench::Bench().title("Fixed Rate Coupon Creation and Iteration");

    // parameters
    size_t numInstruments = 100000;
    Date startDate(1, Month::Aug, 2020);
    QuantLib::Settings::instance().evaluationDate() = startDate;
    Date endDate(1, Month::September, 2025);
    Frequency paymentFreq = Frequency::Semiannual;
    double notional       = 100;
    double rate           = 0.03;
    InterestRate interestRate(rate, Actual360(), Compounding::Simple, Frequency::Annual);
    std::vector<FixedRateBulletInstrument> instruments;

    CurveContextStore& mainStore = CurveContextStore::instance();
    if (!mainStore.hasContext("TEST")) {
        FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
        RateIndex index("TEST", Frequency::Annual, Actual360());
        mainStore.createCurveContext("TEST", curveStrategy, index);
    }
    auto& context = mainStore.at("TEST");

    for (size_t i = 0; i < numInstruments; ++i) {
        instruments.push_back(FixedRateBulletInstrument(startDate, endDate, paymentFreq, notional, interestRate, context));
    }

    // slice instruments in sub vectors
    std::vector<std::vector<FixedRateBulletInstrument>> slices;
    size_t sliceSize = 1000;

    for (size_t i = 0; i < numInstruments; i += sliceSize) {
        slices.push_back(std::vector<FixedRateBulletInstrument>(instruments.begin() + i, instruments.begin() + i + sliceSize));
    }

    ankerl::nanobench::Bench().run("Parallel Price: Atlas FixedRateBulletInstrument", [&]() {
        ThreadPool* pool = ThreadPool::getInstance();
        pool->start();
        std::vector<TaskHandle> futures;
        adouble npv = 0;
        for (auto& slice : slices) {
            auto task = [&]() {
                // std::cout << pool->threadNum() << std::endl;
                Date evalDate(1, Month::Aug, 2020);
                CurveContextStore& store_ = CurveContextStore::instance();
                store_.copyContextsFromStore(mainStore);
                Indexer indexer;
                for (size_t i = 0; i < sliceSize; i++) { slice[i].accept(indexer); }
                MarketRequest request;
                indexer.setRequest(request);
                StaticCurveModel model(request);
                MarketData marketData = model.simulate(evalDate);

                NPVCalculator calculator(marketData);
                for (size_t i = 0; i < sliceSize; i++) { slice[i].accept(calculator); }
                npv += calculator.results();
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }

        for (auto& future : futures) { pool->activeWait(future); }
    });

    // QL instruments
    std::vector<QuantLib::FixedRateBond> qlBonds;
    for (size_t i = 0; i < numInstruments; ++i) {
        Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFreq);
        auto qlBond       = QuantLib::FixedRateBond(0, notional, schedule, {interestRate});
        qlBonds.push_back(qlBond);
    }

    std::vector<std::vector<QuantLib::FixedRateBond>> qlSlices;

    for (size_t i = 0; i < numInstruments; i += sliceSize) {
        qlSlices.push_back(std::vector<QuantLib::FixedRateBond>(qlBonds.begin() + i, qlBonds.begin() + i + sliceSize));
    }

    ankerl::nanobench::Bench().run("Parallel Pricing: QuantLib FixedRateBulletInstrument", [&]() {
        ThreadPool* pool = ThreadPool::getInstance();
        pool->start();
        std::vector<TaskHandle> futures;
        double npv = 0;
        QuantLib::RelinkableHandle<QuantLib::YieldTermStructure> discountingTermStructure;
        boost::shared_ptr<QuantLib::YieldTermStructure> curve(
            new QuantLib::FlatForward(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual));
        discountingTermStructure.linkTo(curve);

        for (auto& slice : qlSlices) {
            auto task = [&]() {
                boost::shared_ptr<QuantLib::PricingEngine> bondEngine(new QuantLib::DiscountingBondEngine(discountingTermStructure));
                for (auto& qlBond : slice) {
                    qlBond.setPricingEngine(bondEngine);
                    npv += qlBond.NPV();
                }
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }

        for (auto& future : futures) { pool->activeWait(future); }
    });
}

int main() {
    // sizes();
    bechmarkFixedRateCoupons();
    bechmarkFloatingRateCoupons();
    pricingBenchmark();
}