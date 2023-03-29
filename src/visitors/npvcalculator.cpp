#include <atlas/visitors/npvcalculator.hpp>

namespace Atlas {

    void NPVCalculator::visit(FixedRateInstrument& inst) {
        fixedLegNPV(inst.leg());
        redemptionsNPV(inst.leg());
    };

    void NPVCalculator::visit(FloatingRateInstrument& inst) {
        floatingLegNPV(inst.leg());
        redemptionsNPV(inst.leg());
    };

    void NPVCalculator::redemptionsNPV(const Leg& leg) {
        double npv = 0.0;
        double df  = 0.0;
        for (const auto& redemption : leg.constRedemptions()) {
            df = marketData_.dfs.at(redemption.dfIdx());
            npv += Value(redemption.amount()) * df;
        }
        nonSensNPV_ += npv;
    };

    // missing numeraires
    void NPVCalculator::fixedLegNPV(const FixedRateLeg& leg) {
        adouble npv = 0.0;
        double df   = 0.0;
        for (auto& coupon : leg.constCoupons()) {
            df = marketData_.dfs.at(coupon.dfIdx());
            npv += coupon.amount() * df;
        }
        npv_ += npv;
    };

    void NPVCalculator::floatingLegNPV(FloatingRateLeg& leg) {
        adouble npv = 0.0;
        double df   = 0.0;
        double fwd  = 0.0;
        for (auto& coupon : leg.coupons()) {
            df  = marketData_.dfs.at(coupon.dfIdx());
            fwd = marketData_.fwds.at(coupon.fwdIdx());
            coupon.fixing(fwd);  // ?
            npv += coupon.amount() * df;
        }
        npv_ += npv;
    };

}  // namespace Atlas