#include <atlas/instruments/fixedrate/customfixedrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>
#include <numeric>

namespace Atlas {
    CustomFixedRateProduct::CustomFixedRateProduct(std::vector<QuantLib::Date> dates, std::vector<double> redemptions,
                                                   const QuantLib::InterestRate& rate)
    : FixedRateProduct(dates.front(), dates.back(), rate) {
        for (size_t i = 0; i < redemptions.size(); i++) {
            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);
        }
        notional_ = std::reduce(redemptions.begin(), redemptions.end());
        calculateNotionals(dates, rate);
    };

    void CustomFixedRateProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void CustomFixedRateProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

}  // namespace Atlas