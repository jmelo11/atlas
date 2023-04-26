#ifndef FF5A209D_940E_4E09_893E_310DC53D8AE7
#define FF5A209D_940E_4E09_893E_310DC53D8AE7

#include "createstore.hpp"
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>

void pricingFloatingRateInstruments() {
    // Create a fixed rate instrument
    Date startDate    = Date(1, Month::Aug, 2020);
    Date endDate      = Date(1, Month::Aug, 2021);
    double notional   = 100;
    NumType spread    = 0.03;
    NumType rateValue = 0.03;
    // Create a curve context store
    MarketStore<NumType> store_ = createCurves(startDate);

    FloatingRateBulletInstrument<NumType> instrument(startDate, endDate, notional, spread, store_.rateIndexContext("TEST"),
                                                     store_.curveContext("TEST"));

    Indexer<NumType> indexer;
    indexer.visit(instrument);
    MarketRequest request = indexer.request();
    SpotMarketDataModel<NumType> model(request, store_);
    MarketData<NumType> marketData = model.marketData();
    NPVCalculator<NumType> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
};

#endif /* FF5A209D_940E_4E09_893E_310DC53D8AE7 */
