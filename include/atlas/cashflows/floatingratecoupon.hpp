#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>
#include <atlas/rates/rateindex.hpp>

namespace Atlas {

    /***
     * @brief A class representing a floating rate coupon
     * @details A floating rate coupon has a start date, an end date, a notional amount, a spread, and a rate index
     * @tparam adouble The type of the floating point number used in the coupon
     */
    template <typename adouble>
    class FloatingRateCoupon : public Coupon<adouble> {
       public:
        /**
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param forecastCurveContext The forecast CurveContext of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, adouble spread, const CurveContext& forecastCurveContext)
        : Coupon<adouble>(startDate, endDate, notional), spread_(spread), forecastContextIdx_(forecastCurveContext.idx()), hasForecastContext_(true) {
            rateDef_ = {forecastCurveContext.index().dayCounter(), forecastCurveContext.index().rateFrequency(),
                        forecastCurveContext.index().rateCompounding()};
        };

        /**
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param forecastCurveContext The forecast CurveContext of the coupon
         * @param discountCurveContext The discount CurveContext of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, adouble spread, const CurveContext& forecastCurveContext,
                           const CurveContext& discountCurveContext)
        : FloatingRateCoupon(startDate, endDate, notional, spread, forecastCurveContext) {
            this->discountContextIdx_ = discountCurveContext.idx();
            this->hasDiscountContext_ = true;
        };

        /***
         * @return The spread of the coupon
         */
        inline adouble spread() const { return spread_; }

        /***
         * Sets the spread of the coupon
         * @param spread The spread of the coupon
         */
        inline void spread(adouble spread) {
            spread_       = spread;
            this->amount_ = accruedAmount(this->startDate(), this->endDate());
        }

        /***
         * Sets the fixing of the coupon
         * @param fixing The fixing of the coupon
         */
        inline void fixing(adouble fixing) {
            fixing_       = fixing;
            this->amount_ = accruedAmount(this->startDate(), this->endDate());
        };

        /***
         * Gets the fixing of the coupon
         * @return The fixing of the coupon
         */
        inline adouble fixing() const { return fixing_; }

        /***
         * Sets the forecast CurveContext of the coupon
         * @param forecastCurveContext The forecast CurveContext of the coupon
         */
        void forecastCurveContext(const CurveContext& context) {
            forecastContextIdx_ = context.idx();
            rateDef_            = {context.index().dayCounter(), context.index().rateFrequency(), context.index().rateCompounding()};
            hasForecastContext_ = true;
        }

        /***
         * Gets the day counter of the coupon
         * @return The day counter of the coupon
         */
        inline DayCounter dayCounter() const override { return rateDef_.dayCounter; };

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
            adouble totalRate = fixing_ + spread_;
            InterestRate r(totalRate, rateDef_.dayCounter, rateDef_.comp, rateDef_.freq);
            return this->notional() * (r.compoundFactor(start, end) - 1.0);
        };

        /**
         * @brief Checks if the coupon has a forecast CurveContext
         *
         * @return true If the coupon has a forecast CurveContext
         */
        inline bool hasForecastContext() const { return hasForecastContext_; }

        /**
         * @brief Gets the forecast curve context idx
         *
         * @return size_t The forecast curve context idx
         */
        inline size_t forecastContextIdx() const { return forecastContextIdx_; }

       private:
        struct RateDef {
            DayCounter dayCounter;
            Frequency freq;
            Compounding comp;
        };

        adouble spread_ = 0.0;
        adouble fixing_ = 0.0;
        RateDef rateDef_;

        size_t forecastContextIdx_;
        bool hasForecastContext_ = false;
    };
}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
