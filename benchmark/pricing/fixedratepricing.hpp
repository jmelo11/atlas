#ifndef E33FF0E7_2A2F_48F6_9352_1F04A5DA0613
#define E33FF0E7_2A2F_48F6_9352_1F04A5DA0613

// QL
#include <ql/cashflows/couponpricer.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/iborcoupon.hpp>
#include <ql/indexes/ibor/euribor.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bonds/zerocouponbond.hpp>
#include <ql/interestrate.hpp>
#include <ql/pricingengines/bond/discountingbondengine.hpp>
#include <ql/termstructures/volatility/optionlet/constantoptionletvol.hpp>
#include <ql/termstructures/yield/flatforward.hpp>
// AT
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>

// MT
#include <atlas/multithreading/threadpool.hpp>

namespace PricingBenchmark {
    namespace QL = QuantLib;
    namespace AT = Atlas;

    inline double priceZCB_QL_ST(size_t n) {
        QL::Date today(1, QL::January, 2020);
        QL::Settings::instance().evaluationDate() = today;

        // bond parameters
        QL::Date startDate              = today;
        QL::Date endDate                = startDate + QL::Period(10, QL::Years);
        QL::Frequency paymentFreq       = QL::Semiannual;
        QL::Calendar calendar           = QL::NullCalendar();
        QL::DayCounter couponDayCounter = QL::Thirty360(QL::Thirty360::BondBasis);
        double couponRate               = 0.05;
        double curveRate                = 0.06;
        double notional                 = 100.0;
        QL::InterestRate rate(couponRate, couponDayCounter, QL::Simple, QL::Annual);

        std::vector<QL::ZeroCouponBond> bonds;
        bonds.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            bonds.push_back(QL::ZeroCouponBond(0, calendar, notional * rate.discountFactor(startDate, endDate), endDate));
        }

        // discounting term structure
        QL::DayCounter curveDayCounter = QL::Actual360();
        boost::shared_ptr<QL::FlatForward> curvePtr(new QL::FlatForward(today, curveRate, curveDayCounter));
        QL::Handle<QL::YieldTermStructure> discountingTermStructure(curvePtr);

        // pricing engine
        boost::shared_ptr<QL::PricingEngine> bondEngine(new QL::DiscountingBondEngine(discountingTermStructure));

