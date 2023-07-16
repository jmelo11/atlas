#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>
#include <atlas/rates/rateindex.hpp>

namespace Atlas {

    /**
     * @class FloatingRateCoupon
     * @brief A class representing a floating rate coupon
     * @details A floating rate coupon has a start date, an end date, a notional amount, a spread, and a rate index
     * @ingroup Cashflows
     *
     * @tparam adouble The type of the floating point number used in the coupon
     */
    template <typename adouble>
    class FloatingRateCoupon : public BaseCoupon<adouble, FloatingRateCoupon> {
       public:
        /**
         * Constructor
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param rateIndexContext The forecast CurveContext of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, adouble spread,
                           const Context<RateIndex<adouble>>& rateIndexContext)
        : BaseCoupon<adouble, FloatingRateCoupon>(startDate, endDate, notional),
          spread_(spread),
          rateIndexContextIdx_(rateIndexContext.idx()),
          hasRateIndexContext_(true) {
            rateDef_ = {rateIndexContext.object().dayCounter(), rateIndexContext.object().rateFrequency(),
                        rateIndexContext.object().rateCompounding()};
        };

        /**
         * @brief Construct a new Floating Rate Coupon object
         *
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param rateIndexContext The forecast CurveContext of the coupon
         * @param discountCurveContext The discount CurveContext of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, adouble spread,
                           const Context<RateIndex<adouble>>& rateIndexContext, const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : FloatingRateCoupon(startDate, endDate, notional, spread, rateIndexContext) {
            this->discountContextIdx_ = discountCurveContext.idx();
            this->hasDiscountContext_ = true;
        };

        /**
         * @brief Returns the spread of the coupon
         *
         * @return The spread of the coupon
         */
        inline adouble spread() const { return spread_; }

        /**
         * @brief Sets the spread of the coupon
         *
         * @param spread The spread of the coupon
         */
        inline void spread(adouble spread) {
            spread_       = spread;
            this->amount_ = accruedAmount(this->startDate(), this->endDate());
        }

        /**
         * @brief Sets the fixing of the coupon
         *
         * @param fixing The fixing of the coupon
         */
        inline void fixing(adouble fixing) {
            fixing_       = fixing;
            isFixingSet_  = true;
            this->amount_ = accruedAmount(this->startDate(), this->endDate());
        };

        /**
         * @brief Gets the fixing of the coupon
         *
         * @return The fixing of the coupon
         */
        inline adouble fixing() const { return fixing_; }

        /***
         * Sets the forecast CurveContext of the coupon
         * @param rateIndexContext The forecast CurveContext of the coupon
         */
        void rateIndexContext(const Context<RateIndex<adouble>>& context) {
            rateIndexContextIdx_ = context.idx();
            auto& index          = context.object();
            rateDef_             = {index.dayCounter(), index.rateFrequency(), index.rateCompounding()};
            hasRateIndexContext_ = true;
        }

        /**
         * @brief Gets the day counter of the coupon
         *
         * @return The day counter of the coupon
         */
        inline DayCounter dayCounter() const { return rateDef_.dayCounter; };

        /**
         * @brief Gets the accrued period of the coupon
         *
         * @param refStart The start evaluation date for the accrual period
         * @param refEnd The end evaluation date for the accrual period
         * @return The accrued period of the coupon
         */
        inline double accruedPeriod(const Date& refStart, const Date& refEnd) const {
            if (refStart >= this->endDate() || refEnd <= this->startDate()) return 0.0;
            auto datesPair = this->accrualDates(refStart, refEnd);
            return dayCounter().yearFraction(datesPair.first, datesPair.second);
        };

        /**
         * @brief Gets the accrued amount of the coupon
         *
         * @param refStart The start evaluation date for the accrual period
         * @param refEnd The end evaluation date for the accrual period
         * @return The accrued amount of the coupon
         */
        inline adouble accruedAmount(const Date& refStart, const Date& refEnd) const {
            auto datesPair = this->accrualDates(refStart, refEnd);
            if (refStart >= this->endDate() || refEnd <= this->startDate()) return 0.0;
            if (!isFixingSet()) throw std::runtime_error("Fixing rate not set");
            adouble totalRate = fixing_ + spread_;
            adouble compFactor =
                fastCompoundFactor<adouble>(totalRate, rateDef_.dayCounter, rateDef_.comp, rateDef_.freq, datesPair.first, datesPair.second);
            return this->notional() * (compFactor - 1.0);
        };

        /**
         * @brief Checks if the coupon has a forecast CurveContext
         *
         * @return true If the coupon has a forecast CurveContext
         */
        inline bool hasRateIndexContext() const { return hasRateIndexContext_; }

        /**
         * @brief Gets the forecast curve context idx
         *
         * @return size_t The forecast curve context idx
         */
        inline size_t rateIndexContextIdx() const { return rateIndexContextIdx_; }

        /**
         * @brief Checks if the fixing rate has been set
         *
         * @return true If the fixing rate has been set, false otherwise
         */
        inline bool isFixingSet() const { return isFixingSet_; }

        /**
         * @brief Sets if the fixing has been set
         *
         * @param fixing The fixing rate
         */

       private:
        /**
         * @brief A struct representing the rate definition of the coupon
         * @details The rate definition of the coupon is the day counter, frequency, and compounding of the coupon
         */
        struct RateDef {
            DayCounter dayCounter;
            Frequency freq;
            Compounding comp;
        };

        adouble spread_ = 0.0;
        adouble fixing_ = 0.0;
        RateDef rateDef_;

        size_t rateIndexContextIdx_;
        bool isFixingSet_         = false;
        bool hasRateIndexContext_ = false;
    };
}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
