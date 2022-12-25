
#include <algorithm>
#include <atlas/visitors/forwardrateforecaster.hpp>

namespace Atlas {

    void ForwardRateForecaster::visit(Deposit& inst) {}

    void ForwardRateForecaster::visit(FixedRateBulletProduct& inst) {}

    void ForwardRateForecaster::visit(EqualPaymentProduct& inst) {}

    void ForwardRateForecaster::visit(FloatingRateBulletProduct& inst) {
        fixFloatingCoupons(inst.leg());
    }

    void ForwardRateForecaster::visit(CustomFloatingRateProduct& inst) {
        fixFloatingCoupons(inst.leg());
    }

    void ForwardRateForecaster::visit(CustomFixedRateProduct& inst) {}

    void ForwardRateForecaster::fixFloatingCoupons(FloatingRateLeg& leg) {
        auto f = [&](auto& coupon) {
            double fwd = marketData_.fwds.at(coupon.fwdIdx());
            coupon.fixing(fwd);
        };

        std::for_each(leg.coupons().begin(), leg.coupons().end(), f);
    }

}  // namespace Atlas