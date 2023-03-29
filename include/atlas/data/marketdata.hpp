/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef AFBFF92A_3FC8_47D1_8AA5_E29304D51829
#define AFBFF92A_3FC8_47D1_8AA5_E29304D51829

#include <atlas/atlasconfig.hpp>
#include <tuple>
#include <vector>

namespace Atlas {

    struct MarketRequest {
        /**
         * @brief A struct representing a rate request.
         */
        struct Rate {
            Date startDate_;          /**< The start date of the rate request. */
            Date endDate_;            /**< The end date of the rate request. */
            DayCounter dayCounter_;   /**< The day counter used to calculate the rate. */
            Compounding compounding_; /**< The compounding frequency used to calculate the rate. */
            Frequency frequency_;     /**< The frequency of the rate. */
            size_t curve_;            /**< The index of the curve used to calculate the rate. */

            /**
             * @brief Constructs a Rate object with the specified parameters.
             * @param curve The index of the curve used to calculate the rate.
             * @param startDate The start date of the rate request.
             * @param endDate The end date of the rate request.
             * @param dayCounter The day counter used to calculate the rate. Default is Actual360.
             * @param compounding The compounding frequency used to calculate the rate. Default is Compounding::Simple.
             * @param frequency The frequency of the rate. Default is Frequency::Annual.
             */
            Rate(const size_t& curve, const Date& startDate, const Date& endDate, const DayCounter& dayCounter = Actual360(),
                 const Compounding& compounding = Compounding::Simple, const Frequency& frequency = Frequency::Annual)
            : startDate_(startDate), endDate_(endDate), dayCounter_(dayCounter), compounding_(compounding), frequency_(frequency), curve_(curve){};
        };

        /**
         * @brief A struct representing a discount factor request.
         */
        struct DiscountFactor {
            Date date_;    /**< The date of the discount factor request. */
            size_t curve_; /**< The index of the curve used to calculate the discount factor. */

            /**
             * @brief Constructs a DiscountFactor object with the specified parameters.
             * @param curve The index of the curve used to calculate the discount factor.
             * @param date The date of the discount factor request.
             */
            DiscountFactor(size_t curve, const Date& date) : date_(date), curve_(curve) {}
        };

        /**
         * @brief Returns the index of the most recently added discount factor.
         * @return The index of the most recently added discount factor.
         */
        size_t discountFactorIdx() const { return dfs.size() - 1; }

        /**
         * @brief Returns the index of the most recently added forward rate.
         * @return The index of the most recently added forward rate.
         */
        size_t forwardRateIdx() const { return fwds.size() - 1; }

        std::vector<DiscountFactor> dfs; /**< A vector of discount factor requests. */
        std::vector<Rate> fwds;          /**< A vector of forward rate requests. */
    };

    /**
     * @brief A struct representing market data used for financial calculations.
     */
    struct MarketData {
        Date refDate;              ///< The reference date for the market data.
        adouble numerarie = 1;      ///< The numerarie used in financial calculations.
        std::vector<adouble> dfs;   ///< The vector of discount factors.
        std::vector<adouble> fwds;  ///< The vector of forward rates.

        /**
         * @brief Allocates memory for the discount factors and forward rates according to the given market request.
         *
         * @param marketRequest The market request for which to allocate memory.
         */
        void allocate(const MarketRequest& marketRequest);

        /**
         * @brief Initializes the discount factors and forward rates to 1.
         */
        void initialize();
    };

}  // namespace Atlas

#endif /* AFBFF92A_3FC8_47D1_8AA5_E29304D51829 */
