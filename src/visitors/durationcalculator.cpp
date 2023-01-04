#include <atlas/models/staticyieldmodel.hpp>
#include <atlas/visitors/durationcalculator.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>

namespace Atlas {

    DurationCalculator::DurationCalculator(const MarketData& marketData, double delta) : marketData_(marketData), delta_(delta){};

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