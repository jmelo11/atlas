#ifndef C77767A1_346A_4254_865F_DDE836D15D05
#define C77767A1_346A_4254_865F_DDE836D15D05

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
     * @class FixingVisitor
     * @brief A visitor that fixes the coupons of an instrument with floating rate legs. After the fixing, the instrument can be priced.
     * @details sets the variable isFixingSet_ to true after fixing.
     * @ingroup Visitors
     * @tparam adouble
     */
    template <typename adouble>
    class FixingVisitor : public BaseVisitor<adouble> {
       public:
        /**
         * @brief Construct a new Fixing Visitor object
         *
         * @param marketData A MarketData object containing the market data needed for fixing rates.
         */
        FixingVisitor(const MarketData<adouble>& marketData, bool showLogs = false) : BaseVisitor<adouble>(showLogs), marketData_(marketData){};

        /**
         * @brief Since the instrument is not a floating rate instrument, the visitor only prints a message.
         *
         * @param inst CustomFixedRateInstrument
         */
        void operator()(CustomFixedRateInstrument<adouble>& inst) override {
            this->template printLogs<FixingVisitor>(this, "CustomFixedRateInstrument is not a floating rate instrument.");
        };

        /**
         * @brief Since the instrument is not a floating rate instrument, the visitor only prints a message.
         *
         * @param inst EqualPaymentInstrument
         */
        void operator()(EqualPaymentInstrument<adouble>& inst) override {
            this->template printLogs<FixingVisitor>(this, "EqualPaymentInstrument is not a floating rate instrument.");
        };

        /**
         * @brief Since the instrument is not a floating rate instrument, the visitor only prints a message.
         *
         * @param inst FixedRateBulletInstrument
         */
        void operator()(FixedRateBulletInstrument<adouble>& inst) override {
            this->template printLogs<FixingVisitor>(this, "FixedRateBulletInstrument is not a floating rate instrument.");
        };

        /**
         * @brief Since the instrument is not a floating rate instrument, the visitor only prints a message.
         *
         * @param inst ZeroCouponInstrument
         */
        void operator()(ZeroCouponInstrument<adouble>& inst) override {
            std::cout << "FixingVisitor: ZeroCouponInstrument is not a floating rate instrument." << std::endl;
        };

        /**
         * @brief Fix the coupons of the floating rate leg of the instrument.
         *
         * @param inst CustomFloatingRateInstrument
         */
        void operator()(CustomFloatingRateInstrument<adouble>& inst) override { fixLegCoupons(inst.leg()); };

        /**
         * @brief Fix the coupons of the floating rate leg of the instrument.
         *
         * @param inst FloatingRateBulletInstrument
         */
        void operator()(FloatingRateBulletInstrument<adouble>& inst) override { fixLegCoupons(inst.leg()); };

        /**
         * @brief Fix the coupons of the floating rate leg of the instrument.
         *
         * @param inst FloatingRateEqualRedemptionInstrument
         */
        void operator()(FloatingRateEqualRedemptionInstrument<adouble>& inst) override { fixLegCoupons(inst.leg()); };

        /**
         * @brief An fx forward is not a floating rate instrument, so the visitor only prints a message.
         *
         * @param inst
         */
        void operator()(FxForward<adouble>& inst) override {
            this->template printLogs<FixingVisitor>(this, "FxForward is not a floating rate instrument.");
        };

        /**
         * @brief Fix the coupons of the floating rate legs of the instrument.
         *
         * @param inst FixFloatSwap
         */
        void operator()(FixFloatSwap<adouble>& inst) override { fixLegCoupons(inst.secondLeg()); };

       private:
        /**
         * @brief Fix the coupons of the floating rate leg of the instrument.
         *
         * @tparam L A floating rate leg
         * @param leg The given leg.
         */
        template <typename L>
        void fixLegCoupons(L& leg) {
            if constexpr (std::is_same_v<L, FloatingRateLeg<adouble>>) {
                for (auto& coupon : leg.coupons()) {
                    adouble fwd = marketData_.fwds.at(coupon.fwdIdx());
                    coupon.fixing(fwd);
                    coupon.hasFixingSet(true);
                }
            }
        }

        const MarketData<adouble>& marketData_;
    };
}  // namespace Atlas

#endif /* C77767A1_346A_4254_865F_DDE836D15D05 */
