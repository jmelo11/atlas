#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for custom floating rate instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble>
    class CustomFloatingRateInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param rateIndexContext forecast curve context of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const Context<RateIndex<adouble>>& rateIndexContext)
        : FloatingRateInstrument<adouble>(dates.front(), dates.back(), Side::Long, 0, spread) {
            this->leg_ = MakeLeg<adouble, FloatingRateLeg<adouble>>()
                             .dates(dates)
                             .redemptions(redemptions)
                             .spread(spread)
                             .rateIndexContext(&rateIndexContext)
                             .build();

            adouble impliedNotional = std::reduce(redemptions.begin(), redemptions.end());
            this->side_             = impliedNotional > 0 ? Side::Long : Side::Short;
            this->notional_         = abs(impliedNotional);
            this->disbursement(Cashflow<adouble>(dates.front(), impliedNotional));
        };
        ;
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param rateIndexContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const Context<RateIndex<adouble>>& rateIndexContext,
                                     const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : CustomFloatingRateInstrument(dates, redemptions, spread, rateIndexContext) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement_.discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
