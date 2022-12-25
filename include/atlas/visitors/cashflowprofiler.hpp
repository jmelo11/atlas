#ifndef D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D
#define D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D

#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>
#include <map>

namespace Atlas {

    using Profile = std::map<QuantLib::Date, double>;

    class CashflowProfiler : public ConstVisitor {
       public:
        CashflowProfiler(){};

        void visit(const Deposit& inst) const override;
        void visit(const FixedRateBulletProduct& inst) const override;
        void visit(const EqualPaymentProduct& inst) const override;
        void visit(const FloatingRateBulletProduct& inst) const override;
        void visit(const CustomFixedRateProduct& inst) const override;
        void visit(const CustomFloatingRateProduct& inst) const override;

        void clear() {
            redemptions_.clear();
            interests_.clear();
        };

        Profile redemptions() const { return redemptions_; }
        Profile interests() const { return interests_; }

       private:
        template <typename T>
        void agreggate(const T& leg) const {
            for (const auto& coupon : leg.constCoupons()) {
                if (interests_.find(coupon.paymentDate()) == interests_.end()) { interests_[coupon.paymentDate()] = 0.0; }
                interests_[coupon.paymentDate()] += coupon.amount();
            };
            for (const auto& redemption : leg.constRedemptions()) {
                if (redemptions_.find(redemption.paymentDate()) == redemptions_.end()) { redemptions_[redemption.paymentDate()] = 0.0; }
                redemptions_[redemption.paymentDate()] += redemption.amount();
            };
        };

        mutable Profile redemptions_;
        mutable Profile interests_;
    };
}  // namespace Atlas

#endif /* D56F5F8B_A8F6_4E9B_81F6_BCD8A1914F0D */
