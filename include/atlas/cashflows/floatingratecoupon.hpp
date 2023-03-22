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
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread,
                           std::shared_ptr<CurveContext> forecastCurveContext, std::shared_ptr<CurveContext> discountCurveContext = nullptr);
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

        void fixing(double fixing);

        double fixing() const { return fixing_; }

        /***
         * @return The forecast CurveContext of the coupon
         */
        std::shared_ptr<CurveContext> forecastCurveContext() const { return forecastCurveContext_; }

        /***
         * Sets the forecast CurveContext of the coupon
         * @param forecastCurveContext The forecast CurveContext of the coupon
         */
        void forecastCurveContext(std::shared_ptr<CurveContext> context) { forecastCurveContext_ = context; }

        /***
         * Gets the day counter of the coupon
         * @return The day counter of the coupon
         */
        DayCounter dayCounter() const override;

        /***
         * Gets the accrued period of the coupon
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued period of the coupon
         */
        double accruedPeriod(const Date& start, const Date& end) const override;

        /***
         * Gets the accrued amount of the coupon
         * @param start The start date of the coupon
         * @param end The end date of the coupon
         * @return The accrued amount of the coupon
         */
        double accruedAmount(const Date& start, const Date& end) const override;

       private:
        double spread_ = 0.0;
        double fixing_ = 0.0;
        std::shared_ptr<CurveContext> forecastCurveContext_;
    };
}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
