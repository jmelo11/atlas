#ifndef C2882491_9882_4D56_B587_DCDE8ED50E68
#define C2882491_9882_4D56_B587_DCDE8ED50E68

#include <atlas/fundation/marketstore.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

namespace MarketStoreExample {
    using namespace Atlas;

    inline void setupExample() {
        double curveRate = 0.05;
        Date refDate     = Date(1, Month::Jan, 2020);
        FlatForwardTermStructure usdCurve(refDate, 0.05);
        InterestRateIndex usdIndex(Frequency::Annual);
        FlatForwardTermStructure clpCurve(refDate, 0.01);
        FlatForwardTermStructure clfCurve(refDate, 0.03);

        MarketStore marketStore(refDate);

        auto& curveManager = marketStore.curveManager();
        curveManager.addCurveContext("A USD curve", usdCurve, usdIndex, USD());
        curveManager.addCurveContext("A CLP curve", clpCurve, InterestRateIndex(), CLP(), true);
        curveManager.addCurveContext("A CLF curve", clfCurve, InterestRateIndex(), CLF(), false);
        curveManager.summary();

        auto curveContext = curveManager.curveContext(0);
        auto rateDef      = curveContext.index().rateDefinition();

        auto& fxManager = marketStore.fxManager();
        fxManager.addExchangeRate(CLP(), USD(), 800);
        fxManager.addExchangeRate(CLP(), CLF(), 30000);
        fxManager.exchangeRate(USD(), CLF());
        fxManager.summary();
    }
}  // namespace MarketStoreExample

#endif /* C2882491_9882_4D56_B587_DCDE8ED50E68 */
