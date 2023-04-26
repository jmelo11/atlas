#ifndef F1B69B4F_EFCB_409A_87B1_B6A303F217AB
#define F1B69B4F_EFCB_409A_87B1_B6A303F217AB

#include "createstore.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>

void currencyConversion() {
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2025);
    double notional = 1;  // CLP
    NumType r       = 0.03;

    Frequency paymentFrequency = Frequency::Semiannual;
    InterestRate<NumType> rate(r, Actual360(), Compounding::Compounded, Frequency::Annual);

    // Create a curve context store
    MarketStore<NumType> store_ = createCurves(startDate);
    store_.addExchangeRate(CLP(), USD(), 800);

    // by default, the instrument is set to be in the local currency, whatever it is (assigns 0 as the currency code)
    FixedRateBulletInstrument<NumType> instrument(startDate, endDate, paymentFrequency, notional, rate, store_.curveContext("TEST"));
    instrument.disbursement().currency(USD());

    // indexing and request is independent of the local currency
    Indexer<NumType> indexer;
    indexer.visit(instrument);
    MarketRequest request = indexer.request();

    // price as if CLP is the local currency (no conversion)
    store_.localCurrency(CLP());
    SpotMarketDataModel<NumType> model(request, store_);
    MarketData<NumType> marketData = model.marketData();
    NPVCalculator<NumType> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    std::cout << "NPV: " << npvCalculator.results() << std::endl;

    // price as if USD is the local currency (conversion)
    store_.localCurrency(USD());
    SpotMarketDataModel<NumType> model2(request, store_);
    MarketData<NumType> marketData2 = model2.marketData();
    NPVCalculator<NumType> npvCalculator2(marketData2);
    npvCalculator2.visit(instrument);

    std::cout << "NPV: " << npvCalculator2.results() << std::endl;
}

#endif /* F1B69B4F_EFCB_409A_87B1_B6A303F217AB */
