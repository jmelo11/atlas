#ifndef A246223F_01CB_4480_9953_6D8FAC75CE3D
#define A246223F_01CB_4480_9953_6D8FAC75CE3D

#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    /**
     * @brief Fixed rate coupon class
     * @tparam adouble The type of the floating point number used in the coupon
     */
    template <typename adouble>
    class FixedRateCoupon : public Coupon<adouble> {
       public:
        /**
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param rate The interest rate of the coupon
         */
        FixedRateCoupon(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble>& rate)
        : Coupon<adouble>(startDate, endDate, notional), rate_(rate) {
            this->amount_ = accruedAmount(startDate, endDate);
        };

        /**
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param rate The interest rate of the coupon
         */
        FixedRateCoupon(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble>& rate,
                        const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : Coupon<adouble>(startDate, endDate, notional, discountCurveContext), rate_(rate) {
            this->amount_ = accruedAmount(startDate, endDate);
        };

        /***
         * Gets the interest rate of the coupon
         * @return The interest rate of the coupon
         */
        inline InterestRate<adouble> rate() const { return rate_; };

        /***
         * Sets the interest rate of the coupon
         * @param rate The interest rate of the coupon
         */
        inline void rate(const InterestRate<adouble>& rate) {
            rate_         = rate;
            this->amount_ = accruedAmount(this->startDate(), this->endDate());
        };

        /***
         * Gets the day counter of the coupon
         * @return The day counter of the coupon
         */
        inline DayCounter dayCounter() const override { return rate_.dayCounter(); };

        /***
         * Gets the accrued period of the coupon
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued period of the coupon
         */
        inline double accruedPeriod(const Date& start, const Date& end) const override { return dayCounter().yearFraction(start, end); };

        /***
         * Gets the accrued amount of the coupon
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued amount of the coupon
         */
        inline adouble accruedAmount(const Date& start, const Date& end) const override {
            return this->notional() * (rate_.compoundFactor(start, end) - 1.0);
        };

       private:
        InterestRate<adouble> rate_;
    };

}  // namespace Atlas

#endif /* A246223F_01CB_4480_9953_6D8FAC75CE3D */
