
#include <atlas/visitors/cashflowprofiler.hpp>

namespace Atlas {
    void CashflowProfiler::visit(const Deposit& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const FixedRateBulletProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const EqualPaymentProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const FixedRateEqualRedemptionProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const FloatingRateBulletProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const FloatingRateEqualRedemptionProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const CustomFixedRateProduct& inst) const {
        agreggate(inst.constLeg());
    }

    void CashflowProfiler::visit(const CustomFloatingRateProduct& inst) const {
        agreggate(inst.constLeg());
    }

}  // namespace Atlas