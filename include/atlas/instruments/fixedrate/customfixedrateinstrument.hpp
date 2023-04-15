#ifndef CA42F9F0_E48D_4774_8366_AE0752280DAC
#define CA42F9F0_E48D_4774_8366_AE0752280DAC

#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for custom fixed rate instruments.
     *
     */
    template <typename adouble>
    class CustomFixedRateInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         *
         * @param dates dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param rate rate of the instrument
         */
        CustomFixedRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, const InterestRate<adouble>& rate)
        : FixedRateInstrument<adouble>(dates.front(), dates.back(), rate) {
            for (size_t i = 0; i < redemptions.size(); i++) {
                Redemption<adouble> redemption(dates.at(i + 1), redemptions.at(i));
                this->leg().addRedemption(redemption);
            }
            this->notional_ = std::reduce(redemptions.begin(), redemptions.end());
            this->calculateNotionals(dates, rate);
            this->disbursement_ = Cashflow<adouble>(dates.front(), -this->notional_);
        };

        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         *
         * @param dates dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        CustomFixedRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, const InterestRate<adouble>& rate,
                                  const CurveContext<adouble>& discountCurveContext)
        : CustomFixedRateInstrument(dates, redemptions, rate) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement_.discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* CA42F9F0_E48D_4774_8366_AE0752280DAC */
