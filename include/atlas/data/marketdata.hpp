/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef AFBFF92A_3FC8_47D1_8AA5_E29304D51829
#define AFBFF92A_3FC8_47D1_8AA5_E29304D51829

#include <atlas/atlasconfig.hpp>
#include <functional>
#include <optional>
#include <tuple>
#include <vector>

namespace Atlas {

    struct MarketRequest {
        /**
         * @brief A struct representing a rate request.
         */
        struct ForwardRate {
            Date startDate_; /**< The start date of the rate request. */
            Date endDate_;   /**< The end date of the rate request. */
            size_t curve_;   /**< The index of the curve used to calculate the rate. */

            /**
             * @brief Constructs a Rate object with the specified parameters.
             * @param curve The index of the curve used to calculate the rate.
             * @param startDate The start date of the rate request.
             * @param endDate The end date of the rate request.
             */
            ForwardRate(const Date& startDate, const Date& endDate, size_t curve) : startDate_(startDate), endDate_(endDate), curve_(curve){};

            bool operator==(const ForwardRate& other) const {
                return startDate_ == other.startDate_ && endDate_ == other.endDate_ && curve_ == other.curve_;
            }
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

            bool operator==(const DiscountFactor& other) const { return date_ == other.date_ && curve_ == other.curve_; }
        };

        /**
         * @brief
         *
         */
        struct Spot {
            size_t ccy_;         /**< The index of the currency used to calculate the spot. */
            Date date_ = Date(); /**< The date of the spot request. */

            Spot(size_t ccy, const Date& date) : ccy_(ccy), date_(date) {}
            bool operator==(const Spot& other) const { return ccy_ == other.ccy_ && date_ == other.date_; }
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
        std::vector<ForwardRate> fwds;   /**< A vector of forward rate requests. */
        std::vector<Spot> spots;         /**< A vector of spot requests. */
    };

    /**
     * @brief A struct representing market data used for financial calculations.
     */
    template <typename adouble>
    struct MarketData {
        Date refDate;                ///< The reference date for the market data.
        std::vector<adouble> dfs;    ///< The vector of discount factors.
        std::vector<adouble> fwds;   ///< The vector of forward rates.
        std::vector<adouble> spots;  ///< The vector of spot rates.
        adouble numerarie = 1;       ///< The numerarie used in financial calculations.

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
            std::fill(dfs.begin(), dfs.end(), 0.0);
            std::fill(fwds.begin(), fwds.end(), 0.0);
            std::fill(spots.begin(), spots.end(), 0.0);
        };
    };

}  // namespace Atlas

// namespace Atlas
namespace std {
    template <>
    struct hash<Atlas::MarketRequest::DiscountFactor> {
        size_t operator()(const Atlas::MarketRequest::DiscountFactor& df) const {
            return std::hash<size_t>()(df.date_.serialNumber()) ^ std::hash<size_t>()(df.curve_);
        }
    };

    template <>
    struct hash<Atlas::MarketRequest::ForwardRate> {
        size_t operator()(const Atlas::MarketRequest::ForwardRate& rate) const {
            return std::hash<size_t>()(rate.startDate_.serialNumber()) ^ std::hash<size_t>()(rate.endDate_.serialNumber()) ^
                   std::hash<size_t>()(rate.curve_);
        }
    };

    template <>
    struct hash<Atlas::MarketRequest::Spot> {
        size_t operator()(const Atlas::MarketRequest::Spot& spot) const {
            return std::hash<size_t>()(spot.date_.serialNumber()) ^ std::hash<size_t>()(spot.ccy_);
        }
    };
}  // namespace std

#endif /* AFBFF92A_3FC8_47D1_8AA5_E29304D51829 */
