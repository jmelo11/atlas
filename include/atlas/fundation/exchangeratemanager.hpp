#ifndef DE58DF6A_C264_4FEA_ADB5_CAD0E751958B
#define DE58DF6A_C264_4FEA_ADB5_CAD0E751958B

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/context.hpp>
#include <queue>
#include <tuple>
#include <unordered_map>

namespace Atlas {
    template <typename adouble>
    class YieldTermStructure;

    /**
     * @brief ExchangeRateManager is a class that manages a collection of exchange rates.
     *
     * @tparam adouble
     */
    template <typename adouble>
    class ExchangeRateManager {
       public:
        /**
         * @brief Construct a new Exchange Rate Manager object
         *
         * @param refDate
         */
        ExchangeRateManager(const Date& refDate) : refDate_(refDate) {}

        /**
         * @brief adds an exchange rate to the manager, i.e. the rate from weak to strong currency.
         *
         * @param weak
         * @param strong
         * @param spotRate
         */
        void addExchangeRate(const Currency& weak, const Currency& strong, adouble spotRate) {
            exchangeRateMap_[{weak.numericCode(), strong.numericCode()}] = spotRate;
        }

        /**
         * @brief returns the exchange rate from firstCcy to secondCcy.
         *
         * @param firstCcy
         * @param secondCcy
         * @return adouble
         */
        adouble exchange(size_t firstCcy, size_t secondCcy) const {
            if (firstCcy == secondCcy) { return 1.0; }

            auto cacheKey   = std::make_tuple(firstCcy, secondCcy);
            auto cachedRate = exchangeRateCache_.find(cacheKey);
            if (cachedRate != exchangeRateCache_.end()) { return cachedRate->second; }

            std::queue<std::pair<size_t, adouble>> q;
            std::unordered_set<size_t> visited;
            q.push({firstCcy, 1.0});
            visited.insert(firstCcy);

            while (!q.empty()) {
                auto [currentCcy, rate] = q.front();
                q.pop();

                for (const auto& entry : exchangeRateMap_) {
                    auto [source, dest] = entry.first;

                    if (source == currentCcy && visited.find(dest) == visited.end()) {
                        adouble new_rate = rate * entry.second;
                        if (dest == secondCcy) {
                            exchangeRateCache_[cacheKey]                             = new_rate;
                            exchangeRateCache_[std::make_tuple(secondCcy, firstCcy)] = 1.0 / new_rate;
                            return new_rate;
                        }
                        visited.insert(dest);
                        q.push({dest, new_rate});
                    } else if (dest == currentCcy && visited.find(source) == visited.end()) {
                        adouble new_rate = rate / entry.second;
                        if (source == secondCcy) {
                            exchangeRateCache_[cacheKey]                             = new_rate;
                            exchangeRateCache_[std::make_tuple(secondCcy, firstCcy)] = 1.0 / new_rate;
                            return new_rate;
                        }
                        visited.insert(source);
                        q.push({source, new_rate});
                    }
                }
            }

            throw std::runtime_error("Exchange rate not found for the given currencies");
        }

        /**
         * @brief returns the exchange rate from firstCcy to secondCcy.
         *
         * @param firstCcy
         * @param secondCcy
         * @return adouble
         */
        adouble exchange(const Currency& firstCcy, const Currency& secondCcy) const {
            return exchange(firstCcy.numericCode(), secondCcy.numericCode());
        }

