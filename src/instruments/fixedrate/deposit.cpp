#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/deposit.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    Deposit::Deposit(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                     double notional, const QuantLib::InterestRate& rate)
    : FixedRateProduct(startDate, endDate, rate, notional) {
        FixedRateCoupon coupon(startDate, endDate, notional, rate);  // interest coupon
        Redemption redemption(endDate, notional);                    // notinal payment at the end

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