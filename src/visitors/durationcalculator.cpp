#include <atlas/models/staticyieldmodel.hpp>
#include <atlas/visitors/durationcalculator.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>

namespace Atlas {

    DurationCalculator::DurationCalculator(const MarketData& marketData, double delta) : marketData_(marketData), delta_(delta){};

    void DurationCalculator::visit(const FixedRateInstrument& inst) const {
        fixedInstSens(inst);
    };

    void DurationCalculator::visit(const FloatingRateInstrument& inst) const {
        floatingInstSens(inst);
    };

}  // namespace Atlas