#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <numeric>

namespace Atlas {
    CustomFixedRateInstrument::CustomFixedRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, const InterestRate& rate,
                                                         std::shared_ptr<CurveContext> discountCurveContext)
    : FixedRateInstrument(dates.front(), dates.back(), rate) {
        for (size_t i = 0; i < redemptions.size(); i++) {
            Redemption redemption(dates.at(i + 1), redemptions.at(i), discountCurveContext);
            leg_.addRedemption(redemption);
        }
        notional_ = std::reduce(redemptions.begin(), redemptions.end());
        calculateNotionals(dates, rate);

        disbursement_ = Cashflow(dates.front(), -notional_, discountCurveContext);
    };

}  // namespace Atlas