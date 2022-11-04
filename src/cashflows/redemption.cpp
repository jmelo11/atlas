
#include <atlas/cashflows/redemption.hpp>

namespace Atlas {

    QuantLib::Date Redemption::date() const {
        return date_;
    }

    double Redemption::amount() const {
        return amount_;
    }

    bool Redemption::hasOcurred(const Date& date) const {
        if (date > date_) return false;
        return true;
    }
}  // namespace Atlas