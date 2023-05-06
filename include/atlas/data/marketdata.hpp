/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef AFBFF92A_3FC8_47D1_8AA5_E29304D51829
#define AFBFF92A_3FC8_47D1_8AA5_E29304D51829

#include <atlas/atlasconfig.hpp>
#include <atlas/data/marketrequest.hpp>

namespace Atlas {

    /**
     * @brief A struct representing market data used for financial calculations.
     */
    template <typename adouble>
    struct MarketData {
        Date refDate;                 ///< The reference date for the market data.
        std::vector<adouble> dfs;     ///< The vector of discount factors.
        std::vector<adouble> fwds;    ///< The vector of forward rates.
        std::vector<adouble> prices;  ///< The vector of spot prices.
        std::vector<adouble> fxs;      ///< The vector of fx prices.
        adouble numerarie = 1;        ///< The numerarie used in financial calculations.

        /**
         * @brief Allocates memory for the discount factors and forward rates according to the given market request.
         *
         * @param marketRequest The market request for which to allocate memory.
         */
        inline void allocate(const MarketRequest& marketRequest) {
            dfs.reserve(marketRequest.dfs.size());
            fwds.reserve(marketRequest.fwds.size());
            prices.reserve(marketRequest.prices.size());
            fxs.reserve(marketRequest.fxs.size());
        };

        /**
         * @brief Initializes the discount factors and forward rates to 1.
         */
        inline void initialize() {
            std::fill(dfs.begin(), dfs.end(), 0.0);
            std::fill(fwds.begin(), fwds.end(), 0.0);
            std::fill(prices.begin(), prices.end(), 0.0);
            std::fill(fxs.begin(), fxs.end(), 0.0);
        };
    };

}  // namespace Atlas

#endif /* AFBFF92A_3FC8_47D1_8AA5_E29304D51829 */
