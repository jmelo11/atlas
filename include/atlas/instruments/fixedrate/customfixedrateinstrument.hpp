#ifndef CA42F9F0_E48D_4774_8366_AE0752280DAC
#define CA42F9F0_E48D_4774_8366_AE0752280DAC

#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for custom fixed rate instruments.
     *
     */
    class CustomFixedRateInstrument : public FixedRateInstrument {
       public:
        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         *
         * @param dates dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param rate rate of the instrument
         */
        CustomFixedRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, const InterestRate& rate);
        /**
         * @brief Construct a new Custom Fixed Rate Instrument object
         *
         * @param dates dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        CustomFixedRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, const InterestRate& rate,
                                  const CurveContext& discountCurveContext);
    };
}  // namespace Atlas

#endif /* CA42F9F0_E48D_4774_8366_AE0752280DAC */
