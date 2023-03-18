
#include <atlas/visitors/cashflowprofiler.hpp>

namespace Atlas {
    void CashflowProfiler::visit(const FloatingRateInstrument& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const FixedRateInstrument& inst) const {
        agreggate(inst.constLeg());
    }

}  // namespace Atlas