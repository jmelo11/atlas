
#include <atlas/cashflows/redemption.hpp>

namespace Atlas {

    Redemption::Redemption(const Date& paymentDate, double amount)
    : Cashflow(paymentDate, amount){};

}  // namespace Atlas