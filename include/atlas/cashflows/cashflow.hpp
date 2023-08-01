#ifndef FB3CE86C_B207_47DE_B110_DA337769FAF4
#define FB3CE86C_B207_47DE_B110_DA337769FAF4

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/indexable.hpp>

namespace Atlas {

    template <typename adouble>
    class Instrument;

    /**
     * @class Cashflow
     * @brief Base class representing a simple cashflow.
     * @defgroup Cashflows Cashflows
     *
     * @tparam adouble The type of the cashflow amount
     */
    template <typename adouble>
    class Cashflow : public Indexable {
       public:
        Cashflow() = default;

        /**
         * @brief Construct a new Cashflow object
         *
         * @param discountContextIdx The discount curve context
         */
        Cashflow(size_t discountContextIdx) : hasDiscountContext_(true), discountContextIdx_(discountContextIdx){};

        /**
         * @brief Construct a new Cashflow object
         *
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow
         */
        Cashflow(const Date& paymentDate, adouble amount) : amount_(amount), paymentDate_(paymentDate){};

        /**
         * @brief Construct a new Cashflow object
         *
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow
         * @param discountContextIdx The discount curve context
         */
        Cashflow(const Date& paymentDate, adouble amount, size_t discountContextIdx)
        : amount_(amount), paymentDate_(paymentDate), hasDiscountContext_(true), discountContextIdx_(discountContextIdx){};

        virtual ~Cashflow(){};

        /**
         * @brief Gets the amount (total payment) of the cashflow
         *
         * @return The amount (total payment) of the cashflow
         */
        inline virtual adouble amount() const { return amount_; }

        /**
         * @brief Gets the payment date of the cashflow
         *
         * @return The payment date of the cashflow
         */
        inline virtual const Date& paymentDate() const { return paymentDate_; }

        /**
         * @brief Checks if the cashflow has occurred at a given a past date
         *
         * @param date The date of evaluation
         * @return True if the cashflow has occurred, false otherwise
         */
        inline virtual bool hasOcurred(const Date& date) const {
            if (paymentDate() > date) return false;
            return true;
        }

        /**
         * @brief Sets the discount curve context
         *
         * @param context The discount curve context
         */
        inline void discountContextIdx(size_t idx) {
            if (idx == SIZE_MAX) throw std::runtime_error("Invalid discount curve context index");
            discountContextIdx_ = idx;
            hasDiscountContext_ = true;
        }

        /**
         * @brief Checks if the cashflow has a discount curve context
         * @return True if the cashflow has a discount curve context, false otherwise
         */
        inline bool hasDiscountContext() const { return hasDiscountContext_; }

        /**
         * @brief Gets the discount curve context
         *
         * @return The discount curve context
         */
        inline size_t discountContextIdx() const { return discountContextIdx_; }

        /**
         * @brief Sets the currency of the cashflow.
         * @details The store value is the currency code.
         *
         * @param ccy The currency
         */
        inline void currency(const Currency& ccy) { ccy_ = ccy; }

        /**
         * @brief Gets the currency of the cashflow.
         * @details The store value is the currency code.
         *
         * @return The currency
         */
        inline Currency currency() const { return ccy_; }

        /**
         * @brief Gets the currency code
         *
         * @return size_t
         */
        inline size_t currencyCode() const {
            if (ccy_ != Currency()) return ccy_.numericCode();
            return 0;
        }

        /**
         * @brief Returns applyCcy_ flag. This flag is used in pricing, to transform the cashflow amount to the store base currency.
         *
         * @return true
         * @return false
         */
        inline bool applyCcy() const { return applyCcy_; }

        /**
         * @brief Sets the applyCcy_ flag. This flag is used in pricing, to transform the cashflow amount to the store base currency.
         *
         * @param applyCcy
         */
        inline void applyCcy(bool applyCcy) { applyCcy_ = applyCcy; }

       protected:
        adouble amount_            = 0;
        Date paymentDate_          = Date();
        bool hasDiscountContext_   = false;
        Currency ccy_              = Currency();
        bool applyCcy_             = false;
        size_t discountContextIdx_ = SIZE_MAX;

       private:
        inline void amount(adouble amount) { amount_ = amount; }

        friend class Instrument<adouble>;
    };

    template <typename adouble>
    using Redemption = Cashflow<adouble>;
}  // namespace Atlas

#endif /* FB3CE86C_B207_47DE_B110_DA337769FAF4 */
