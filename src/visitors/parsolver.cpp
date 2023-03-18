
#include <atlas/visitors/parsolver.hpp>

namespace Atlas {

    void ParSolver::visit(const FixedRateInstrument& inst) const {
        evalFixedRateProd(inst);
    };

    void ParSolver::visit(const EqualPaymentInstrument& inst) const {
        const auto& leg                             = inst.constLeg();
        const std::vector<FixedRateCoupon>& coupons = leg.constCoupons();
        double startDf                              = marketData_.dfs.at(inst.disbursement().dfIdx());
        InterestRate prodRate                       = inst.rate();
        double dfSum                                = 0;
        for (const auto& coupon : coupons) dfSum += marketData_.dfs.at(coupon.dfIdx());
        double payment = startDf / dfSum;

        auto f = [&](double r) {
            InterestRate tmpRate(r, prodRate.dayCounter(), prodRate.compounding(), prodRate.frequency());
            double totalRedemption = 0;
            for (const auto& coupon : coupons)
                totalRedemption += payment - (tmpRate.compoundFactor(coupon.startDate(), coupon.endDate()) - 1) * (1 - totalRedemption);
            return 1 - totalRedemption;
        };

        value_ = solver_.solve(f, accuracy_, guess_, step_);
        // evalFixedRateProd(inst);
    };

    void ParSolver::visit(const FloatingRateInstrument& inst) const {
        evalFloatingRateProd(inst);
    };

}  // namespace Atlas