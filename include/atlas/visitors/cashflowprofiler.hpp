#ifndef D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D
#define D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D

#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>
#include <map>

namespace Atlas {

    template <typename adouble>
    using Profile = std::map<Date, adouble>;

    template <typename adouble>
    class CashflowProfiler : public ConstVisitor<adouble> {
       public:
        CashflowProfiler(){};

        void visit(const FixedRateInstrument<adouble>& inst) const override { agreggate(inst.constLeg()); };
        void visit(const FloatingRateInstrument<adouble>& inst) const override { agreggate(inst.constLeg()); };

        void clear() {
            redemptions_.clear();
            interests_.clear();
        };

        Profile<adouble> redemptions() const { return redemptions_; }
        Profile<adouble> interests() const { return interests_; }

       private:
        template <typename T>
        void agreggate(const T& leg) const {
            for (const auto& coupon : leg.constCoupons()) {
                if (interests_.find(coupon.paymentDate()) == interests_.end()) { interests_[coupon.paymentDate()] = 0.0; }
                interests_[coupon.paymentDate()] += coupon.amount();
            };
            for (const auto& redemption : leg.constRedemptions()) {
                if (redemptions_.find(redemption.paymentDate()) == redemptions_.end()) { redemptions_[redemption.paymentDate()] = 0.0; }
            };
        };

        mutable Profile<adouble> redemptions_;
        mutable Profile<adouble> interests_;
    };
}  // namespace Atlas

#endif /* D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D */
