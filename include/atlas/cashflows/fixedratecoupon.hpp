#ifndef A246223F_01CB_4480_9953_6D8FAC75CE3D
#define A246223F_01CB_4480_9953_6D8FAC75CE3D

#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    /***
     * Fixed rate coupon class
    */
    class FixedRateCoupon : public Coupon {
       public:

        /***
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param rate The interest rate of the coupon
        */
        FixedRateCoupon(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate);

        DayCounter dayCounter() const override;
        
        double accruedAmount(const Date& start, const Date& end) const override;

        /***
         * Gets the interest rate of the coupon
         * @return The interest rate of the coupon
        */
        InterestRate rate() const;

        /***
         * Sets the interest rate of the coupon
         * @param rate The interest rate of the coupon
        */
        void rate(const InterestRate& rate);

       private:
        InterestRate rate_;
    };

}  // namespace Atlas

#endif /* A246223F_01CB_4480_9953_6D8FAC75CE3D */
