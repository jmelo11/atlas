#ifndef EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE
#define EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <iostream>

namespace Atlas {

    template <typename adouble>
    class NPVCalculator : public Visitor<adouble> {
       public:
        NPVCalculator(const MarketData<adouble>& marketData) : marketData_(marketData){};

        void clear() {
            npv_        = 0.0;
            nonSensNPV_ = 0.0;
        };

        inline adouble results() const { return npv_ + nonSensNPV_; };

        inline adouble sensNPV() const { return npv_; };

        inline double nonSensNPV() const { return nonSensNPV_; };

        void visit(FixedRateInstrument<adouble>& inst) override {
            fixedLegNPV(inst.leg());
            redemptionsNPV(inst.leg());
            //npv(inst.leg());
        };

        void visit(FloatingRateInstrument<adouble>& inst) override {
            floatingLegNPV(inst.leg());
            redemptionsNPV(inst.leg());
        };

       private:
        template <typename LegType>
        void npv(LegType& leg) {
            adouble df = 0.0;
            for (const auto& redemption : leg.redemptions()) {
                df = marketData_.dfs.at(redemption.dfIdx());
                if constexpr (std::is_same_v<adouble, double>) {
                    nonSensNPV_ += redemption.amount() * df;
                } else {
                    nonSensNPV_ += redemption.amount().val * df.val;
                }
            }

            for (auto& coupon : leg.coupons()) {
                df = marketData_.dfs.at(coupon.dfIdx());
                if constexpr (std::is_same_v<LegType, FloatingRateLeg<adouble>>) {
                    adouble fwd = marketData_.fwds.at(coupon.fwdIdx());
                    coupon.fixing(fwd);
                }
                npv_ += coupon.amount() * df;
            }
        };

        void redemptionsNPV(const Leg<adouble>& leg) {
            adouble npv = 0.0;
            adouble df  = 0.0;
            for (const auto& redemption : leg.redemptions()) {
                df = marketData_.dfs.at(redemption.dfIdx());
                npv += redemption.amount() * df;
            }
            if constexpr (std::is_same_v<adouble, double>) {
                nonSensNPV_ += npv;
            } else {
                nonSensNPV_ += npv.val;
            }
        };

        void fixedLegNPV(const FixedRateLeg<adouble>& leg) {
            adouble npv = 0.0;
            adouble df  = 0.0;
            for (auto& coupon : leg.coupons()) {
                df = marketData_.dfs.at(coupon.dfIdx());
                npv += coupon.amount() * df;
            }
            npv_ += npv;
        };

        void floatingLegNPV(FloatingRateLeg<adouble>& leg) {
            adouble npv = 0.0;
            adouble df  = 0.0;
            adouble fwd = 0.0;
            for (FloatingRateCoupon<adouble>& coupon : leg.coupons()) {
                df  = marketData_.dfs.at(coupon.dfIdx());
                fwd = marketData_.fwds.at(coupon.fwdIdx());
                coupon.fixing(fwd);
                npv += coupon.amount() * df;
            }
            npv_ += npv;
        };

        adouble npv_       = 0.0;
        double nonSensNPV_ = 0.0;
        const MarketData<adouble>& marketData_;
    };
}  // namespace Atlas

#endif /* EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE */
