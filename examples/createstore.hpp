#ifndef A1D27773_FEA8_41FB_ACB4_FA651854C8F7
#define A1D27773_FEA8_41FB_ACB4_FA651854C8F7

#include <atlas/fundation/marketstore.hpp>
#include <atlas/rates/yieldtermstructure/flatforwardcurve.hpp>

using namespace Atlas;
using NumType = double;

static MarketStore<NumType> createCurves(const Date& refDate) {
    MarketStore<NumType> store(refDate);
    NumType curveRate = 0.03;
    // create atlas curves
    YieldTermStructure<NumType> curve(std::make_unique<FlatForwardStrategy<NumType>>(refDate, curveRate));
    RateIndex<NumType> index(refDate, Frequency::Semiannual);
    store.addCurve("TEST", curve, index);
    return store;
}

#endif /* A1D27773_FEA8_41FB_ACB4_FA651854C8F7 */
