#ifndef C97E38A5_ACC9_4464_8B0E_B4A6622F7456
#define C97E38A5_ACC9_4464_8B0E_B4A6622F7456

#include <atlas/atlasconfig.hpp>
#include <functional>

namespace Atlas {

    template <typename adouble>
    class MarketStore;

    template <typename adouble>
    using CcyRecepy = std::function<adouble(const Date&, const Date&, const MarketStore<adouble>&)>;

    /**
     * @brief A class representing a currency context
     * @details A currency context is a container for the currency code and the currency index
     */

    template <typename adouble>
    class CurrencyContext {
       public:
        /**
         * Gets the currency code
         * @return The currency code
         */
        inline Currency ccy() const { return ccy_; }

        /**
         * Gets the currency value
         * @return The currency value
         */
        inline adouble value() const { return value_; }

        /**
         * Gets the currency recepy
         * @return The currency recepy
         */
        inline CcyRecepy<adouble> recepy() const { return recepy_; }

        /**
         * Gets the currency index
         * @return The currency index
         */
        inline size_t idx() const { return idx_; }

       private:
        /**
         * Constructor
         * @param ccyCode The currency code
         * @param ccyIdx The currency index
         */
        CurrencyContext(const Currency& ccy, adouble value, size_t idx, CcyRecepy<adouble> recepy)
        : ccy_(ccy), value_(value), idx_(idx), recepy_(recepy){};

        Currency ccy_;
        adouble value_;
        size_t idx_;
        CcyRecepy<adouble> recepy_;

        friend class MarketStore<adouble>;
    };

}  // namespace Atlas

#endif /* C97E38A5_ACC9_4464_8B0E_B4A6622F7456 */
