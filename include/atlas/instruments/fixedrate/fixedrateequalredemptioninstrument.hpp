#ifndef A4801BB0_4300_4C89_9784_55E269E00A56
#define A4801BB0_4300_4C89_9784_55E269E00A56

#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for fixed, single-legged, equal redemption instruments.
     * @ingroup FixedRateInstruments
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
                                           const InterestRate<adouble>& rate, Side side = Side::Long)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional) {
            Schedule schedule        = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
            std::vector<Date> dates  = schedule.dates();
            adouble redemptionAmount = notional / (dates.size() - 1);
            std::vector<adouble> redemptions(dates.size() - 1, redemptionAmount);

            this->leg_ = MakeLeg<adouble, FixedRateLeg<adouble>>()
                             .startDate(startDate)
                             .endDate(endDate)
                             .paymentFrequency(freq)
                             .notional(notional)
                             .rate(this->rate_)
                             .redemptions(redemptions)
                             .build();

            adouble disbursement = -notional;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
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
                                           const InterestRate<adouble>& rate, const Context<YieldTermStructure<adouble>>& discountCurveContext, Side side = Side::Long)
        : FixedRateEqualRedemptionInstrument(startDate, endDate, freq, notional, rate, side) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* A4801BB0_4300_4C89_9784_55E269E00A56 */
