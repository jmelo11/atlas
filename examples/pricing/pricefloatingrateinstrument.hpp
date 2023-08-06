#ifndef B636D092_3D0A_46F6_BC48_AD96A9BDBAA8
#define B636D092_3D0A_46F6_BC48_AD96A9BDBAA8

#include "shared.hpp"

namespace PricingExample {

    inline void floatingRateProductExample() {
        printCenteredBlock("Floating Rate Bullet Instrument");

        // bond parameters
        Date startDate        = Date(1, Month::January, 2020);
        Date endDate          = startDate + Period(10, TimeUnit::Years);
        Frequency paymentFreq = Frequency::Semiannual;
        double couponRate     = 0.05;
        double curveRate      = 0.06;
        double notional       = 100.0;
        double spread         = 0.01;

        MarketStore<double> store                             = createStore(startDate);
        const YieldTermStructureManager<double>& curveManager = store.curveManager();
        CurveContext forecastContext                          = curveManager.curveContext("ForecastCurve");
        CurveContext discountContext                          = curveManager.curveContext("DiscountCurve");
        size_t discountContextIdx                             = discountContext.idx();
        size_t forecastContextIdx                             = forecastContext.idx();
        InterestRateIndex index                               = forecastContext.index();

        InstrumentVariant<double> instrument = FloatingRateBulletInstrument(startDate, endDate, notional, spread, index, discountContextIdx, forecastContextIdx);

        // indexing
        IndexingVisitor<double> indexingVisitor;
        std::visit(indexingVisitor, instrument);
        MarketRequest request = indexingVisitor.getResults();

        SpotMarketDataModel model(request, store);
        MarketData marketData = model.marketData(startDate);

        // pricing
        FixingVisitor fixingVisitor(marketData);
        NPVConstVisitor npvVisitor(marketData);

        std::visit(fixingVisitor, instrument);
        std::visit(npvVisitor, instrument);

        printLine("NPV", npvVisitor.getResults().npv, 20);

        ParRateConstVisitor<double> parVisitor(marketData);
        std::visit(parVisitor, instrument);
        printLine("Par Spread", parVisitor.getResults().parSpread, 20);

        SensitivityConstVisitor<double> sensitivityVisitor(marketData);
        std::visit(sensitivityVisitor, instrument);
        printLine("Sensitivity", sensitivityVisitor.getResults().spreadSens, 20);

        InstrumentCashflowsConstVisitor<double> cashflowVisitor;
        std::visit(cashflowVisitor, instrument);
        auto results = cashflowVisitor.getResults();

        std::cout << std::endl;
        printTable(results);
    }
}  // namespace PricingExample

#endif /* B636D092_3D0A_46F6_BC48_AD96A9BDBAA8 */
