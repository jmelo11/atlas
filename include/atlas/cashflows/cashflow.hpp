#ifndef FB3CE86C_B207_47DE_B110_DA337769FAF4
#define FB3CE86C_B207_47DE_B110_DA337769FAF4

#include <ql/time/date.hpp>

namespace Atlas {

    class Cashflow {
       public:
        Cashflow(){};
        Cashflow(const QuantLib::Date& paymentDate, double amount)
        : paymentDate_(paymentDate), amount_(amount){};

        virtual double amount() const { return amount_; }

        virtual QuantLib::Date paymentDate() const { return paymentDate_; }

        virtual bool hasOcurred(const QuantLib::Date& date) const {
            if (paymentDate() > date) return false;
            return true;
        }

        size_t dfIdx() const { return dfIdx_; }

        void dfIdx(size_t idx) { dfIdx_ = idx; }

       protected:
        double amount_              = 0;
        QuantLib::Date paymentDate_ = QuantLib::Date();

       private:
        size_t dfIdx_ = 0;  // 0 is reserved (0% DF)
    };
}  // namespace Atlas

#endif /* FB3CE86C_B207_47DE_B110_DA337769FAF4 */
