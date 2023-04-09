#ifndef FB3CE86C_B207_47DE_B110_DA337769FAF4
#define FB3CE86C_B207_47DE_B110_DA337769FAF4

#include <atlas/atlasconfig.hpp>
#include <atlas/fundation/indexable.hpp>
#include <atlas/rates/curvecontext.hpp>

namespace Atlas {

    template <typename adouble>
    class Instrument;
    /**
     * @brief A class representing a cashflow
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
        Cashflow(const CurveContext<adouble>& discountCurveContext) : discountContextIdx_(discountCurveContext.idx()), hasDiscountContext_(true){};

        /**
         * Constructor
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow
         */
        Cashflow(const Date& paymentDate, adouble amount, size_t ccyCode = 0, bool applyCcy = false)
        : amount_(amount), paymentDate_(paymentDate), ccyCode_(ccyCode), applyCcy_(applyCcy){};

        /**
         * Constructor
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow
         */
        Cashflow(const Date& paymentDate, adouble amount, const CurveContext<adouble>& discountCurveContext, size_t ccyCode = 0, bool applyCcy = false)
        : amount_(amount),
          paymentDate_(paymentDate),
          hasDiscountContext_(true),
          ccyCode_(ccyCode),
          applyCcy_(applyCcy),
          discountContextIdx_(discountCurveContext.idx()){};

        virtual ~Cashflow(){};

        /**
         * Gets the amount of the cashflow
         * @return The amount (total payment) of the cashflow
         */
        inline virtual adouble amount() const { return amount_; }

        /**
         * Gets the payment date of the cashflow
         * @return The payment date of the cashflow
         */
        inline virtual Date paymentDate() const { return paymentDate_; }

        /**
         * Checks if the cashflow has occurred (is alive)
         * @param date The date of evaluation
         * @return True if the cashflow has occurred, false otherwise
         */
        inline virtual bool hasOcurred(const Date& date) const {
            if (paymentDate() > date) return false;
            return true;
        }

        /**
         * Sets the discount curve context
         * @param context The discount curve context
         */
        inline void discountCurveContext(const CurveContext<adouble>& context) {
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
         * @return The discount curve context
         */
        inline size_t discountContextIdx() const { return discountContextIdx_; }

        /**
         * @brief Gets the currency code
         * @return The currency code
         */
        inline size_t ccyCode() const { return ccyCode_; }

        /**
         * @brief Sets the currency code
         * @param ccyCode The currency code
         */
        inline void ccyCode(size_t ccyCode) { ccyCode_ = ccyCode; }

        /**
         * @brief Checks if the cashflow applies the currency
         * @return True if the cashflow applies the currency, false otherwise
         */
        inline bool applyCcy() const { return applyCcy_; }

        /**
         * @brief Checks if the cashflow applies the currency
         * @return True if the cashflow applies the currency, false otherwise
         */
        inline void applyCcy(bool applyCcy) { applyCcy_ = applyCcy; }

       protected:
        adouble amount_          = 0;  // amount means static (non-risk-sensitive cashflows)
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
