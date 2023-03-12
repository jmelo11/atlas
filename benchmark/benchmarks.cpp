#define ANKERL_NANOBENCH_IMPLEMENT
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
#include <algorithm>
#include <atlas/aad/aad.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/multithreading/threadpool.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <nanobench.h>

using namespace Atlas;

void sizes() {
    Date startDate(1, Month::Aug, 2020);
    Date endDate(1, Month::Aug, 2021);
    double notional = 100;

    std::cout << "\nSizes: \n" << std::endl;

    Cashflow cashflow(endDate, notional);
    std::cout << "sizeof(Cashflow) = " << sizeof(Cashflow) << std::endl;

    InterestRate rate(0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    FixedRateCoupon coupon(startDate, endDate, notional, rate);
    std::cout << "sizeof(FixedRateCoupon) = " << sizeof(FixedRateCoupon) << std::endl;

    LIBOR6M index;
    double fixing = 0.03;
    double spread = 0.01;
    FloatingRateCoupon coupon2(startDate, endDate, notional, spread, index);
    std::cout << "sizeof(FloatingRateCoupon) = " << sizeof(FloatingRateCoupon) << std::endl;

    FloatingRateCoupon2 coupon3(startDate, endDate, notional, spread, std::make_shared<LIBOR6M>());
    std::cout << "sizeof(FloatingRateCoupon2) = " << sizeof(FloatingRateCoupon2) << std::endl;
}

void bechmarkFixedRateCoupons() {
    // this benchmark tries to measure the time cost of creating and iterating over a vector of coupons
    // first test: 1000 coupons and compare quantlib against atlas

    // benchmark description
    ankerl::nanobench::Bench().title("Fixed Rate Coupon Creation and Iteration");

    // parameters
    size_t numCoupons = 100000;
    Date startDate(1, Month::Aug, 2020);
    Date endDate(1, Month::September, 2020);
    double notional = 100;
    double rate     = 0.03;
    InterestRate interestRate(rate, Actual360(), Compounding::Simple, Frequency::Annual);

    // Atlas
    std::vector<FixedRateCoupon> coupons;
    ankerl::nanobench::Bench().run("Build: Atlas", [&]() {
        for (size_t i = 0; i < numCoupons; ++i) { coupons.push_back(FixedRateCoupon(startDate, endDate, notional, interestRate)); }
    });

    // QuantLib
    QuantLib::Leg qlCoupons;
    ankerl::nanobench::Bench().minEpochIterations(10).run("Build: QuantLib", [&]() {
        for (size_t i = 0; i < numCoupons; ++i) {
            qlCoupons.push_back(boost::make_shared<QuantLib::FixedRateCoupon>(endDate, notional, interestRate, startDate, endDate));
        }
    });

    // benchmark
    ankerl::nanobench::Bench().run("Atlas", [&]() {
        double sum = 0;
        for (auto& coupon : coupons) { sum += coupon.amount(); }
    });

    ankerl::nanobench::Bench().run("QuantLib", [&]() {
        double sum = 0;
        for (auto& coupon : qlCoupons) { sum += coupon->amount(); }
    });

    ankerl::nanobench::Bench().run("Parallel Atlas", [&]() {
        double sum = 0;
// parallel for each
#pragma omp parallel for
        for (size_t i = 0; i < numCoupons; ++i) { sum += coupons[i].amount(); }
    });

    ankerl::nanobench::Bench().run("Parallel QuantLib", [&]() {
        double sum = 0;
// parallel for each
#pragma omp parallel for
        for (size_t i = 0; i < numCoupons; ++i) { sum += qlCoupons[i]->amount(); }
    });
}

int main() {
    // benchmarkTest();
    bechmarkFixedRateCoupons();
    sizes();
}