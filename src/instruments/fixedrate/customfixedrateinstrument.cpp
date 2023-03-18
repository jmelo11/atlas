#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <numeric>

namespace Atlas {
    CustomFixedRateInstrument::CustomFixedRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, const InterestRate& rate)
    : FixedRateInstrument(dates.front(), dates.back(), rate) {
        for (size_t i = 0; i < redemptions.size(); i++) {
            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);
        }
        notional_ = std::reduce(redemptions.begin(), redemptions.end());
        calculateNotionals(dates, rate);
    };

}  // namespace Atlas