        /**
         * @brief detects arbitrage opportunities in the exchange rate graph.
         *
         * @return std::vector<size_t>
         */
        std::vector<size_t> detectArbitrage() const {
            // Initialize distances and predecessors
            std::unordered_map<size_t, adouble> distances;
            std::unordered_map<size_t, size_t> predecessors;
            for (const auto& [key, _] : exchangeRateMap_) {
                distances[std::get<0>(key)]    = std::numeric_limits<adouble>::infinity();
                distances[std::get<1>(key)]    = std::numeric_limits<adouble>::infinity();
                predecessors[std::get<0>(key)] = 0;
                predecessors[std::get<1>(key)] = 0;
            }
            auto startCurrency       = distances.begin()->first;
            distances[startCurrency] = 0.0;  // Set the first currency's distance to 0

            // Bellman-Ford algorithm
            size_t numCurrencies       = distances.size();
            size_t lastUpdatedCurrency = 0;
            for (size_t i = 0; i < numCurrencies - 1; ++i) {
                for (const auto& [key, value] : exchangeRateMap_) {
                    auto source         = std::get<0>(key);
                    auto dest           = std::get<1>(key);
                    adouble newDistance = distances[source] - std::log(value);
                    if (newDistance < distances[dest]) {
                        distances[dest]     = newDistance;
                        predecessors[dest]  = source;
                        lastUpdatedCurrency = dest;
                    }
                }
            }

            // Check for negative cycles
            for (const auto& [key, value] : exchangeRateMap_) {
                auto source         = std::get<0>(key);
                auto dest           = std::get<1>(key);
                adouble newDistance = distances[source] - std::log(value);
                if (newDistance < distances[dest]) {
                    // Reconstruct the cycle
                    std::vector<size_t> cycle;
                    for (size_t i = 0; i < numCurrencies; ++i) { lastUpdatedCurrency = predecessors[lastUpdatedCurrency]; }

                    size_t currentCurrency = lastUpdatedCurrency;
                    do {
                        cycle.push_back(currentCurrency);
                        currentCurrency = predecessors[currentCurrency];
                    } while (currentCurrency != lastUpdatedCurrency);

                    return cycle;
                }
            }

            return {};  // No arbitrage found
        }

        /**
         * @brief returns a clone of the ExchangeRateManager.
         *
         * @return std::unique_ptr<ExchangeRateManager>
         */
        std::unique_ptr<ExchangeRateManager> clone() const {
            auto clonedManager                = std::make_unique<ExchangeRateManager>(refDate_);
            clonedManager->exchangeRateMap_   = exchangeRateMap_;
            clonedManager->exchangeRateCache_ = exchangeRateCache_;
            return clonedManager;
        }

        /**
         * @brief returns true if the exchange rate between the two currencies is known.
         *
         * @param weak
         * @param strong
         * @return true
         * @return false
         */
        bool hasExchangeRate(const Currency& weak, const Currency& strong) const {
            // check if the currency is in the map
            bool hasCcy = exchangeRateMap_.find({weak.numericCode(), strong.numericCode()}) != exchangeRateMap_.end();
            if (!hasCcy) {
                try {
                    exchange(weak, strong);
                    return true;
                } catch (std::runtime_error& e) { return false; }
                return false;
            }
        }

        /**
         * @brief Set the risk free curve for a given currency.
         *
         * @param ccy
         * @param curve
         */
        void riskFreeCurveIdx(const Currency& ccy, const Context<YieldTermStructure<adouble>>& curve) {
            riskFreeCurveMap_[ccy.numericCode()] = curve.idx();
        }

        /**
         * @brief Get the risk free curve for a given currency.
         *
         * @param ccy
         * @return size_t
         */
        size_t riskFreeCurveIdx(const Currency& ccy) const { return riskFreeCurveIdx(ccy.numericCode()); }

        size_t riskFreeCurveIdx(size_t ccy) const {
            auto it = riskFreeCurveMap_.find(ccy);
            if (it == riskFreeCurveMap_.end()) { throw std::runtime_error("Risk-free curve not found for the given currency"); }
            return it->second;
        }

       private:
        Date refDate_;
        mutable std::unordered_map<std::tuple<size_t, size_t>, adouble> exchangeRateCache_;
        std::unordered_map<std::tuple<size_t, size_t>, adouble> exchangeRateMap_;
        std::unordered_map<size_t, size_t> riskFreeCurveMap_;
    };

}  // namespace Atlas

namespace std {

    /**
     * @brief hash specialization for std::tuple<size_t, size_t>
     *
     * @tparam
     */
    template <>
    struct hash<std::tuple<size_t, size_t>> {
        size_t operator()(const std::tuple<size_t, size_t>& tuple) const {
            auto [first, second] = tuple;
            return std::hash<size_t>()(first) ^ std::hash<size_t>()(second);
        }
    };
}  // namespace std

#endif /* DE58DF6A_C264_4FEA_ADB5_CAD0E751958B */
