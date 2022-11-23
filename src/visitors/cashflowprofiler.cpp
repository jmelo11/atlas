
#include <atlas/visitors/cashflowprofiler.hpp>

namespace Atlas {
    void CashflowProfiler::visit(const Deposit& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const FixedBulletProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const EqualPaymentProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const FloatingRateBulletProduct& inst) const {
        agreggate(inst.constLeg());
    }

}  // namespace Atlas