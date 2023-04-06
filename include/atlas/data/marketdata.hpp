/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef AFBFF92A_3FC8_47D1_8AA5_E29304D51829
#define AFBFF92A_3FC8_47D1_8AA5_E29304D51829

#include <atlas/atlasconfig.hpp>
#include <optional>
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
         * @brief
         *
         */
        struct Spot {
            size_t ccyCode_;     /**< The index of the currency used to calculate the spot. */
            Date date_ = Date(); /**< The date of the spot request. */
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

        /**
         * @brief Returns the index of the most recently added spot rate.
         * @return The index of the most recently added spot rate.
         */
        size_t spotIdx() const { return spots.size() - 1; }

        std::vector<DiscountFactor> dfs; /**< A vector of discount factor requests. */
        std::vector<Rate> fwds;          /**< A vector of forward rate requests. */
        std::vector<Spot> spots;         /**< A vector of spot requests. */
    };

    /**
     * @brief A struct representing market data used for financial calculations.
     */
    template <typename adouble>
    struct MarketData {
        Date refDate;                ///< The reference date for the market data.
        adouble numerarie;           ///< The numerarie used in financial calculations.
        std::vector<adouble> dfs;    ///< The vector of discount factors.
        std::vector<adouble> fwds;   ///< The vector of forward rates.
        std::vector<adouble> spots;  ///< The vector of spot rates.

        /**
         * @brief Allocates memory for the discount factors and forward rates according to the given market request.
         *
         * @param marketRequest The market request for which to allocate memory.
         */
        inline void allocate(const MarketRequest& marketRequest) {
            dfs.reserve(marketRequest.dfs.size());
            fwds.reserve(marketRequest.fwds.size());
            spots.reserve(marketRequest.spots.size());
        };

        /**
         * @brief Initializes the discount factors and forward rates to 1.
         */
        inline void initialize() {
            numerarie = 1;
            std::fill(dfs.begin(), dfs.end(), 0.0);
            std::fill(fwds.begin(), fwds.end(), 0.0);
            std::fill(spots.begin(), spots.end(), 0.0);
        };
    };

}  // namespace Atlas

#endif /* AFBFF92A_3FC8_47D1_8AA5_E29304D51829 */
