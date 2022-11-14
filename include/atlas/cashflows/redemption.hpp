#ifndef A0BBBC2B_D8BB_4FFA_8288_2D14D574A14E
#define A0BBBC2B_D8BB_4FFA_8288_2D14D574A14E

#include <atlas/cashflows/cashflow.hpp>

namespace Atlas {

    class Redemption : public Cashflow {
       public:
        Redemption(const QuantLib::Date& paymentDate, double amount);
    };

}  // namespace Atlas

#endif /* A0BBBC2B_D8BB_4FFA_8288_2D14D574A14E */
