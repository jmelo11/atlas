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
        Cashflow(std::shared_ptr<CurveContext> discountCurveContext = nullptr) : discountCurveContext_(discountCurveContext){};
        /***
         * Constructor
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow
         */
        Cashflow(const Date& paymentDate, double amount, std::shared_ptr<CurveContext> discountCurveContext = nullptr)
        : amount_(amount), paymentDate_(paymentDate), discountCurveContext_(discountCurveContext){};

        virtual ~Cashflow(){};

        /***
         * Gets the amount of the cashflow
         * @return The amount (total payment) of the cashflow
         */
        virtual double amount() const { return amount_; }

        /***
         * Gets the payment date of the cashflow
         * @return The payment date of the cashflow
         */
        virtual Date paymentDate() const { return paymentDate_; }

        /***
         * Checks if the cashflow has occurred (is alive)
         * @param date The date of evaluation
         * @return True if the cashflow has occurred, false otherwise
         */
        virtual bool hasOcurred(const Date& date) const {
            if (paymentDate() > date) return false;
            return true;
        }

        /***
         * Gets the discount curve index
         * @return The discount curve index
         */
        std::shared_ptr<CurveContext> discountCurveContext() const { return discountCurveContext_; }

        /***
         * Sets the discount curve context
         * @param context The discount curve context
         */
        void discountCurveContext(std::shared_ptr<CurveContext> context) { discountCurveContext_ = context; }

       protected:
        double amount_    = 0;
        Date paymentDate_ = Date();
        std::shared_ptr<CurveContext> discountCurveContext_;
    };

}  // namespace Atlas

#endif /* FB3CE86C_B207_47DE_B110_DA337769FAF4 */
