
#include <atlas/cashflows/cashflows.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <algorithm>
namespace Atlas {

    void Cashflows::sortRedemptions(std::vector<Redemption>& redemptions) {
        auto f = [](const Redemption& lhs, const Redemption& rhs) {
            return lhs.paymentDate() < rhs.paymentDate();
        };
        std::sort(redemptions.begin(), redemptions.end(), f);
    }

}  // namespace Atlas
