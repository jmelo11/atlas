#include <atlas/visitors/npvcalculator.hpp>

namespace Atlas {

    void NPVCalculator::visit(Deposit& inst) {
        fixedLegNPV(inst.leg());
        redemptionsNPV(inst.leg());
    };

    void NPVCalculator::visit(FixedBulletProduct& inst) {
        fixedLegNPV(inst.leg());
        redemptionsNPV(inst.leg());
    };

    void NPVCalculator::visit(EqualPaymentProduct& inst) {
        fixedLegNPV(inst.leg());
        redemptionsNPV(inst.leg());
    };

    void NPVCalculator::visit(FloatingRateBulletProduct& inst) {
        floatingLegNPV(inst.leg());
        redemptionsNPV(inst.leg());
    };

    void NPVCalculator::redemptionsNPV(const Leg& leg) {
        double npv = 0.0;
        double df  = 0.0;
        for (const auto& redemption : leg.constRedemptions()) {
            df = marketData_.dfs.at(redemption.dfIdx());
            npv += redemption.amount() * df;
        }
        nonSensNPV_ += npv;
    };

    void NPVCalculator::fixedLegNPV(const FixedRateLeg& leg) {
        double npv = 0.0;
        double df  = 0.0;
        for (auto& coupon : leg.constCoupons()) {
            df = marketData_.dfs.at(coupon.dfIdx());
            npv += coupon.amount() * df;
        }
        npv_ += npv;
    };

    void NPVCalculator::floatingLegNPV(FloatingRateLeg& leg) {
        double npv = 0.0;
        double df  = 0.0;
        double fwd = 0.0;
        for (auto& coupon : leg.coupons()) {
            df  = marketData_.dfs.at(coupon.dfIdx());
            fwd = marketData_.fwds.at(coupon.fwdIdx());
            coupon.fixing(fwd);
            npv += coupon.amount() * df;
        }
        npv_ += npv;
    };

}  // namespace Atlas