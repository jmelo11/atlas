#ifndef C5139B34_BABA_4D42_B2D5_CD375677E6A8
#define C5139B34_BABA_4D42_B2D5_CD375677E6A8

#include <atlas/cashflows/coupon.hpp>
#include <atlas/rates/index/ratedefinition.hpp>

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
    class FloatingRateCoupon : public Coupon<adouble> {
       public:
        /**
         * @brief Construct a new Floating Rate Coupon object
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param rateDef The rate definition of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, adouble spread, const RateDefinition& rateDef)
        : Coupon<adouble>(startDate, endDate, notional), spread_(spread), rateDef_(rateDef){};

        /**
         * @brief Construct a new Floating Rate Coupon object
         *
         * @param startDate The start date of the coupon
         * @param endDate The end date of the coupon
         * @param notional The notional amount of the coupon
         * @param spread The spread of the coupon
         * @param rateDef The rate definition of the coupon
         * @param indexContextIdx The forecast CurveContext of the coupon
         * @param discountContextIdx The discount CurveContext of the coupon
         */
        FloatingRateCoupon(const Date& startDate, const Date& endDate, double notional, adouble spread, const RateDefinition& rateDef,
                           size_t indexContextIdx, size_t discountContextIdx)
        : FloatingRateCoupon(startDate, endDate, notional, spread, rateDef) {
            this->indexContextIdx(indexContextIdx);
            this->discountContextIdx(discountContextIdx);
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

        /**
         * @brief Sets the forecast CurveContext of the coupon
         *
         * @param idx The forecast CurveContext of the coupon
         */
        void indexContextIdx(size_t idx) {
            if (idx == SIZE_MAX) throw std::invalid_argument("Invalid index context idx");
            indexContextIdx_    = idx;
            hasIndexContextIdx_ = true;
        }

        /**
         * @brief Gets the forecast curve context idx
         *
         * @return size_t The forecast curve context idx
         */
        inline size_t indexContextIdx() const { return indexContextIdx_; }

        /**
         * @brief Gets the day counter of the coupon
         *
         * @return The day counter of the coupon
         */
        inline const DayCounter& dayCounter() const override { return rateDef_.dayCounter(); };

        /**
         * @brief Gets the accrued period of the coupon
         *
         * @param refStart The start evaluation date for the accrual period
         * @param refEnd The end evaluation date for the accrual period
         * @return The accrued period of the coupon
         */
        inline double accruedPeriod(const Date& refStart, const Date& refEnd) const override {
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
        inline adouble accruedAmount(const Date& refStart, const Date& refEnd) const override {
            auto datesPair = this->accrualDates(refStart, refEnd);
            if (refStart >= this->endDate() || refEnd <= this->startDate()) return 0.0;
            if (!isFixingSet()) throw std::runtime_error("Fixing rate not set");
            adouble totalRate  = fixing_ + spread_;
            adouble compFactor = fastCompoundFactor<adouble>(totalRate, rateDef_.dayCounter(), rateDef_.compounding(), rateDef_.frequency(),
                                                             datesPair.first, datesPair.second);
            return this->notional() * (compFactor - 1.0);
        };

        /**
         * @brief Checks if the coupon has a forecast CurveContext
         *
         * @return true If the coupon has a forecast CurveContext
         */
        inline bool hasIndexContextIdx() const { return hasIndexContextIdx_; }

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
        adouble spread_          = 0.0;
        adouble fixing_          = 0.0;
        size_t indexContextIdx_  = SIZE_MAX;
        bool hasIndexContextIdx_ = false;
        bool isFixingSet_        = false;
        RateDefinition rateDef_;
    };
}  // namespace Atlas

#endif /* C5139B34_BABA_4D42_B2D5_CD375677E6A8 */
