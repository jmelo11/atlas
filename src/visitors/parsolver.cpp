
#include <atlas/visitors/parsolver.hpp>

namespace Atlas {

    void ParSolver::visit(const Deposit& inst) const {
        evalFixedRateProd(inst);
    };

    void ParSolver::visit(const FixedRateBulletProduct& inst) const {
        evalFixedRateProd(inst);
    };

    void ParSolver::visit(const EqualPaymentProduct& inst) const {
        const auto& leg                             = inst.constLeg();
        const std::vector<FixedRateCoupon>& coupons = leg.constCoupons();
        double startDf                              = marketData_.dfs.at(leg.dfIdx());
        QuantLib::InterestRate prodRate             = inst.rate();
        double dfSum                                = 0;
        for (const auto& coupon : coupons) dfSum += marketData_.dfs.at(coupon.dfIdx());
        double payment = startDf / dfSum;

        auto f = [&](double r) {
            QuantLib::InterestRate tmpRate(r, prodRate.dayCounter(), prodRate.compounding(), prodRate.frequency());
            double totalRedemption = 0;
            for (const auto& coupon : coupons)
                totalRedemption += payment - (tmpRate.compoundFactor(coupon.startDate(), coupon.endDate()) - 1) * (1 - totalRedemption);
            return startDf - totalRedemption;
        };

        value_ = solver_.solve(f, accuracy_, guess_, step_);
    };

    void ParSolver::visit(const FloatingRateBulletProduct& inst) const {
        evalFloatingRateProd(inst);
    };

    void ParSolver::visit(const CustomFloatingRateProduct& inst) const {
        evalFloatingRateProd(inst);
    };

    void ParSolver::visit(const CustomFixedRateProduct& inst) const {
        evalFixedRateProd(inst);
    };

}  // namespace Atlas