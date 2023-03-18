#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>
#include <atlas/rates/rateindex.hpp>

namespace Atlas {

    /***
     * @brief A class representing a floating rate coupon
     * @details A floating rate coupon has a start date, an end date, a notional amount, a spread, and a rate index
     */
    class FloatingRateCoupon : public Coupon {
       public:
        /***
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param index Pointer to the rate index of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread);
        /***
         * @return The spread of the coupon
         */
        double spread() const { return spread_; }

        /***
         * Sets the spread of the coupon
         * @param spread The spread of the coupon
         */
        void spread(double spread) { spread_ = spread; }

        /***
         * Sets amount of the coupon since it is not calculated by the coupon itself
         */
        void amount(double amount) { amount_ = amount; }

        double amount() const { return amount_; }

        void forecastCurveIdx(size_t forecastCurveIdx) { forecastCurveIdx_ = forecastCurveIdx; }

        size_t forecastCurveIdx() const { return forecastCurveIdx_; }

        void fixing(double fixing) { fixing_ = fixing; }

        double fixing() const { return fixing_; }

       private:
        double spread_ = 0.0;
        double fixing_ = 0.0;  // pending
        size_t forecastCurveIdx_;
    };
}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
