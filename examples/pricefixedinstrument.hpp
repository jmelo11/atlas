#ifndef FC966EC1_F615_4EDF_A10C_943615A4081E
#define FC966EC1_F615_4EDF_A10C_943615A4081E

#include "createstore.hpp"
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/models/spotmarketdatamodel.hpp>
#include <atlas/visitors/cashflowprofiler.hpp>
#include <atlas/visitors/newvisitors/indexingvisitor.hpp>
#include <atlas/visitors/newvisitors/npvconstvisitor.hpp>
#include <atlas/visitors/newvisitors/parrateconstvisitor.hpp>

using namespace Atlas;

void printCenteredBlock(const std::string& text, int blockWidth = 40) {
    int padding = blockWidth - text.size();
    if (padding < 0) { padding = 0; }
    int paddingLeft  = padding / 2;
    int paddingRight = padding - paddingLeft;

    std::cout << std::string(blockWidth, '=') << '\n';
    std::cout << std::string(paddingLeft, ' ') << text << std::string(paddingRight, ' ') << '\n';
    std::cout << std::string(blockWidth, '=') << '\n';
}
void printLine(const std::string& key, double value, int keyWidth) {
    const std::string sep = ":";
    std::cout << std::left << std::setw(keyWidth) << key << sep << value << '\n';
}

void productEvaluationExample() {
    // Create a fixed rate instrument
    printCenteredBlock("Fixed Rate Bullet Instrument");

    Date startDate              = Date(1, Month::Aug, 2020);
    Date endDate                = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency  = Frequency::Semiannual;
    double notional             = 100;
    NumType r                   = 0.03;
    Side side                   = Side::Short;
    InterestRate<NumType> rate  = InterestRate<NumType>(r, Actual360(), Compounding::Compounded, Frequency::Annual);
    MarketStore<NumType> store_ = createCurves(startDate);

    FixedRateBulletInstrument<NumType> instrument(startDate, endDate, paymentFrequency, notional, rate, store_.curveContext("TEST"), side);
    InstrumentVariant<NumType> instrumentVariant(instrument);

    IndexingVisitor<NumType> indexVisitor;
    std::visit(indexVisitor, instrumentVariant);
    MarketRequest request = indexVisitor.getResults();

    SpotMarketDataModel<NumType> model(request, store_);
    MarketData<NumType> marketData = model.marketData();

    NPVConstVisitor<NumType> npvVisitor(marketData);
    std::visit(npvVisitor, instrumentVariant);
    printLine("NPV", npvVisitor.getResults(), 20);

    ParRateConstVisitor<NumType> parVisitor(marketData);
    std::visit(parVisitor, instrumentVariant);
    printLine("Par Rate", parVisitor.getResults()["rate"], 20);

    CashflowProfiler<NumType> profileVisitor;
    profileVisitor.visit(instrument);
    auto interest    = profileVisitor.interests();
    auto redemptions = profileVisitor.redemptions();
    printCenteredBlock("Payments");
    for (auto& i : interest) {
        std::cout << i.first << " : " << i.second << '\n';
    }
};

#endif /* FC966EC1_F615_4EDF_A10C_943615A4081E */
