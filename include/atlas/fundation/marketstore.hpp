#ifndef B0F28AFF_3A60_4857_90F2_152C813BE8CC
#define B0F28AFF_3A60_4857_90F2_152C813BE8CC

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/exchangeratemanager.hpp>
#include <atlas/fundation/yieldtermstructuremanager.hpp>

namespace Atlas {

    /**
     * @class MarketStore
     * @brief This class is responsable for storing all market related data, so processes relying on it can access it easely.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class MarketStore {
       public:
        /**
         * @brief Construct a new Market Store object
         *
         * @param refDate reference date of the data inside
         * @param localCurrency local currency of the store
         */
        MarketStore(const Date& refDate, const Currency& localCurrency = USD()) : refDate_(refDate), localCurrency_(localCurrency) {
            curveManager_ = YieldTermStructureManager<adouble>(refDate, localCurrency);
            fxManager_    = ExchangeRateManager<adouble>(refDate, localCurrency);
        };

        /**
         * @brief Returns an reference to the curve manager inside the store.
         *
         * @return YieldTermStructureManager<adouble>&
         */
        inline YieldTermStructureManager<adouble>& curveManager() { return curveManager_; };

        /**
         * @brief Return a const reference to the curve manager inside the store
         *
         * @return const YieldTermStructureManager<adouble>&
         */
        inline const YieldTermStructureManager<adouble>& curveManager() const { return curveManager_; };

        /**
         * @brief Returns a reference to the exchange rate manager inside the store.
         *
         * @return ExchangeRateManager<adouble>&
         */
        inline ExchangeRateManager<adouble>& fxManager() { return fxManager_; };

        /**
         * @brief Returns a const reference to the exchange rate manager inside the store.
         *
         * @return const ExchangeRateManager<adouble>&
         */
        inline const ExchangeRateManager<adouble>& fxManager() const { return fxManager_; };

        /**
         * @brief Returns the reference date of the store
         *
         * @return const Date&
         */
        inline const Date& refDate() const { return refDate_; };

        /**
         * @brief Returns the local currency of the store
         *
         * @return const Currency&
         */
        inline const Currency& localCurrency() const { return localCurrency_; };

       private:
        YieldTermStructureManager<adouble> curveManager_;
        ExchangeRateManager<adouble> fxManager_;
        Date refDate_           = Date();
        Currency localCurrency_ = Currency();
    };

}  // namespace Atlas

#endif /* B0F28AFF_3A60_4857_90F2_152C813BE8CC */
