#ifndef FB3CE86C_B207_47DE_B110_DA337769FAF4
#define FB3CE86C_B207_47DE_B110_DA337769FAF4

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/context.hpp>
#include <atlas/fundation/indexable.hpp>

namespace Atlas {
    template <typename adouble>
    class YieldTermStructure;

    template <typename adouble>
    using CurveContext = Context<YieldTermStructure<adouble>>;

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
         * @param discountCurveContext The discount curve context
         */
        Cashflow(const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : hasDiscountContext_(true), discountContextIdx_(discountCurveContext.idx()){};

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
         * @param discountCurveContext The discount curve context
         */
        Cashflow(const Date& paymentDate, adouble amount, const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : amount_(amount), paymentDate_(paymentDate), hasDiscountContext_(true), discountContextIdx_(discountCurveContext.idx()){};

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
        inline virtual Date paymentDate() const { return paymentDate_; }

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
        inline void discountCurveContext(const Context<YieldTermStructure<adouble>>& context) {
            discountContextIdx_ = context.idx();
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
        inline void currency(const Currency& ccy) { ccyCode_ = ccy.numericCode(); }

        /**
         * @brief Gets the currency code
         *
         * @return size_t
         */
        inline size_t currencyCode() const { return ccyCode_; }

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
        adouble amount_          = 0;
        Date paymentDate_        = Date();
        bool hasDiscountContext_ = false;
        size_t ccyCode_          = 0;
        bool applyCcy_           = false;
        size_t discountContextIdx_;

       private:
        void amount(adouble amount) { amount_ = amount; }

        friend class Instrument<adouble>;
    };

    template <typename adouble>
    using Redemption = Cashflow<adouble>;
}  // namespace Atlas

#endif /* FB3CE86C_B207_47DE_B110_DA337769FAF4 */
