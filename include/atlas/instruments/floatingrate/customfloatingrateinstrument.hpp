#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for custom floating rate instruments.
     *
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
         * @param forecastCurveContext forecast curve context of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const CurveContext<adouble>& forecastCurveContext)
        : FloatingRateInstrument<adouble>(dates.front(), dates.back(), 0, spread) {
            this->notional_    = std::reduce(redemptions.begin(), redemptions.end());
            double outstanding = this->notional_;
            for (size_t i = 0; i < redemptions.size(); i++) {
                Redemption<adouble> redemption(dates.at(i + 1), redemptions.at(i));
                this->leg_.addRedemption(redemption);

                FloatingRateCoupon<adouble> coupon(dates.at(i), dates.at(i + 1), outstanding, spread, forecastCurveContext);
                this->leg_.addCoupon(coupon);
                outstanding -= redemptions.at(i);
            }

            this->disbursement_ = Cashflow<adouble>(dates.front(), -this->notional_);
        };
        ;
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const CurveContext<adouble>& forecastCurveContext, const CurveContext<adouble>& discountCurveContext)
        : CustomFloatingRateInstrument(dates, redemptions, spread, forecastCurveContext) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement_.discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
