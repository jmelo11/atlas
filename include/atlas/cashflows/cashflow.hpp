#ifndef FB3CE86C_B207_47DE_B110_DA337769FAF4
#define FB3CE86C_B207_47DE_B110_DA337769FAF4

#include <ql/time/date.hpp>

namespace Atlas {

    class Cashflow {
       public:
        Cashflow(){};
        virtual double amount() const { return 0; }
        virtual QuantLib::Date date() const { return QuantLib::Date(); }
        virtual bool hasOcurred(const QuantLib::Date& date) const { return false; }
    };
}  // namespace Atlas

#endif /* FB3CE86C_B207_47DE_B110_DA337769FAF4 */
