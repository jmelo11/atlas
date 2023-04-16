#ifndef EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE
#define EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <iostream>

namespace Atlas {
    /**
     * @brief  A class for calculating the net present value of an instrument.
     *
     * @tparam adouble
     */
    template <typename adouble>
    class NPVCalculator : public Visitor<adouble> {
       public:
        /**
         * @brief Construct a new NPV Calculator object
         *
         * @param marketData market data for the instrument
         */
        NPVCalculator(const MarketData<adouble>& marketData) : marketData_(marketData){};

        /**
         * @brief Clear the net present value of the instrument
         *
         */
        void clear() { npv_ = 0.0; };

        /**
         * @brief Returns the net present value of the instrument
         *
         * @return adouble
         */
        inline adouble results() const { return npv_; };

        /**
         * @brief Visit a fixed rate instrument
         *
         * @param inst
         */
        void visit(FixedRateInstrument<adouble>& inst) override {
            npv_ += fixedLegNPV(inst.leg());
            npv_ += redemptionsNPV(inst.leg());
        };

        /**
         * @brief Visit a floating rate instrument
         *
         * @param inst
         */
        void visit(FloatingRateInstrument<adouble>& inst) override {
            npv_ += floatingLegNPV(inst.leg());
            npv_ += redemptionsNPV(inst.leg());
        };

        /**
         * @brief Visit a swap
         *
         * @param inst
         */
        void visit(FxForward<adouble>& inst) override {
            int side     = (int)inst.side();
            adouble spot = marketData_.spots.at(inst.secondLeg().redemptions()[0].spotIdx());
            adouble npv1 = redemptionsNPV(inst.firstLeg());
            adouble npv2 = redemptionsNPV(inst.secondLeg());
            npv_ += side * (npv1 - npv2 * spot);
        };

       private:
        /**
         * @brief Calculate the net present value of the redemptions
         *
         * @param leg
         * @return adouble
         */
        adouble redemptionsNPV(const Leg<adouble>& leg) {
            adouble npv = 0.0;
            for (const auto& redemption : leg.redemptions()) {
                adouble spot      = marketData_.spots.at(redemption.spotIdx());
                adouble numeraire = marketData_.numerarie;
                adouble df        = marketData_.dfs.at(redemption.dfIdx());
                npv += redemption.amount() * df * spot / numeraire;
            }
            return npv;
        };
        /**
         * @brief Calculate the net present value of the fixed rate leg
         *
         * @param leg
         * @return adouble
         */
        adouble fixedLegNPV(const FixedRateLeg<adouble>& leg) {
            adouble npv = 0.0;
            adouble df  = 0.0;
            for (auto& coupon : leg.coupons()) {
                df = marketData_.dfs.at(coupon.dfIdx());
                npv += coupon.amount() * df;
            }
            return npv;
        };

        /**
         * @brief Calculate the net present value of the floating rate leg
         *
         * @param leg
         * @return adouble
         */
        adouble floatingLegNPV(FloatingRateLeg<adouble>& leg) {
            adouble npv = 0.0;
            adouble df  = 0.0;
            adouble fwd = 0.0;
            for (FloatingRateCoupon<adouble>& coupon : leg.coupons()) {
                df  = marketData_.dfs.at(coupon.dfIdx());
                fwd = marketData_.fwds.at(coupon.fwdIdx());
                coupon.fixing(fwd);
                npv += coupon.amount() * df;
            }
            return npv;
        };

        adouble npv_ = 0.0;
        const MarketData<adouble>& marketData_;
    };
}  // namespace Atlas

#endif /* EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE */
