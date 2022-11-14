
#include <atlas/visitors/ratesolver.hpp>

namespace Atlas {

    void ParSolver::visit(const Deposit& inst) const {
        evalFixedRateProd(inst);
    };

    void ParSolver::visit(const FixedBulletProduct& inst) const {

    };

    void ParSolver::visit(const EqualPaymentProduct& inst) const {

    };

    void ParSolver::visit(const FloatingRateBulletProduct& inst) const {

    };

}  // namespace Atlas