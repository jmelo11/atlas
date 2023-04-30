#ifndef EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE
#define EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
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
            adouble npv = 0.0;
            npv += fixedLegNPV(inst.leg());
            npv += redemptionsNPV(inst.leg());
            adouble fx = marketData_.fxs.at(inst.disbursement().fxIdx());
            npv_ += npv / fx;
        };

        /**
         * @brief Visit a floating rate instrument
         *
         * @param inst
         */
        void visit(FloatingRateInstrument<adouble>& inst) override {
            adouble npv = 0.0;
            npv += floatingLegNPV(inst.leg());
            npv += redemptionsNPV(inst.leg());
            adouble fx = marketData_.fxs.at(inst.disbursement().fxIdx());
            npv_ += npv / fx;
        };

        /**
         * @brief Visit a FxForward
         *
         * @param inst
         */
        void visit(FxForward<adouble>& inst) override {
            const auto& cashflows = inst.leg().redemptions();
            int side              = inst.side();

            adouble fwd = marketData_.fxs.at(cashflows.at(0).fxIdx());
            adouble df  = marketData_.dfs.at(cashflows.at(0).dfIdx());

            adouble spot = marketData_.fxs.at(cashflows.at(1).fxIdx());
            npv_ += (fwd - inst.fwdPrice()) * df * side * inst.notional() / spot;
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
                adouble df = marketData_.dfs.at(redemption.dfIdx());
                npv += redemption.amount() * df;
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
