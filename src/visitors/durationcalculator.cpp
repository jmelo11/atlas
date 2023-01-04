#include <atlas/models/staticyieldmodel.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>
#include <atlas/visitors/durationcalculator.hpp>

namespace Atlas {

    DurationCalculator::DurationCalculator(const QuantLib::Date& refDate, const MarketData& marketData, double delta)
    : refDate_(refDate), marketData_(marketData), delta_(delta){};

    void DurationCalculator::visit(const Deposit& inst) const {
        calculateFixedProductSens(inst);
    };

    void DurationCalculator::visit(const FixedRateBulletProduct& inst) const {
        calculateFixedProductSens(inst);
    };

    void DurationCalculator::visit(const EqualPaymentProduct& inst) const {
        calculateFixedProductSens(inst);
    };

    void DurationCalculator::visit(const FloatingRateBulletProduct& inst) const {
        calculateFloatingProductSens(inst);
    };

    void DurationCalculator::visit(const CustomFloatingRateProduct& inst) const {
        calculateFloatingProductSens(inst);
    };

    void DurationCalculator::visit(const CustomFixedRateProduct& inst) const {
        calculateFixedProductSens(inst);
    };

}  // namespace Atlas