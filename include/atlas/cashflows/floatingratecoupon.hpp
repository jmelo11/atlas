#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/basictypes/rateindex.hpp>
#include <atlas/cashflows/coupon.hpp>

namespace Atlas {

    /***
     * Floating rate coupon class
     */
    class FloatingRateCoupon : public Coupon {
       public:
        /***
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param index The rate index of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, double spread, const RateIndex& index);

        /***
         * @return The spread of the coupon
         */
        double spread() const { return spread_; }

        /***
         * Sets the spread of the coupon
         * @param spread The spread of the coupon
         */
        void spread(double spread) {
            spread_ = spread;
            amount_ = accruedAmount(startDate(), endDate());
        }

        /***
         * Sets the base fixing of the coupon and calculates the accrued amount
         * @param value The fixing of the coupon
         */
        void fixing(double value) {
            fixing_ = value;
            amount_ = accruedAmount(startDate(), endDate());  // should be lazy?
        }

        /***
         * @return The fixing of the coupon
         */
        double fixing() const { return fixing_; }

        double accruedAmount(const Date& start, const Date& end) const;

        DayCounter dayCounter() const;

        /***
         * @return The rate index of the coupon
         */
        const RateIndex& index() const { return index_; }

        /***
         * Checks if the coupon is still floating
         * @return True if the coupon is still floating
         */
        bool isFloating() const { return isFloating_; }

        /***
         * Sets the coupon to fixed coupon
         */
        void fix() { isFloating_ = false; }

       private:
        double fixing_   = 0.0;
        double spread_   = 0.0;
        bool isFloating_ = true;  // 1 byte, might cause padding
        RateIndex index_;
    };

    /***
     * This class attemps to remove RateIndex from FloatingRateCoupon as a member variable, using a pointer instead to the IndexStore and an index
     * size_t to retrieve the RateIndex from the IndexStore. This is to avoid the need to copy the RateIndex object when copying the
     * FloatingRateCoupon object.
     *
     * Also removed the isFloating_ member variable, with the idea that models should handle the fixing status.
     */

    class FloatingRateCoupon2 : public Coupon {
       public:
        /***
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param index Pointer to the rate index of the coupon
         */
        FloatingRateCoupon2(const Date& startDate, const Date& endDate, double notional, double spread)  // two parameters for the same thing
        : Coupon(startDate, endDate, notional), spread_(spread){};

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

        void indexIdx(size_t indexIdx) { indexIdx_ = indexIdx; }

        size_t indexIdx() const { return indexIdx_; }

        void forecastCurveIdx(size_t forecastCurveIdx) { forecastCurveIdx_ = forecastCurveIdx; }

        size_t forecastCurveIdx() const { return forecastCurveIdx_; }

       private:
        double spread_ = 0.0;
        size_t indexIdx_;
        size_t forecastCurveIdx_;
    };
}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
