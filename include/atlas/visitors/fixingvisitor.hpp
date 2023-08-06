#ifndef C77767A1_346A_4254_865F_DDE836D15D05
#define C77767A1_346A_4254_865F_DDE836D15D05

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/basevisitor.hpp>

namespace Atlas {

    /**
     * @class FixingVisitor
     * @brief A visitor that fixes the coupons of an instrument with floating rate legs. After the fixing, the instrument can be priced.
     * @details sets the variable isFixingSet_ to true after fixing.
     * @ingroup Visitors
     *
     * @tparam adouble The number type used for the calculations. Can be double or an AAD type.
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

        void operator()(std::monostate& inst) override { this->template printLogs<FixingVisitor>(this, "monostate"); }

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
        void operator()(CustomFloatingRateInstrument<adouble>& inst) override { fixFloatingRate(inst.cashflows()); };

        /**
         * @brief Fix the coupons of the floating rate leg of the instrument.
         *
         * @param inst FloatingRateBulletInstrument
         */
        void operator()(FloatingRateBulletInstrument<adouble>& inst) override { fixFloatingRate(inst.cashflows()); };

        /**
         * @brief Fix the coupons of the floating rate leg of the instrument.
         *
         * @param inst FloatingRateEqualRedemptionInstrument
         */
        void operator()(FloatingRateEqualRedemptionInstrument<adouble>& inst) override { fixFloatingRate(inst.cashflows()); };

       private:
        /**
         * @brief Fix the coupons of the floating rate leg of the instrument.
         *
         * @tparam L A floating rate leg
         * @param leg The given leg.
         */
        template <class CS>
        void fixFloatingRate(CS& cashflows) {
            if constexpr (std::is_base_of_v<FloatingRateCouponStreamMixin<adouble>, CS>) {
                for (auto& coupon : cashflows.floatingRateCoupons()) {
                    if (!coupon.isIndexed()) throw std::runtime_error("FixingVisitor: Coupon is not indexed.");
                    adouble fwd = marketData_.fwds.at(coupon.fwdIdx());
                    coupon.fixing(fwd);
                }
            }
        }

        const MarketData<adouble>& marketData_;
    };
}  // namespace Atlas

#endif /* C77767A1_346A_4254_865F_DDE836D15D05 */
