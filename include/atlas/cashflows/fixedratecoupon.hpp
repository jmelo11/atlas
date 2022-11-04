#ifndef A246223F_01CB_4480_9953_6D8FAC75CE3D
#define A246223F_01CB_4480_9953_6D8FAC75CE3D

#include <ql/interestrate.hpp>
#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    class FixedRateCoupon : public Coupon {
       public:
        FixedRateCoupon(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                        double notional, QuantLib::InterestRate rate)
        : Coupon(startDate, endDate, notional), rate_(rate){};

        QuantLib::DayCounter dayCounter() const override;

        double accruedAmount(const QuantLib::Date& start, const QuantLib::Date& end) const override;

        QuantLib::InterestRate rate() const;

        void rate(QuantLib::InterestRate rate);

        double amount() const override;

       

       private:
        QuantLib::InterestRate rate_;
    };

}  // namespace Atlas

#endif /* A246223F_01CB_4480_9953_6D8FAC75CE3D */
