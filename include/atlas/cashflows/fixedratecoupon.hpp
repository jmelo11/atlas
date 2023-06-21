#ifndef A246223F_01CB_4480_9953_6D8FAC75CE3D
#define A246223F_01CB_4480_9953_6D8FAC75CE3D

#include <atlas/atlasconfig.hpp>
#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    /**
     * @class FixedRateCoupon
     * @brief Fixed rate coupon class
     * @ingroup Cashflows
     *
     * @tparam adouble The type of the floating point number used in the coupon
     */
    template <typename adouble>
    class FixedRateCoupon : public Coupon<adouble> {
       public:
        /**
         * @brief Construct a new Fixed Rate Coupon object
         *
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
         * @brief Construct a new Fixed Rate Coupon object
         *
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

        /**
         * @brief Gets the interest rate of the coupon
         *
         * @return The interest rate of the coupon
         */
        inline InterestRate<adouble> rate() const { return rate_; };

        /**
         * @brief Sets the interest rate of the coupon
         *
         * @param rate The interest rate of the coupon
         */
        inline void rate(const InterestRate<adouble>& rate) {
            rate_         = rate;
            this->amount_ = accruedAmount(this->startDate(), this->endDate());
        };

        /**
         * @brief Gets the day counter of the coupon
         *
         * @return The day counter of the coupon
         */
        inline DayCounter dayCounter() const override { return rate_.dayCounter(); };

        /**
         * @brief Gets the accrued period of the coupon
         * @details The accrued period is calculated considering the coupon dates. If the coupon dates are before or after the reference dates, the
         * accrued period is zero.
         *
         * @param refStart The start date of the coupon
         * @param refEnd The end date of the coupon
         * @return The accrued period of the coupon
         */
        inline double accruedPeriod(const Date& refStart, const Date& refEnd) const override {
            std::pair<Date, Date> validDates = this->checkDates(refStart, refEnd);
            return dayCounter().yearFraction(validDates.first, validDates.second);
        };

        /**
         * @brief Gets the accrued amount of the coupon
         * @details The accrued amount is calculated considering the coupon dates. If the coupon dates are before or after the reference dates, the
         * accrued amount is zero.
         *
         * @param refStart The start date of the coupon
         * @param refEnd The end date of the coupon
         * @return The accrued amount of the coupon
         */
        inline adouble accruedAmount(const Date& refStart, const Date& refEnd) const override {
            std::pair<Date, Date> validDates = this->checkDates(refStart, refEnd);
            return this->notional() * (rate_.compoundFactor(validDates.first, validDates.second) - 1.0);
        };

       private:
        InterestRate<adouble> rate_;
    };

}  // namespace Atlas

#endif /* A246223F_01CB_4480_9953_6D8FAC75CE3D */
