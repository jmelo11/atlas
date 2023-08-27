#ifndef E102C28E_32DD_41F0_8B8D_E2A53129216D
#define E102C28E_32DD_41F0_8B8D_E2A53129216D

#include <atlas/atlasconfig.hpp>
#include <atlas/others/auxfuncs.hpp>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace Atlas {

    /**
     * @class ExchangeRateManager
     * @brief This class is reponsible for managing fx parities.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class ExchangeRateManager {
       public:
        /**
         * @brief Construct a new Exchange Rate Manager object
         *
         */
        ExchangeRateManager() = default;

        /**
         * @brief Construct a new Exchange Rate Manager object
         *
         * @param refDate reference date of the manager
         * @param localCurrency local currency of the manager
         */
        ExchangeRateManager(const Date& refDate, const Currency& localCurrency) : refDate_(refDate), localCurrency_(localCurrency) {}

        /**
         * @brief Return the local currency of the manager
         *
         * @return const Currency& local currency
         */
        inline const Currency& localCurrency() const { return localCurrency_; }

        /**
         * @brief Returns the reference date of the manager
         *
         * @return const Date& reference date
         */
        inline const Date& refDate() const { return refDate_; }

        /**
         * @brief Adds an fx parity to the manager
         *
         * @param weak weak currency
         * @param strong strong currency
         * @param spotRate conversion value
         */
        void addExchangeRate(const Currency& weak, const Currency& strong, adouble spotRate) {
            exchangeRateMap_[{weak.numericCode(), strong.numericCode()}] = spotRate;

            ccyNames_[weak.numericCode()]   = weak.code();
            ccyNames_[strong.numericCode()] = strong.code();
        }

        /**
         * @brief calculates the exchange rate between two currencies.
         * @details if a pair is not available directly, it will triagulate the price using other parities.
         *
         * @param firstCcy weak currency
         * @param secondCcy strong currency
         * @return adouble conversion value
         */
        adouble exchangeRate(int firstCcy, int secondCcy) const {
            firstCcy  = codeMapping(firstCcy);
            secondCcy = codeMapping(secondCcy);

            if (firstCcy == secondCcy) { return 1.0; }

            auto cacheKey   = std::make_tuple(firstCcy, secondCcy);
            auto cachedRate = exchangeRateCache_.find(cacheKey);
            if (cachedRate != exchangeRateCache_.end()) { return cachedRate->second; }

            std::queue<std::pair<int, adouble>> q;
            std::unordered_set<int> visited;
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
         * @brief calculates the exchange rate between two currencies.
         * @details if a pair is not available directly, it will triagulate the price using other parities.
         *
         * @param firstCcy weak currency
         * @param secondCcy strong currency
         * @return adouble conversion value
         */
        adouble exchangeRate(const Currency& firstCcy, const Currency& secondCcy) const {
            return exchangeRate(firstCcy.numericCode(), secondCcy.numericCode());
        }

        /**
         * @brief Checks if there are arbitrages between the parities given.
         *
         * @return std::vector<int> parities with arbitrages
         */
        std::vector<int> detectArbitrage() const {
            // Initialize distances and predecessors
            std::unordered_map<int, adouble> distances;
            std::unordered_map<int, int> predecessors;
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
                    std::vector<int> cycle;
                    for (size_t i = 0; i < numCurrencies; ++i) { lastUpdatedCurrency = predecessors[lastUpdatedCurrency]; }

                    int currentCurrency = lastUpdatedCurrency;
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
         * @brief Checks if an fx parity is available between two currencies
         * @details if a parity is not available directly, it calculates it.
         *
         * @param weak
         * @param strong
         * @return true
         * @return false
         */
        bool hasExchangeRate(const Currency& weak, const Currency& strong) const {
            // check if the currency is in the map
            bool hasCcy = exchangeRateMap_.find({weak, strong}) != exchangeRateMap_.end();
            if (!hasCcy) {
                try {
                    exchangeRate(weak, strong);
                    return true;
                } catch (std::runtime_error&) { return false; }
                return false;
            }
            return true;
        }

        /**
         * @brief Prints a summary of the values inside the manager.
         *
         */
        void summary() const {
            std::vector<std::vector<std::string>> tableData;
            tableData.push_back({"weak", "strong", "value", "isImplied"});
            for (const auto& [pair, value] : exchangeRateMap_) {
                std::string weakName   = ccyNames_.at(std::get<0>(pair));
                std::string strongName = ccyNames_.at(std::get<1>(pair));
                tableData.push_back({weakName, strongName, std::to_string(value), "false"});
            }
            for (const auto& [pair, value] : exchangeRateCache_) {
                std::string weakName   = ccyNames_.at(std::get<0>(pair));
                std::string strongName = ccyNames_.at(std::get<1>(pair));
                tableData.push_back({weakName, strongName, std::to_string(value), "true"});
            }
            printTable(tableData);
        }

       private:
        /**
         * @brief Helper method to transform 0 (representation of the local currency when no info is available)
         * to the local currency of the manager.
         *
         * @param code
         * @return int
         */
        int codeMapping(int code) const { return code == 0 ? localCurrency_.numericCode() : code; }

        Date refDate_           = Date();
        Currency localCurrency_ = Currency();
        std::unordered_map<std::tuple<int, int>, adouble> exchangeRateMap_;
        mutable std::unordered_map<std::tuple<int, int>, adouble> exchangeRateCache_;
        std::unordered_map<int, std::string> ccyNames_;
    };

}  // namespace Atlas

namespace std {

    /**
     * @brief hash specialization for std::tuple<size_t, size_t>
     *
     * @tparam
     */
    template <>
    struct hash<std::tuple<int, int>> {
        size_t operator()(const std::tuple<int, int>& tuple) const {
            auto [first, second] = tuple;
            return std::hash<int>()(first) ^ std::hash<int>()(second);
        }
    };
}  // namespace std

#endif /* E102C28E_32DD_41F0_8B8D_E2A53129216D */
