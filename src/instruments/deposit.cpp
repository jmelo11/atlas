
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/deposit.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    Deposit::Deposit(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double amount,
                     QuantLib::InterestRate rate) {
        FixedRateCoupon coupon(startDate, endDate, amount, rate);  // interest coupon
        Redemption redemption(endDate, amount);                    // notinal payment at the end
        std::vector<Cashflow> cashflows;
        cashflows.push_back(coupon);
        cashflows.push_back(redemption);
        legs_.push_back(Leg(cashflows));
    }

    void Deposit::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void Deposit::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas