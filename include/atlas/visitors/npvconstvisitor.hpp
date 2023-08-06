#ifndef DC329E39_DDE8_403D_8DB9_7FECE70A4FA9
#define DC329E39_DDE8_403D_8DB9_7FECE70A4FA9

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/basevisitor.hpp>
#include <mutex>

namespace Atlas {
    /**
     * @class NPVConstVisitor
     * @brief A class for calculating the net present value of an instrument. It is a const visitor, so it does not modify the instrument.
     * @details If the instrument has floating rate coupons, fixing should be done prior to calling this visitor. Also, NPV are aggregated each
     * time the visitor is called, so if single values are needed, reset() should be called.
     * @ingroup Visitors
     *
     * @tparam adouble The number type used for the calculations. Can be double or an AAD type.
     */
    template <typename adouble>
    class NPVConstVisitor : public BaseConstVisitor<adouble> {
       public:
        struct Results {
            adouble npv                    = 0.0;
            adouble disbursementsNPV       = 0.0;
            adouble redemptionsNPV         = 0.0;
            adouble floatingRateCouponsNPV = 0.0;
            adouble fixedRateCouponsNPV    = 0.0;
        };

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
        void operator()(const CustomFixedRateInstrument<adouble>& inst) const override { cashflowsNPV(inst.cashflows()); };

        /**
         * @brief Calculate the net present value of a EqualPaymentInstrument.
         *
         * @param inst EqualPaymentInstrument
         */
        void operator()(const EqualPaymentInstrument<adouble>& inst) const override { cashflowsNPV(inst.cashflows()); };

        /**
         * @brief Calculate the net present value of a FixedRateBulletInstrument.
         *
         * @param inst FixedRateBulletInstrument
         */
        void operator()(const FixedRateBulletInstrument<adouble>& inst) const override { cashflowsNPV(inst.cashflows()); };

        /**
         * @brief Calculate the net present value of a ZeroCouponInstrument.
         *
         * @param inst ZeroCouponInstrument
         */
        void operator()(const ZeroCouponInstrument<adouble>& inst) const override { cashflowsNPV(inst.cashflows()); };

        /**
         * @brief Calculate the net present value of a CustomFloatingRateInstrument.
         *
         * @param inst CustomFloatingRateInstrument
         */
        void operator()(const CustomFloatingRateInstrument<adouble>& inst) const override { cashflowsNPV(inst.cashflows()); };

        /**
         * @brief Calculate the net present value of a FloatingRateBulletInstrument.
         *
         * @param inst FloatingRateBulletInstrument
         */
        void operator()(const FloatingRateBulletInstrument<adouble>& inst) const override { cashflowsNPV(inst.cashflows()); };

        /**
         * @brief Calculate the net present value of a FloatingRateEqualRedemptionInstrument.
         *
         * @param inst FloatingRateEqualRedemptionInstrument
         */
        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& inst) const override { cashflowsNPV(inst.cashflows()); };

        /**
         * @brief Clear the net present value of the visited instruments.
         *
         */
        void reset() { results_ = Results(); };

        /**
         * @brief Returns the net present value of the visited instruments.
         *
         * @return adouble
         */
        inline Results getResults() const {
            results_.npv = results_.disbursementsNPV + results_.redemptionsNPV + results_.floatingRateCouponsNPV + results_.fixedRateCouponsNPV;
            return results_;
        };

       private:
        /**
         * @brief Helper method for calculating the net present value of a fixed income instrument.
         *
         * @tparam I An instrument class.
         * @param inst The given instrument.
         */
        template <class CS>
        void cashflowsNPV(const CS& cashflows) const {
            std::lock_guard<std::mutex> lock(mtx_);
            for (const auto& cashflow : cashflows.disbursements()) results_.disbursementsNPV += cashflowNPV(cashflow);
            for (const auto& cashflow : cashflows.redemptions()) results_.redemptionsNPV += cashflowNPV(cashflow);
            if constexpr (std::is_base_of_v<FloatingRateCouponStreamMixin<adouble>, CS>) {
                for (const auto& cashflow : cashflows.floatingRateCoupons()) results_.floatingRateCouponsNPV += cashflowNPV(cashflow);
            }
            if constexpr (std::is_base_of_v<FixedRateCouponStreamMixin<adouble>, CS>) {
                for (const auto& cashflow : cashflows.fixedRateCoupons()) results_.fixedRateCouponsNPV += cashflowNPV(cashflow);
            }
        };

        template <class C>
        adouble cashflowNPV(const C& cashflow) const {
            if (!cashflow.isIndexed()) throw std::runtime_error("Coupon is not indexed.");
            if constexpr (std::is_same_v<FloatingRateCoupon<adouble>, C>) {
                if (!cashflow.isFixingSet()) throw std::runtime_error("Floating coupon does not have fixing set.");
            }
            adouble df  = marketData_.dfs.at(cashflow.dfIdx());
            adouble fx  = marketData_.fxs.at(cashflow.fxIdx());
            adouble npv = cashflow.amount() * df / fx * static_cast<int>(cashflow.side());
            return npv;
        };

        mutable std::mutex mtx_;
        const MarketData<adouble>& marketData_;
        mutable Results results_ = Results();
    };
}  // namespace Atlas

#endif /* DC329E39_DDE8_403D_8DB9_7FECE70A4FA9 */
