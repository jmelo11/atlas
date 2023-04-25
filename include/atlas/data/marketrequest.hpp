#ifndef CFEA503E_8365_4F82_BD67_821346F85498
#define CFEA503E_8365_4F82_BD67_821346F85498

#include <atlas/atlasconfig.hpp>

namespace Atlas {
    struct MarketRequest {
        /**
         * @brief A struct representing a rate request.
         * @details A rate request is a container for the start date, end date and the curve index.
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
         * @details A discount factor request is a container for the date and the curve index.
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
         * @brief A struct representing a price request.
         * @details A price request is a container for the currency code and the date.
         */
        struct Price {
            size_t price_;       /**< The index of the currency used to calculate the price. */
            Date date_ = Date(); /**< The date of the spot request. */

            Price(size_t price, const Date& date) : price_(price), date_(date) {}
            bool operator==(const Price& other) const { return price_ == other.price_ && date_ == other.date_; }
        };

        /**
         * @brief A struct representing a fx price request.
         * @details A fx price request is a container for the currency codes and the date.
         */
        struct FxPrice {
            size_t ccy1_;
            size_t ccy2_;
            Date date_;
            FxPrice(size_t ccy1, size_t ccy2, const Date& date) : ccy1_(ccy1), ccy2_(ccy2), date_(date) {}
            bool operator==(const FxPrice& other) const { return ccy1_ == other.ccy1_ && ccy2_ == other.ccy2_ && date_ == other.date_; }
        };

        std::vector<DiscountFactor> dfs; /**< A vector of discount factor requests. */
        std::vector<ForwardRate> fwds;   /**< A vector of forward rate requests. */
        std::vector<Price> prices;       /**< A vector of spot requests. */
        std::vector<FxPrice> fxPrices;   /**< A vector of fx price requests. */
    };
}  // namespace Atlas

namespace std {

    /**
     * @brief A hash function for the DiscountFactor struct.
     *
     * @tparam
     */
    template <>
    struct hash<Atlas::MarketRequest::DiscountFactor> {
        size_t operator()(const Atlas::MarketRequest::DiscountFactor& df) const {
            return std::hash<size_t>()(df.date_.serialNumber()) ^ std::hash<size_t>()(df.curve_);
        }
    };

    /**
     * @brief A hash function for the ForwardRate struct.
     *
     * @tparam
     */
    template <>
    struct hash<Atlas::MarketRequest::ForwardRate> {
        size_t operator()(const Atlas::MarketRequest::ForwardRate& rate) const {
            return std::hash<size_t>()(rate.startDate_.serialNumber()) ^ std::hash<size_t>()(rate.endDate_.serialNumber()) ^
                   std::hash<size_t>()(rate.curve_);
        }
    };

    /**
     * @brief A hash function for the Price struct.
     *
     * @tparam
     */
    template <>
    struct hash<Atlas::MarketRequest::Price> {
        size_t operator()(const Atlas::MarketRequest::Price& price) const {
            return std::hash<size_t>()(price.date_.serialNumber()) ^ std::hash<size_t>()(price.price_);
        }
    };

    /**
     * @brief A hash function for the FxPrice struct.
     *
     * @tparam
     */
    template <>
    struct hash<Atlas::MarketRequest::FxPrice> {
        size_t operator()(const Atlas::MarketRequest::FxPrice& fx) const {
            return std::hash<size_t>()(fx.date_.serialNumber()) ^ std::hash<size_t>()(fx.ccy1_) ^ std::hash<size_t>()(fx.ccy2_);
        }
    };
}  // namespace std

#endif /* CFEA503E_8365_4F82_BD67_821346F85498 */
