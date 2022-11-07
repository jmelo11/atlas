#include <ql/time/schedule.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedbulletproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FixedBulletProduct::FixedBulletProduct(const QuantLib::Date& start, const QuantLib::Date& end,
                                           QuantLib::Frequency freq, double notional,
                                           QuantLib::InterestRate rate) {
        QuantLib::Schedule schedule =
            QuantLib::MakeSchedule().from(start).to(end).withFrequency(freq);

        std::vector<Cashflow> cashflows;
        QuantLib::Date firstDate = QuantLib::Date();
        for (const auto& endDate : schedule.dates()) {
            if (firstDate != QuantLib::Date()) {
                FixedRateCoupon coupon(firstDate, endDate, notional, rate);
                cashflows.push_back(coupon);
            }
            firstDate = endDate;
        }

        Redemption redemption(schedule.endDate(), notional);
        cashflows.push_back(redemption);
        legs_.push_back(Leg(cashflows));
    }

    void FixedBulletProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void FixedBulletProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas