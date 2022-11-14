
#include <atlas/cashflows/redemption.hpp>

namespace Atlas {

    Redemption::Redemption(const QuantLib::Date& paymentDate, double amount)
    : Cashflow(paymentDate, amount){};

}  // namespace Atlas