#ifndef D4200363_5413_450D_8BD5_105CDD2B946A
#define D4200363_5413_450D_8BD5_105CDD2B946A

#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>

void sensCalculation() {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2025);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    dual r                     = 0.03;

    tape_type tape;
    
    MarketStore<dual> store(startDate);
    dual curveRate = 0.03;
    tape.registerInput(curveRate);
    tape.newRecording();
    // create atlas curves
    YieldTermStructure<dual> curve(std::make_unique<FlatForwardStrategy<dual>>(startDate, curveRate));
    RateIndex<dual> index(startDate, Frequency::Semiannual);
    store.addCurve("TEST", curve, index);

    InterestRate<dual> rate = InterestRate(r, Actual360(), Compounding::Simple, Frequency::Annual);
    FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate, store.curveContext("TEST"));
    // set curve context
    Indexer<dual> indexer;
    indexer.visit(instrument);
    MarketRequest request = indexer.request();
    SpotMarketDataModel<dual> model(request, store);
    MarketData<dual> marketData = model.marketData();
    NPVCalculator<dual> npvCalculator(marketData);
    npvCalculator.visit(instrument);

    dual npv = 0.0;
    tape.registerOutput(npv);
    npv = npvCalculator.results();

    npv.setDerivative(0.0001);
    tape.computeAdjoints();

    std::cout << "Sens: " << curveRate.getDerivative() << std::endl;
};

#endif /* D4200363_5413_450D_8BD5_105CDD2B946A */