        // calculate the npv
        double npv = 0.0;
        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) {
            bond.setPricingEngine(bondEngine);
            npv += bond.NPV();
        });

        return npv;
    }

    inline double priceZCB_AT_ST(size_t n) {
        // bond parameters
        AT::Date startDate              = AT::Date(1, QL::January, 2020);
        AT::Date endDate                = startDate + QL::Period(10, QL::Years);
        AT::Frequency paymentFreq       = AT::Frequency::Semiannual;
        AT::DayCounter couponDayCounter = AT::Thirty360(QL::Thirty360::BondBasis);
        double couponRate               = 0.05;
        double curveRate                = 0.06;
        double notional                 = 100.0;
        AT::InterestRate rate(couponRate, couponDayCounter);

        // market data
        AT::MarketStore<double> marketStore(startDate);
        AT::YieldTermStructure<double> curve = AT::FlatForwardTermStructure<double>(startDate, curveRate);
        marketStore.curveManager().addCurveContext("CLP", curve);
        size_t context = marketStore.curveManager().curveContext("CLP").idx();

        std::vector<AT::InstrumentVariant<double>> bonds;
        bonds.reserve(n);
        for (size_t i = 0; i < n; ++i) { bonds.push_back(AT::ZeroCouponInstrument(startDate, endDate, notional, rate, context)); }

        // indexing
        AT::IndexingVisitor<double> indexingVisitor;
        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) { std::visit(indexingVisitor, bond); });

        auto request = indexingVisitor.getResults();
        AT::SpotMarketDataModel model(request, marketStore);
        auto marketData = model.marketData(startDate);

        // pricing
        AT::NPVConstVisitor npvVisitor(marketData);

        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) { std::visit(npvVisitor, bond); });
        return npvVisitor.getResults().npv;
    }

    inline double priceBond_QL_ST(size_t n) {
        QL::Date today(1, QL::January, 2020);
        QL::Settings::instance().evaluationDate() = today;

        // bond parameters
        QL::Date startDate              = today;
        QL::Date endDate                = startDate + QL::Period(10, QL::Years);
        QL::Frequency paymentFreq       = QL::Semiannual;
        QL::DayCounter couponDayCounter = QL::Thirty360(QL::Thirty360::BondBasis);
        double couponRate               = 0.05;
        double curveRate                = 0.06;
        double notional                 = 100.0;

        std::vector<QL::FixedRateBond> bonds;
        bonds.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            QL::Schedule schedule =
                QL::MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFreq);  // inside bc might be different for each bond
            bonds.push_back(QL::FixedRateBond(0, notional, schedule, {couponRate}, couponDayCounter));
        }

        // discounting term structure
        QL::DayCounter curveDayCounter   = QL::Actual360();
        QL::Compounding curveCompounding = QL::Simple;
        boost::shared_ptr<QL::FlatForward> curvePtr(new QL::FlatForward(today, curveRate, curveDayCounter, curveCompounding));
        QL::Handle<QL::YieldTermStructure> discountingTermStructure(curvePtr);

        // pricing engine
        boost::shared_ptr<QL::PricingEngine> bondEngine(new QL::DiscountingBondEngine(discountingTermStructure));

        // calculate the npv
        double npv = 0.0;
        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) {
            bond.setPricingEngine(bondEngine);
            npv += bond.NPV();
        });

        return npv;
    }

    inline double priceBond_AT_ST(size_t n) {
        // bond parameters
        AT::Date startDate              = AT::Date(1, QL::January, 2020);
        AT::Date endDate                = startDate + QL::Period(10, QL::Years);
        AT::Frequency paymentFreq       = AT::Frequency::Semiannual;
        AT::DayCounter couponDayCounter = AT::Thirty360(QL::Thirty360::BondBasis);
        double couponRate               = 0.05;
        double curveRate                = 0.06;
        double notional                 = 100.0;
        AT::InterestRate rate(couponRate, couponDayCounter);

        // market data

        AT::MarketStore<double> marketStore(startDate);
        AT::DayCounter curveDayCounter       = QL::Actual360();
        QL::Compounding curveCompounding     = QL::Simple;
        AT::YieldTermStructure<double> curve = AT::FlatForwardTermStructure<double>(startDate, curveRate, curveDayCounter, curveCompounding);
        marketStore.curveManager().addCurveContext("CLP", curve);
        size_t context = marketStore.curveManager().curveContext("CLP").idx();

        std::vector<AT::InstrumentVariant<double>> bonds;
        bonds.reserve(n);
        for (size_t i = 0; i < n; ++i) { bonds.push_back(AT::FixedRateBulletInstrument(startDate, endDate, paymentFreq, notional, rate, context)); }

        // indexing
        AT::IndexingVisitor<double> indexingVisitor;
        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) { std::visit(indexingVisitor, bond); });

        auto request = indexingVisitor.getResults();
        AT::SpotMarketDataModel model(request, marketStore);
        auto marketData = model.marketData(startDate);

        // pricing
        AT::NPVConstVisitor npvVisitor(marketData);

        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) { std::visit(npvVisitor, bond); });
        return npvVisitor.getResults().npv;
    }

    inline double priceBond_QL_MT(size_t n) {
        QL::Date today(1, QL::January, 2020);
        QL::Settings::instance().evaluationDate() = today;

        // bond parameters
        QL::Date startDate        = today;
        QL::Date endDate          = startDate + QL::Period(10, QL::Years);
        QL::Frequency paymentFreq = QL::Semiannual;

        double couponRate = 0.05;
        double curveRate  = 0.06;
        double notional   = 100.0;

        // curves inside to reduce shared memory access
        QL::DayCounter curveDayCounter   = QL::Actual360();
        QL::Compounding curveCompounding = QL::Simple;
        boost::shared_ptr<QL::FlatForward> curvePtr(new QL::FlatForward(today, curveRate, curveDayCounter, curveCompounding));
        QL::Handle<QL::YieldTermStructure> discountingTermStructure(curvePtr);

        boost::shared_ptr<QL::PricingEngine> bondEngine(new QL::DiscountingBondEngine(discountingTermStructure));

        std::mutex mtx;
        size_t numThreads    = std::thread::hardware_concurrency();
        AT::ThreadPool* pool = AT::ThreadPool::getInstance();
        pool->start();
        std::vector<AT::TaskHandle> futures;
        double npv = 0.0;
        for (size_t i = 0; i < numThreads; ++i) {
            auto task = [&]() {
                double npv_ = 0.0;
                std::lock_guard l(mtx);

                std::vector<QL::FixedRateBond> bonds;
                bonds.reserve(n / numThreads);
                for (size_t i = 0; i < n / numThreads; ++i) {
                    QL::Schedule schedule =
                        QL::MakeSchedule().from(startDate).to(endDate).withFrequency(paymentFreq);  // inside bc might be different for each bond
                    QL::FixedRateBond bond(0, notional, schedule, {couponRate}, QL::Thirty360(QL::Thirty360::BondBasis));
                    bonds.push_back(bond);
                }
                for (auto& bond : bonds) {
                    bond.setPricingEngine(bondEngine);
                    npv_ += bond.NPV();
                }
                npv += npv_;
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();

        return npv;
    }

    inline double priceBond_AT_MT(size_t n) {
        // bond parameters
        AT::Date startDate              = AT::Date(1, QL::January, 2020);
        AT::Date endDate                = startDate + QL::Period(10, QL::Years);
        AT::Frequency paymentFreq       = AT::Frequency::Semiannual;
        double couponRate               = 0.05;
        double curveRate                = 0.06;
        double notional                 = 100.0;
        AT::DayCounter couponDayCounter = AT::Thirty360(QL::Thirty360::BondBasis);  // --> must be defined inside thread, to be fixed in future
        AT::InterestRate rate(couponRate, couponDayCounter);

        // market data
        AT::MarketStore<double> marketStore(startDate);
        AT::DayCounter curveDayCounter       = QL::Actual360();
        QL::Compounding curveCompounding     = QL::Simple;
        AT::YieldTermStructure<double> curve = AT::FlatForwardTermStructure<double>(startDate, curveRate, curveDayCounter, curveCompounding);
        marketStore.curveManager().addCurveContext("CLP", curve);
        size_t context = marketStore.curveManager().curveContext("CLP").idx();

        // MT
        size_t numThreads = std::thread::hardware_concurrency();
        // size_t numThreads = 2;
        AT::ThreadPool* pool = AT::ThreadPool::getInstance();
        pool->start();
        std::vector<AT::TaskHandle> futures;
        double npv = 0.0;
        for (size_t i = 0; i < numThreads; ++i) {
            auto task = [&]() {
                AT::MarketStore localStore = marketStore;

                // building
                std::vector<AT::InstrumentVariant<double>> slice;
                slice.reserve(n / numThreads);
                for (size_t j = 0; j < n / numThreads; ++j) {
                    slice.push_back(AT::FixedRateBulletInstrument(startDate, endDate, paymentFreq, notional, rate, context));
                }

                // indexing
                AT::IndexingVisitor indexingVisitor;
                for (auto& bond : slice) { std::visit(indexingVisitor, bond); }
                auto request = indexingVisitor.getResults();

                AT::SpotMarketDataModel model(request, localStore);
                auto marketData = model.marketData(startDate);

                // pricing
                AT::NPVConstVisitor npvVisitor(marketData);

                for (auto& bond : slice) { std::visit(npvVisitor, bond); }
                npv += npvVisitor.getResults().npv;
                return true;
            };
            futures.push_back(pool->spawnTask(task));
        }
        for (auto& future : futures) { pool->activeWait(future); }
        pool->stop();

        return npv;
    }

}  // namespace PricingBenchmark

#endif /* E33FF0E7_2A2F_48F6_9352_1F04A5DA0613 */
