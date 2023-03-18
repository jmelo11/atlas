
#include <algorithm>
#include <atlas/visitors/forwardrateforecaster.hpp>

namespace Atlas {

    void ForwardRateForecaster::visit(FloatingRateInstrument& inst) {
        auto& leg = inst.leg();
        for (auto& coupon : leg.coupons()) { fixFloatingCoupon(coupon); }
    }

    void ForwardRateForecaster::fixFloatingCoupon(FloatingRateCoupon& coupon) {
        double fwd = marketData_.fwds.at(coupon.fwdIdx());
        coupon.fixing(fwd);
    }

}  // namespace Atlas