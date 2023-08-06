#ifndef DA3284C0_4C94_4B70_97F6_A2F66C72CE93
#define DA3284C0_4C94_4B70_97F6_A2F66C72CE93

// AT
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>
#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>

// MT
#include <atlas/multithreading/threadpool.hpp>

namespace PricingBenchmark {
    inline double priceFloatingBond_QL_ST(size_t n) {
        return 0.0;
    }

    inline double priceFloatingBond_AT_ST(size_t n) {
        // bond parameters
        AT::Date startDate = AT::Date(1, QL::January, 2020);
        AT::Date endDate   = startDate + QL::Period(10, QL::Years);
        double couponRate  = 0.05;
        double curveRate   = 0.06;
        double notional    = 100.0;
        double spread      = 0.01;

        // market data
        AT::MarketStore<double> marketStore(startDate);
        AT::FlatForwardTermStructure<double> curve(startDate, curveRate, AT::Actual360(), AT::Compounding::Simple);
        AT::InterestRateIndex<double> index(AT::Frequency::Semiannual);

        size_t idx = marketStore.curveManager().addCurveContext("CLP", curve, index);

        std::vector<AT::InstrumentVariant<double>> bonds;
        bonds.reserve(n);
        for (size_t i = 0; i < n; ++i) { bonds.push_back(AT::FloatingRateBulletInstrument(startDate, endDate, notional, spread, index, idx, idx)); }

        // indexing
        AT::IndexingVisitor<double> indexingVisitor;
        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) { std::visit(indexingVisitor, bond); });

        auto request = indexingVisitor.getResults();
        AT::SpotMarketDataModel model(request, marketStore);
        auto marketData = model.marketData(startDate);

        // pricing
        AT::FixingVisitor fixingVisitor(marketData);
        AT::NPVConstVisitor npvVisitor(marketData);

        std::for_each(bonds.begin(), bonds.end(), [&](auto& bond) {
            std::visit(fixingVisitor, bond);
            std::visit(npvVisitor, bond);
        });
        return npvVisitor.getResults().npv;
    }

    inline double priceFloatingBond_QL_MT(size_t n) {
        return 0.0;
    }

    inline double priceFloatingBond_AT_MT(size_t n) {
        // bond parameters
        AT::Date startDate = AT::Date(1, QL::January, 2020);
        AT::Date endDate   = startDate + QL::Period(10, QL::Years);
        double couponRate  = 0.05;
        double curveRate   = 0.06;
        double notional    = 100.0;
        double spread      = 0.01;

        // market data
        AT::MarketStore<double> marketStore(startDate);
        AT::FlatForwardTermStructure<double> curve(startDate, curveRate, AT::Actual360(), AT::Compounding::Simple);
        AT::InterestRateIndex<double> index(AT::Frequency::Semiannual);

        size_t idx = marketStore.curveManager().addCurveContext("CLP", curve, index);

        size_t numThreads = std::thread::hardware_concurrency();
        // size_t numThreads    = 1;
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
                for (size_t i = 0; i < n / numThreads; ++i) {
                    slice.push_back(AT::FloatingRateBulletInstrument(startDate, endDate, notional, spread, index, idx, idx));
                }

                // indexing
                AT::IndexingVisitor indexingVisitor;
                for (auto& bond : slice) { std::visit(indexingVisitor, bond); }
                auto request = indexingVisitor.getResults();

                AT::SpotMarketDataModel model(request, localStore);
                auto marketData = model.marketData(startDate);

                // pricing
                AT::FixingVisitor fixingVisitor(marketData);
                AT::NPVConstVisitor npvVisitor(marketData);
                for (auto& bond : slice) {
                    std::visit(fixingVisitor, bond);
                    std::visit(npvVisitor, bond);
                }
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

#endif /* DA3284C0_4C94_4B70_97F6_A2F66C72CE93 */
