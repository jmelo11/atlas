
#include <algorithm>
#include <atlas/visitors/forwardrateforecaster.hpp>

namespace Atlas {

    void ForwardRateForecaster::visit(FloatingRateInstrument& inst) {
        auto& leg = inst.leg();
        for (auto& coupon : leg.coupons()) {
            double fwd = marketData_.fwds.at(coupon.fwdIdx());
            coupon.fixing(fwd);
        }
    }

}  // namespace Atlas