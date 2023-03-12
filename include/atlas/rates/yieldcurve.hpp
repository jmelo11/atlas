#ifndef C9383CFF_FA55_4061_A633_C8B6F16B0878
#define C9383CFF_FA55_4061_A633_C8B6F16B0878

#include <atlas/atlasconfig.hpp>

namespace Atlas {
    /**
     * @class YieldCurve
     * A yield curve represents the term structure of interest rates.
     * It extends the DiscountCurve class and adds an index for the underlying interest rate.
     */
    class YieldCurve : public DiscountCurve {
       public:
        /**
         * Constructs a yield curve from a vector of dates and a vector of corresponding rates.
         * @param dates A vector of dates representing the maturities of the interest rates.
         * @param rates A vector of rates corresponding to the maturities in dates.
         * @param dayCounter The day counter used to calculate the time between dates.
         */
        YieldCurve(const std::vector<Date>& dates, const std::vector<double>& rates, const DayCounter& dayCounter)
        : DiscountCurve(dates, rates, dayCounter){};

        /**
         * Constructs a yield curve from a vector of dates, a vector of corresponding rates, and a rate index.
         * @param dates A vector of dates representing the maturities of the interest rates.
         * @param rates A vector of rates corresponding to the maturities in dates.
         * @param dayCounter The day counter used to calculate the time between dates.
         * @param index The rate index used as the underlying interest rate.
         */
        YieldCurve(const std::vector<Date>& dates, const std::vector<double>& rates, const DayCounter& dayCounter, std::shared_ptr<RateIndex> index)
        : DiscountCurve(dates, rates, dayCounter), index_(index){};

        /**
         * Sets the rate index used as the underlying interest rate.
         * @param index The rate index to set.
         */
        void index(std::shared_ptr<RateIndex> index) { index_ = index; }

        /**
         * Returns the rate index used as the underlying interest rate.
         * @return The rate index used as the underlying interest rate.
         */
        std::shared_ptr<RateIndex> index() const { return index_; }

       private:
        std::shared_ptr<RateIndex> index_; /**< The rate index used as the underlying interest rate. */
    };

}  // namespace Atlas

#endif /* C9383CFF_FA55_4061_A633_C8B6F16B0878 */
