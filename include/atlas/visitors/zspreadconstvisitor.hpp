#ifndef F90CEEA9_894D_44B7_BEBE_BED47A38D117
#define F90CEEA9_894D_44B7_BEBE_BED47A38D117

#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>

namespace Atlas {
    /**
     * @class ZSpreadConstVisitor
     * @brief A visitor for calculating the z-spread of an instrument. Only fix income instruments are supported.
     * @ingroup Visitors
     *
     * @tparam adouble The number type used for the calculations. Can be double or an AAD type.
     */
    template <typename adouble>
    class ZSpreadConstVisitor : public BaseConstVisitor<adouble> {
       public:
        /**
         * @brief Construct a new ZSpreadConstVisitor object
         *
         * @param marketData market data from a model
         * @param targetNPV target npv to solve
         * @param dayCounter implied rate day counter
         * @param comp implied rate compounding
         * @param freq implied rate frequency
         * @param guess search guess
         * @param accuracy accuracy
         * @param maxIter maximum iterations
         */
        ZSpreadConstVisitor(const MarketData<adouble>& marketData, adouble targetNPV, const DayCounter& dayCounter = Actual360(),
                            Compounding comp = Compounding::Compounded, Frequency freq = Frequency::Semiannual, double guess = 0.01,
                            double accuracy = 1e-8, size_t maxIter = 100, bool showLogs = false)
        : BaseConstVisitor<adouble>(showLogs),
          marketData_(marketData),
          targetNPV_(targetNPV),
          dayCounter_(dayCounter),
          comp_(comp),
          freq_(freq),
          guess_(guess),
          accuracy_(accuracy),
          maxIter_(maxIter){};

        /**
         * @brief Set the Target NPV
         *
         * @param targetNPV
         */
        void setTargetNPV(adouble targetNPV) { targetNPV_ = targetNPV; }

        void operator()(const std::monostate& inst) const override { this->template printLogs<ZSpreadConstVisitor>(this, "monostate"); }

        void operator()(const CustomFixedRateInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const FixedRateBulletInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const EqualPaymentInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const FixedRateEqualRedemptionInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const ZeroCouponFixedRateInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const FloatingRateBulletInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const CustomFloatingRateInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void operator()(const ZeroCouponFloatingRateInstrument<adouble>& inst) const override { zspread(inst.cashflows()); };

        void reset() { value_ = 0.0; };

        /**
         * @brief Get the results
         *
         * @return adouble
         */
        adouble getResults() const { return value_; }

       private:
        template <class C>
        inline adouble cashflowNPV(const C& cashflow, adouble z, const MarketData<adouble>& marketData) const {
            adouble df = marketData.dfs.at(cashflow.dfIdx());
            adouble r  = InterestRate<adouble>::impliedRate(1 / df, dayCounter_, comp_, freq_, marketData.refDate, cashflow.paymentDate()).rate();
            return cashflow.amount() / fastCompoundFactor(r + z, dayCounter_, comp_, freq_, marketData.refDate, cashflow.paymentDate());
        }

        template <class CS>
        void zspread(const CS& cashflows) const {
            auto f = [&](adouble z) {
                adouble npv = 0.0;
                if constexpr (std::is_base_of_v<FixedRateCouponStreamMixin<adouble>, CS>) {
                    for (const auto& coupon : cashflows.fixedRateCoupons()) { npv += cashflowNPV(coupon, z, marketData_); }
                }
                if constexpr (std::is_base_of_v<FloatingRateCouponStreamMixin<adouble>, CS>) {
                    for (const auto& coupon : cashflows.floatingRateCoupons()) { npv += cashflowNPV(coupon, z, marketData_); }
                }
                for (const auto& redemption : cashflows.redemptions()) { npv += cashflowNPV(redemption, z, marketData_); }
                return targetNPV_ - npv;
            };

            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                value_ = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g = [&](double x) { return val(f(x)); };
                value_ = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        mutable adouble value_ = 0.0;
        const MarketData<adouble>& marketData_;
        adouble targetNPV_;
        DayCounter dayCounter_;
        Compounding comp_;
        Frequency freq_;
        double guess_;
        double accuracy_;
        size_t maxIter_;
    };
}  // namespace Atlas

#endif /* F90CEEA9_894D_44B7_BEBE_BED47A38D117 */
