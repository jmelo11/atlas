/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#include <atlas/data/marketdata.hpp>

namespace Atlas {

    void MarketData::allocate(const MarketRequest& marketRequest) {
        dfs.reserve(marketRequest.dfs.size());
        fwds.reserve(marketRequest.fwds.size());
    }

    void MarketData::initialize() {
        numerarie = 1;
        std::fill(dfs.begin(), dfs.end(), 0.0);
        std::fill(fwds.begin(), fwds.end(), 0.0);
    }

}  // namespace atlas
