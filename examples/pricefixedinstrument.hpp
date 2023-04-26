#ifndef FC966EC1_F615_4EDF_A10C_943615A4081E
#define FC966EC1_F615_4EDF_A10C_943615A4081E

#include "createstore.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>

using namespace Atlas;

void pricingFixedRateInstruments() {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    NumType r                  = 0.03;
    InterestRate<NumType> rate(r, Actual360(), Compounding::Compounded, Frequency::Annual);
    MarketStore<NumType> store_ = createCurves(startDate);

    FixedRateBulletInstrument<NumType> instrument(startDate, endDate, paymentFrequency, notional, rate, store_.curveContext("TEST"));

    // Create a curve context store

    Indexer<NumType> indexer;
    indexer.visit(instrument);
    MarketRequest request = indexer.request();
    SpotMarketDataModel<NumType> model(request, store_);
    MarketData<NumType> marketData = model.marketData();
    NPVCalculator<NumType> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
};

#endif /* FC966EC1_F615_4EDF_A10C_943615A4081E */
