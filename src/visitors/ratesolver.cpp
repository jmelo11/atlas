
#include <atlas/visitors/ratesolver.hpp>

namespace Atlas {

    void ParSolver::visit(const Deposit& inst) const {
        evalFixedRateProd(inst);
    };

    void ParSolver::visit(const FixedBulletProduct& inst) const {
        evalFixedRateProd(inst);
    };

    void ParSolver::visit(const EqualPaymentProduct& inst) const {
        evalFixedRateProd(inst);
    };

    void ParSolver::visit(const FloatingRateBulletProduct& inst) const {
        evalFloatingRateProd(inst);
    };

}  // namespace Atlas