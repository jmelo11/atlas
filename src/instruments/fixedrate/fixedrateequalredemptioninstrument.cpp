#include <ql/time/schedule.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/instruments/fixedrate/fixedrateequalredemptioninstrument.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    FixedRateEqualRedemptionInstrument::FixedRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, Frequency freq,
                                                                           double notional, const InterestRate& rate)
    : FixedRateInstrument(startDate, endDate, rate, notional) {
        Schedule schedule       = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
        std::vector<Date> dates = schedule.dates();
        std::vector<double> redemptions(dates.size() - 1, notional / (dates.size() - 1));

        double outstanding = notional;
        for (size_t i = 0; i < redemptions.size(); ++i) {
            FixedRateCoupon coupon(dates.at(i), dates.at(i + 1), outstanding, rate);
            leg_.addCoupon(coupon);

            Redemption redemption(dates.at(i + 1), redemptions.at(i));
            leg_.addRedemption(redemption);
            outstanding -= redemptions.at(i);
        }

        disbursement_ = Cashflow(startDate, -notional);
    }

    FixedRateEqualRedemptionInstrument::FixedRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, Frequency freq,
                                                                           double notional, const InterestRate& rate,
                                                                           const CurveContext& discountCurveContext)
    : FixedRateEqualRedemptionInstrument(startDate, endDate, freq, notional, rate) {
        leg().discountCurveContext(discountCurveContext);
        disbursement_.discountCurveContext(discountCurveContext);
    }

}  // namespace Atlas
