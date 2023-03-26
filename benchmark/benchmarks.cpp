#define ANKERL_NANOBENCH_IMPLEMENT

#include <ql/cashflows/couponpricer.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/iborcoupon.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <algorithm>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/multithreading/threadpool.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
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
    ankerl::nanobench::Bench().run("Build: Atlas FixedRateCoupon", [&]() {
        for (size_t i = 0; i < numCoupons; ++i) { coupons.push_back(FixedRateCoupon(startDate, endDate, notional, interestRate)); }
    });

    // QuantLib
    QuantLib::Leg qlCoupons;
    ankerl::nanobench::Bench().minEpochIterations(10).run("Build: QuantLib FixedRateCoupon", [&]() {
        for (size_t i = 0; i < numCoupons; ++i) {
            qlCoupons.push_back(boost::make_shared<QuantLib::FixedRateCoupon>(endDate, notional, interestRate, startDate, endDate));
        }
    });

    // benchmark
    ankerl::nanobench::Bench().run("Amount: Atlas FixedRateCoupon", [&]() {
        double sum = 0;
        for (size_t i = 0; i < numCoupons; ++i) { sum += coupons[i].amount(); }
    });

    ankerl::nanobench::Bench().run("Amount: QuantLib FixedRateCoupon", [&]() {
        double sum = 0;
        for (size_t i = 0; i < numCoupons; ++i) { sum += qlCoupons[i]->amount(); }
    });

    ankerl::nanobench::Bench().run("Parallel Amount: QuantLib FixedRateCoupon", [&]() {
        double sum = 0;
// parallel for each
#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < numCoupons; ++i) { sum += qlCoupons[i]->amount(); }
    });

    ankerl::nanobench::Bench().run("Parallel Amount: Atlas FixedRateCoupon", [&]() {
        double sum = 0;
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

    QuantLib::Leg qlCoupons;
    ankerl::nanobench::Bench().minEpochIterations(10).run("Build: QuantLib FloatingRateCoupon", [&]() {
        for (size_t i = 0; i < numCoupons; ++i) {
            qlCoupons.push_back(boost::make_shared<QuantLib::IborCoupon>(endDate, notional, startDate, endDate, 0, qlIndex));
        }
    });

    // Atlas
    std::vector<FloatingRateCoupon> coupons;
    ankerl::nanobench::Bench().minEpochIterations(100).run("Build: Atlas FloatingRateCoupon", [&]() {
        for (size_t i = 0; i < numCoupons; ++i) { coupons.push_back(FloatingRateCoupon(startDate, endDate, notional, spread, context)); }
    });

    // benchmark
    QuantLib::Volatility volatility = 0.0;
    QuantLib::Handle<QuantLib::OptionletVolatilityStructure> vol;
    vol = QuantLib::Handle<QuantLib::OptionletVolatilityStructure>(boost::shared_ptr<QuantLib::OptionletVolatilityStructure>(
        new QuantLib::ConstantOptionletVolatility(0, QuantLib::NullCalendar(), QuantLib::ModifiedFollowing, volatility, Actual365Fixed())));
    boost::shared_ptr<QuantLib::IborCouponPricer> pricer(new QuantLib::BlackIborCouponPricer);
    pricer->setCapletVolatility(vol);

    ankerl::nanobench::Bench().run("Amount: Atlas FloatingRateCoupon", [&]() {
        double sum = 0;
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
                sum += c->nominal();
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
        double sum = 0;
        // parallel for each
#pragma omp parallel for reduction(+ : sum)
        for (size_t i = 0; i < numCoupons; ++i) {
            coupons[i].fixing(0.03);
            sum += coupons[i].amount();
        }
    });
}

int main() {
    // sizes();
    bechmarkFixedRateCoupons();
    bechmarkFloatingRateCoupons();
}