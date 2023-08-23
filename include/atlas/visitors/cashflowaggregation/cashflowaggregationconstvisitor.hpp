#ifndef A308957E_F203_42E5_A210_C2C8D58BA6F8
#define A308957E_F203_42E5_A210_C2C8D58BA6F8

#include <atlas/visitors/basevisitor.hpp>
#include <functional>
#include <mutex>

namespace Atlas {

    template <typename adouble = double>
    using CouponInspector = std::function<adouble(const Coupon<adouble>*)>;

    template <typename adouble = double>
    using CashflowInspector = std::function<adouble(const Cashflow<adouble>*)>;

    template <typename adouble = double>
    class CashflowAggregationConstVisitor : public BaseConstVisitor<adouble> {
       public:
        CashflowAggregationConstVisitor(const Date& startDate, const Date& endDate) : startDate_(startDate), endDate_(endDate){};

        virtual ~CashflowAggregationConstVisitor() = default;

        void operator()(const std::monostate& inst) const override { this->template printLogs<CashflowAggregationConstVisitor>(this, "monostate"); }

        void operator()(const CustomFixedRateInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().fixedRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void operator()(const EqualPaymentInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().fixedRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void operator()(const FixedRateBulletInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().fixedRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void operator()(const ZeroCouponFixedRateInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().fixedRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void operator()(const FloatingRateBulletInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().floatingRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().floatingRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void operator()(const CustomFloatingRateInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().floatingRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void operator()(const ZeroCouponFloatingRateInstrument<adouble>& instrument) const override {
            apply(instrument.cashflows().floatingRateCoupons());
            apply(instrument.cashflows().redemptions());
            apply(instrument.cashflows().disbursements());
        }

        void reset() { amount_ = 0.0; }

        adouble getResults() const { return amount_; }

       protected:
        template <class C>
        void apply(const std::vector<C>& cashflows) const {
            adouble amount = 0.0;
            if constexpr (std::is_base_of_v<Coupon<adouble>, C>) {
                for (const auto& cashflow : cashflows) { amount += couponInspector_(&cashflow); }
            } else if constexpr (std::is_same_v<Cashflow<adouble>, C>) {
                for (const auto& cashflow : cashflows) { amount += cashflowInspector_(&cashflow); }
            }
            std::lock_guard<std::mutex> lock(mtx_);
            amount_ += amount;
        }

        void startDate(const Date& startDate) { startDate_ = startDate; }

        void endDate(const Date& endDate) { endDate_ = endDate; }

        Date startDate_;
        Date endDate_;
        CouponInspector<adouble> couponInspector_;
        CashflowInspector<adouble> cashflowInspector_;
        mutable std::mutex mtx_;
        mutable adouble amount_ = 0.0;
    };
}  // namespace Atlas

#endif /* A308957E_F203_42E5_A210_C2C8D58BA6F8 */
