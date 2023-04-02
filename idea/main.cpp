
#include <ql/termstructures/yield/flatforward.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/models/staticcurvemodel.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <cppad/cppad.hpp>
#include <iostream>
#include <numeric>

using namespace Atlas;

using NumType = double;

void pricingFixedRateInstruments() {
    // Create a fixed rate instrument
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2021);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    NumType r                  = 0.03;
    InterestRate<NumType> rate(r, Actual360(), Compounding::Compounded, Frequency::Annual);
    FixedRateBulletInstrument<NumType> instrument(startDate, endDate, paymentFrequency, notional, rate);

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    FlatForwardStrategy curveStrategy(startDate, 0.03, Actual360(), Compounding::Simple, Frequency::Annual);
    RateIndex index("TEST", Frequency::Annual, Actual360());
    store_.createCurveContext("TEST", curveStrategy, index);

    // set curve context
    instrument.discountCurveContex(store_.at("TEST"));

    Indexer<NumType> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<NumType> model(request);

    MarketData<NumType> marketData = model.simulate(startDate);

    NPVCalculator<NumType> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

void pricingFloatingRateInstruments() {
    // Create a fixed rate instrument
    Date startDate  = Date(1, Month::Aug, 2020);
    Date endDate    = Date(1, Month::Aug, 2021);
    double notional = 100;
    double spread   = 0.03;

    // Create a curve context store
    CurveContextStore& store_ = CurveContextStore::instance();
    const auto& context       = store_.at("TEST");

    FloatingRateBulletInstrument<double> instrument(startDate, endDate, notional, spread, context);
    // set curve context
    instrument.discountCurveContex(context);

    Indexer<double> indexer;
    indexer.visit(instrument);

    MarketRequest request;
    indexer.setRequest(request);

    StaticCurveModel<double> model(request);

    MarketData<double> marketData = model.simulate(startDate);

    NPVCalculator<double> npvCalculator(marketData);
    npvCalculator.visit(instrument);
    std::cout << "NPV: " << npvCalculator.results() << std::endl;
}

void newtonRaphson() {
    auto f      = [](const dual& x) { return pow(x * x - 6 - 5, 2); };
    auto newton = [](std::function<dual(dual)> f, double x0, double tol = 1e-6, int max_iter = 100) {
        dual x = x0;
        dual y = f(x);
        for (int i = 0; i < max_iter; ++i) {
            if (abs(y) < tol) { break; }
            double dfdx = derivative(f, wrt(x), at(x));
            x           = x - y / dfdx;
            y           = f(x);
        }
        return x;
    };
    std::cout << "x: " << newton(f, 5.0) << std::endl;
}

void rateSens() {
    Date startDate             = Date(1, Month::Aug, 2020);
    Date endDate               = Date(1, Month::Aug, 2025);
    Frequency paymentFrequency = Frequency::Semiannual;
    double notional            = 100;
    dual r                     = 0.03;

    CurveContextStore& store_ = CurveContextStore::instance();

    auto f = [&](const dual& r) {
        InterestRate<dual> rate = InterestRate(r, Actual360(), Compounding::Simple, Frequency::Annual);
        FixedRateBulletInstrument<dual> instrument(startDate, endDate, paymentFrequency, notional, rate);
        // set curve context
        instrument.discountCurveContex(store_.at("TEST"));
        Indexer<dual> indexer;
        indexer.visit(instrument);
        MarketRequest request;
        indexer.setRequest(request);
        StaticCurveModel<dual> model(request);
        MarketData<dual> marketData = model.simulate(instrument.startDate());

        instrument.rate(r);
        NPVCalculator<dual> npvCalculator(marketData);
        npvCalculator.visit(instrument);
        return npvCalculator.results();
    };

    dual tol  = 1e-6;
    dual npv  = f(r);
    dual npv2 = f(r + tol);
    dual sens = derivative(f, wrt(r), at(r)) * 0.01;

    std::cout << "NPV: " << npv << std::endl;
    std::cout << "NPV2: " << npv2 << std::endl;
    std::cout << "NPV2-NPV: " << npv2.val - npv.val << std::endl;
    std::cout << "Sens: " << sens << std::endl;
}

int main() {
    pricingFixedRateInstruments();
    pricingFloatingRateInstruments();
    newtonRaphson();
    rateSens();
    return 0;
}