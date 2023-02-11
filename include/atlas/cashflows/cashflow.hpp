#ifndef FB3CE86C_B207_47DE_B110_DA337769FAF4
#define FB3CE86C_B207_47DE_B110_DA337769FAF4

#include <ql/time/date.hpp>
#include <atlas/atlasconfig.hpp>
#include <atlas/basictypes/indexable.hpp>

namespace Atlas {

    class Cashflow : public Indexable {
       public:    

        Cashflow() = default;

        /***
         * Constructor        
         * @param paymentDate The payment date of the cashflow
         * @param amount The amount of the cashflow         
        */
        Cashflow(const Date& paymentDate, double amount) : amount_(amount), paymentDate_(paymentDate){};

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

       protected:
        double amount_    = 0;
        Date paymentDate_ = Date();

    };
}  // namespace Atlas

#endif /* FB3CE86C_B207_47DE_B110_DA337769FAF4 */
