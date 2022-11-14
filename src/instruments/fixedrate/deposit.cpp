#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/deposit.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    Deposit::Deposit(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double amount,
                     const QuantLib::InterestRate& rate)
    : FixedRateProduct(startDate, endDate) {
        FixedRateCoupon coupon(startDate, endDate, amount, rate);  // interest coupon
        Redemption redemption(endDate, amount);                    // notinal payment at the end

        leg_.addCoupon(coupon);
        leg_.addRedemption(redemption);
    }

    void Deposit::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void Deposit::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas