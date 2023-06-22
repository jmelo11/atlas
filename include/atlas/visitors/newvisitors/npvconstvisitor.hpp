#ifndef DC329E39_DDE8_403D_8DB9_7FECE70A4FA9
#define DC329E39_DDE8_403D_8DB9_7FECE70A4FA9

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/derivatives/fixfloatswap.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/newvisitors/basevisitor.hpp>

namespace Atlas {
    /**
     * @class NPVConstVisitor
     * @brief A class for calculating the net present value of an instrument. It is a const visitor, so it does not modify the instrument.
     * @details If the instrument has floating rate coupons, fixing should be done prior to calling this visitor. Also, NPV are aggregated each
     * time the visitor is called, so if single values are needed, reset() should be called.
     * @ingroup Visitors
     *
     * @tparam adouble
     */
    template <typename adouble>
    class NPVConstVisitor : public BaseConstVisitor<adouble> {
       public:
        /**
         * @brief Construct a new NPVConstVisitor object
         *
         * @param marketData MarketData object containing the discount factors, rates and others needed to price the instrument.
         */
        NPVConstVisitor(const MarketData<adouble>& marketData) : marketData_(marketData){};

        void operator()(const std::monostate& inst) const override { this->template printLogs<NPVConstVisitor>(this, "monostate"); }

        /**
         * @brief Calculate the net present value of a CustomFixedRateInstrument.
         *
         * @param inst CustomFixedRateInstrument
         */
        void operator()(const CustomFixedRateInstrument<adouble>& inst) const override { fixedIncomeNPV(inst); };

        /**
         * @brief Calculate the net present value of a EqualPaymentInstrument.
         *
         * @param inst EqualPaymentInstrument
         */
        void operator()(const EqualPaymentInstrument<adouble>& inst) const override { fixedIncomeNPV(inst); };

        /**
         * @brief Calculate the net present value of a FixedRateBulletInstrument.
         *
         * @param inst FixedRateBulletInstrument
         */
        void operator()(const FixedRateBulletInstrument<adouble>& inst) const override { fixedIncomeNPV(inst); };

        /**
         * @brief Calculate the net present value of a ZeroCouponInstrument.
         *
         * @param inst ZeroCouponInstrument
         */
        void operator()(const ZeroCouponInstrument<adouble>& inst) const override { fixedIncomeNPV(inst); };

        /**
         * @brief Calculate the net present value of a CustomFloatingRateInstrument.
         *
         * @param inst CustomFloatingRateInstrument
         */
        void operator()(const CustomFloatingRateInstrument<adouble>& inst) const override { fixedIncomeNPV(inst); };

        /**
         * @brief Calculate the net present value of a FloatingRateBulletInstrument.
         *
         * @param inst FloatingRateBulletInstrument
         */
        void operator()(const FloatingRateBulletInstrument<adouble>& inst) const override { fixedIncomeNPV(inst); };

        /**
         * @brief Calculate the net present value of a FloatingRateEqualRedemptionInstrument.
         *
         * @param inst FloatingRateEqualRedemptionInstrument
         */
        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& inst) const override { fixedIncomeNPV(inst); };

        /**
         * @brief Calculate the net present value of a FxForward.
         *
         * @param inst
         */
        void operator()(const FxForward<adouble>& inst) const override {
            const auto& cashflows = inst.leg().redemptions();
            int side              = inst.side();

            if (!cashflows.at(0).isIndexed()) throw std::runtime_error("Redemption is not indexed.");
            adouble fwd = marketData_.fxs.at(cashflows.at(0).fxIdx());
            adouble df  = marketData_.dfs.at(cashflows.at(0).dfIdx());

            adouble spot = marketData_.fxs.at(cashflows.at(1).fxIdx());
            adouble npv  = (fwd - inst.fwdPrice()) * df * side * inst.notional() / spot;

            std::lock_guard<std::mutex> lock(mtx_);
            npv_ += npv;
        };

        /**
         * @brief Calculate the net present value of a FixFloatSwap.
         *
         * @param inst FixFloatSwap
         */
        void operator()(const FixFloatSwap<adouble>& inst) const override {
            int side         = inst.side();
            adouble fixNPV   = 0.0;
            adouble floatNPV = 0.0;

            const auto& firstCoupon = inst.firstLeg().coupons().at(0);
            if (!firstCoupon.isIndexed()) throw std::runtime_error("FixFloatSwap is not indexed.");
            adouble fx = marketData_.fxs.at(firstCoupon.fxIdx());
            fixNPV += legNPV(inst.firstLeg(), fx);
            floatNPV += legNPV(inst.secondLeg(), fx);
            adouble npv = (fixNPV - floatNPV) * side;

            std::lock_guard<std::mutex> lock(mtx_);
            npv_ += npv;
        }

        /**
         * @brief Clear the net present value of the visited instruments.
         *
         */
        void reset() { npv_ = 0.0; };

        /**
         * @brief Returns the net present value of the visited instruments.
         *
         * @return adouble
         */
        inline adouble getResults() const { return npv_; };

       private:
        /**
         * @brief Helper method for calculating the net present value of a fixed income instrument.
         *
         * @tparam I An instrument class.
         * @param inst The given instrument.
         */
        template <typename I>
        void fixedIncomeNPV(const I& inst) const {
            const auto& disbursement = inst.disbursement();
            if (!disbursement.isIndexed()) throw std::runtime_error("Disbursement is not indexed.");
            adouble spotFx = marketData_.fxs.at(disbursement.fxIdx());
            adouble npv    = legNPV(inst.leg(), spotFx);
            std::lock_guard<std::mutex> lock(mtx_);
            npv_ += npv;
        };

        /**
         * @brief Helper method for calculating the net present value of a leg.
         * @details It applies an exchange rate to the amount.
         *
         * @tparam L A leg class.
         * @param leg The given leg.
         * @param spotFx The spot fx rate.
         * @return adouble
         */
        template <typename L>
        adouble legNPV(const L& leg, adouble spotFx) const {
            adouble npv = 0.0;
            if constexpr (!std::is_same_v<L, Leg<adouble>>) {
                for (auto& coupon : leg.coupons()) {
                    if (!coupon.isIndexed()) throw std::runtime_error("Coupon is not indexed.");
                    if constexpr (std::is_same_v<L, FloatingRateLeg<adouble>>) {
                        if (!coupon.hasFixingSet()) throw std::runtime_error("Floating coupon does not have fixing set.");
                    }
                    adouble df = marketData_.dfs.at(coupon.dfIdx());
                    if (coupon.applyCcy()) {
                        adouble fwdFx = marketData_.fxs.at(coupon.fxIdx());
                        npv += coupon.amount() * df / fwdFx;
                    } else {
                        npv += coupon.amount() * df / spotFx;
                    }
                }
            }
            for (const auto& redemption : leg.redemptions()) {
                if (!redemption.isIndexed()) throw std::runtime_error("Redemption is not indexed.");
                adouble df = marketData_.dfs.at(redemption.dfIdx());
                if (redemption.applyCcy()) {
                    adouble fwdFx = marketData_.fxs.at(redemption.fxIdx());
                    npv += redemption.amount() * df / fwdFx;
                } else {
                    npv += redemption.amount() * df / spotFx;
                }
            }
            return npv;
        }

        mutable std::mutex mtx_;
        mutable adouble npv_ = 0.0;
        const MarketData<adouble>& marketData_;
    };
}  // namespace Atlas

#endif /* DC329E39_DDE8_403D_8DB9_7FECE70A4FA9 */
