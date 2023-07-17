#ifndef CA42F9F0_E48D_4774_8366_AE0752280DAC
#define CA42F9F0_E48D_4774_8366_AE0752280DAC

#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for custom fixed rate instruments.
     * @ingroup FixedRateInstruments
     */
    template <typename adouble = double>
    class CustomFixedRateInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         * @details Side is determined by the implied notional of the instrument
         * @param dates dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument. Note: casfhlows are flipped if side is short
         * @param rate rate of the instrument
         */
        CustomFixedRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, const InterestRate<adouble>& rate)
        : FixedRateInstrument<adouble>(dates.front(), dates.back(), rate) {
            this->leg_              = MakeLeg<FixedRateLeg, adouble>().dates(dates).redemptions(redemptions).rate(this->rate_).build();
            adouble impliedNotional = std::reduce(redemptions.begin(), redemptions.end());
            this->side_             = impliedNotional > 0 ? Side::Long : Side::Short;
            this->notional_         = abs(impliedNotional);
            this->disbursement(Cashflow<adouble>(dates.front(), impliedNotional));
        };

        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         *
         * @param dates dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument. Note: casfhlows are flipped if side is short
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        CustomFixedRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, const InterestRate<adouble>& rate,
                                  const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : CustomFixedRateInstrument(dates, redemptions, rate) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* CA42F9F0_E48D_4774_8366_AE0752280DAC */
