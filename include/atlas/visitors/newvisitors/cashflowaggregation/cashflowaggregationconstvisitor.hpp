#ifndef A308957E_F203_42E5_A210_C2C8D58BA6F8
#define A308957E_F203_42E5_A210_C2C8D58BA6F8

#include <atlas/visitors/newvisitors/basevisitor.hpp>
#include <functional>

namespace Atlas {

    template <typename adouble = double>
    using CouponInspector = std::function<adouble(const Coupon<adouble>*)>;

    template <typename adouble = double>
    using RedemptionInspector = std::function<adouble(const Redemption<adouble>*)>;

    template <typename adouble = double>
    class CashflowAggregationConstVisitor : public BaseConstVisitor<adouble> {
       public:
        CashflowAggregationConstVisitor(const Date& startDate, const Date& endDate) : startDate_(startDate), endDate_(endDate){};

        virtual ~CashflowAggregationConstVisitor() = default;

        void operator()(const std::monostate& inst) const override { this->template printLogs<CashflowAggregationConstVisitor>(this, "monostate"); }

        void operator()(const CustomFixedRateInstrument<adouble>& instrument) const override {
            apply(instrument.leg().coupons());
            apply(instrument.leg().redemptions());
        }

        void operator()(const EqualPaymentInstrument<adouble>& instrument) const override {
            apply(instrument.leg().coupons());
            apply(instrument.leg().redemptions());
        }

        void operator()(const FixedRateBulletInstrument<adouble>& instrument) const override {
            apply(instrument.leg().coupons());
            apply(instrument.leg().redemptions());
        }

        void operator()(const ZeroCouponInstrument<adouble>& instrument) const override {
            apply(instrument.leg().coupons());
            apply(instrument.leg().redemptions());
        }

        void operator()(const FloatingRateBulletInstrument<adouble>& instrument) const override {
            apply(instrument.leg().coupons());
            apply(instrument.leg().redemptions());
        }

        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& instrument) const override {
            apply(instrument.leg().coupons());
            apply(instrument.leg().redemptions());
        }

        void operator()(const CustomFloatingRateInstrument<adouble>& instrument) const override {
            apply(instrument.leg().coupons());
            apply(instrument.leg().redemptions());
        }

        void operator()(const FxForward<adouble>& instrument) const override { apply(instrument.leg().redemptions()); }

        void operator()(const FixFloatSwap<adouble>& instrument) const override {
            apply(instrument.firstLeg().coupons());
            apply(instrument.firstLeg().redemptions());
            apply(instrument.secondLeg().coupons());
            apply(instrument.secondLeg().redemptions());
        }

        void reset() { amount_ = 0.0; }

        adouble getResults() const { return amount_; }

       protected:
        template <template <typename> class C>
        void apply(const std::vector<C<adouble>>& cashflows) const {
            adouble amount = 0.0;
            if constexpr (std::is_base_of_v<Coupon<adouble>, C<adouble>>) {
                for (const auto& cashflow : cashflows) { amount += couponInspector_(&cashflow); }
            } else if constexpr (std::is_same_v<Redemption<adouble>, C<adouble>>) {
                for (const auto& cashflow : cashflows) { amount += redemptionInspector_(&cashflow); }
            }
            std::lock_guard<std::mutex> lock(mtx_);
            amount_ += amount;
        }

        void startDate(const Date& startDate) { startDate_ = startDate; }

        void endDate(const Date& endDate) { endDate_ = endDate; }

        Date startDate_;
        Date endDate_;
        CouponInspector<adouble> couponInspector_;
        RedemptionInspector<adouble> redemptionInspector_;
        mutable std::mutex mtx_;
        mutable adouble amount_ = 0.0;
    };
}  // namespace Atlas

#endif /* A308957E_F203_42E5_A210_C2C8D58BA6F8 */
