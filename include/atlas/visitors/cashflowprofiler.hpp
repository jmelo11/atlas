#ifndef D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D
#define D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D

#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>
#include <map>

namespace Atlas {

    template <typename adouble>
    class CashflowProfiler : public ConstVisitor<adouble> {
       public:
        CashflowProfiler() = default;

        void visit(const FixedRateInstrument<adouble>& inst) const override { agreggate(inst.leg()); };
        void visit(const FloatingRateInstrument<adouble>& inst) const override { agreggate(inst.leg()); };

        void clear() {
            redemptions_.clear();
            interests_.clear();
        };

        std::map<Date, double> redemptions() const { return redemptions_; }
        std::map<Date, double> interests() const { return interests_; }

       private:
        template <typename T>
        void agreggate(const T& leg) const {
            for (const auto& coupon : leg.coupons()) {
                if (interests_.find(coupon.paymentDate()) == interests_.end()) { interests_[coupon.paymentDate()] = 0.0; }
                if constexpr (std::is_same_v<adouble, double>) {
                    interests_[coupon.paymentDate()] += coupon.amount();
                } else {
                    interests_[coupon.paymentDate()] += val(coupon.amount());
                }
            };
            for (const auto& redemption : leg.redemptions()) {
                if (redemptions_.find(redemption.paymentDate()) == redemptions_.end()) { redemptions_[redemption.paymentDate()] = 0.0; }
                if constexpr (std::is_same_v<adouble, double>) {
                    redemptions_[redemption.paymentDate()] += redemption.amount();
                } else {
                    redemptions_[redemption.paymentDate()] += val(redemption.amount());
                }
            };
        };

        mutable std::map<Date, double> redemptions_;
        mutable std::map<Date, double> interests_;
    };
}  // namespace Atlas

#endif /* D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D */
