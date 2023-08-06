#ifndef FC966EC1_F615_4EDF_A10C_943615A4081E
#define FC966EC1_F615_4EDF_A10C_943615A4081E

#include "shared.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/indexingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>
#include <atlas/visitors/parrateconstvisitor.hpp>
#include <atlas/visitors/sensitivityconstvisitor.hpp>

namespace PricingExample {

    void fixedRateProductExample() {
        // Create a fixed rate instrument
        printCenteredBlock("Fixed Rate Bullet Instrument");

        Date startDate             = Date(1, Month::Aug, 2020);
        Date endDate               = Date(1, Month::Aug, 2025);
        Frequency paymentFrequency = Frequency::Semiannual;
        double notional            = 100;
        double r                   = 0.05;
        Side side                  = Side::Recieve;
        InterestRate<double> rate  = InterestRate<double>(r, Actual360(), Compounding::Compounded, Frequency::Annual);

        MarketStore<double> store                             = createStore(startDate);
        const YieldTermStructureManager<double>& curveManager = store.curveManager();
        CurveContext context                                  = curveManager.curveContext("ExampleCurve");
        size_t idx                                            = context.idx();

        InstrumentVariant<double> instrument = FixedRateBulletInstrument<double>(startDate, endDate, paymentFrequency, notional, rate, idx, side);

        IndexingVisitor<double> indexVisitor;
        std::visit(indexVisitor, instrument);
        MarketRequest request = indexVisitor.getResults();

        SpotMarketDataModel<double> model(request, store);
        MarketData<double> marketData = model.marketData(startDate);

        NPVConstVisitor<double> npvVisitor(marketData);
        std::visit(npvVisitor, instrument);
        printLine("NPV", npvVisitor.getResults().npv, 20);

        ParRateConstVisitor<double> parVisitor(marketData);
        std::visit(parVisitor, instrument);
        printLine("Par Rate", parVisitor.getResults().parRate, 20);

        SensitivityConstVisitor<double> sensitivityVisitor(marketData);
        std::visit(sensitivityVisitor, instrument);
        printLine("Sensitivity", sensitivityVisitor.getResults().couponSens, 20);
    };

}  // namespace PricingExample

#endif /* FC966EC1_F615_4EDF_A10C_943615A4081E */
