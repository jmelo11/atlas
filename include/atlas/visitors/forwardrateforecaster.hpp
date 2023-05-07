#ifndef B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31
#define B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/derivatives/fixfloatswap.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    template <typename adouble>
    class ForwardRateForecaster : public Visitor<adouble> {
       public:
        ForwardRateForecaster(const MarketData<adouble>& marketData) : marketData_(marketData){};

        void visit(FloatingRateInstrument<adouble>& inst) override {
            auto& leg = inst.leg();
            for (auto& coupon : leg.coupons()) {
                adouble fwd = marketData_.fwds.at(coupon.fwdIdx());
                coupon.fixing(fwd);
            }
        };

        void visit(FixFloatSwap<adouble>& inst) override {
            auto& leg = inst.secondLeg();
            for (auto& coupon : leg.coupons()) {
                adouble fwd = marketData_.fwds.at(coupon.fwdIdx());
                coupon.fixing(fwd);
            }
        };

       private:
        const MarketData<adouble>& marketData_;
    };
}  // namespace Atlas

#endif /* B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31 */
