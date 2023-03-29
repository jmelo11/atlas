#ifndef FB3CE86C_B207_47DE_B110_DA337769FAF4
#define FB3CE86C_B207_47DE_B110_DA337769FAF4

#include <ql/time/date.hpp>
#include <atlas/atlasconfig.hpp>
#include <atlas/basictypes/indexable.hpp>
#include <atlas/rates/curvecontext.hpp>

namespace Atlas {
    class Cashflow;
    typedef Cashflow Redemption;

    class Cashflow : public Indexable {
       public:
        Cashflow() = default;

        Cashflow(const CurveContext& discountCurveContext) : discountContextIdx_(discountCurveContext.idx()), hasDiscountContext_(true){};

        /**
         * Constructor
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow
         */
        Cashflow(const Date& paymentDate, double amount) : amount_(amount), paymentDate_(paymentDate){};

        /**
         * Constructor
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow
         */
        Cashflow(const Date& paymentDate, double amount, const CurveContext& discountCurveContext)
        : amount_(amount), paymentDate_(paymentDate), discountContextIdx_(discountCurveContext.idx()), hasDiscountContext_(true){};

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
        inline void discountCurveContext(const CurveContext& context) {
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

       protected:
        adouble amount_   = 0;  // amount means static (non-risk-sensitive cashflows)
        Date paymentDate_ = Date();
        size_t discountContextIdx_;
        bool hasDiscountContext_ = false;
    };

}  // namespace Atlas

#endif /* FB3CE86C_B207_47DE_B110_DA337769FAF4 */
