#ifndef A4801BB0_4300_4C89_9784_55E269E00A56
#define A4801BB0_4300_4C89_9784_55E269E00A56

#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for fixed, single-legged, equal redemption instruments.
     *
     */
    template <typename adouble>
    class FixedRateEqualRedemptionInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Fixed Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         */
        FixedRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional,
                                           const InterestRate<adouble>& rate)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, notional) {
            Schedule schedule        = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
            std::vector<Date> dates  = schedule.dates();
            adouble redemptionAmount = notional / (dates.size() - 1);
            std::vector<adouble> redemptions(dates.size() - 1, redemptionAmount);

            // double outstanding = notional;
            // for (size_t i = 0; i < redemptions.size(); ++i) {
            //     FixedRateCoupon<adouble> coupon(dates.at(i), dates.at(i + 1), outstanding, rate);
            //     this->leg().addCoupon(coupon);

            //     Redemption<adouble> redemption(dates.at(i + 1), redemptions.at(i));
            //     this->leg().addRedemption(redemption);
            //     outstanding -= redemptions.at(i);
            // }

            this->leg_ = MakeLeg<adouble, FixedRateLeg<adouble>>()
                             .startDate(startDate)
                             .endDate(endDate)
                             .paymentFrequency(freq)
                             .notional(notional)
                             .rate(rate)
                             .redemptions(redemptions)
                             .build();
                             
            this->disbursement(Cashflow<adouble>(startDate, -notional));
        };

        /**
         * @brief Construct a new Fixed Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        FixedRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional,
                                           const InterestRate<adouble>& rate, const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : FixedRateEqualRedemptionInstrument(startDate, endDate, freq, notional, rate) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* A4801BB0_4300_4C89_9784_55E269E00A56 */
