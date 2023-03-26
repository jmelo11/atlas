#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for custom floating rate instruments.
     *
     */
    class CustomFloatingRateInstrument : public FloatingRateInstrument {
       public:
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         */
        CustomFloatingRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, double spread,
                                     const CurveContext& forecastCurveContext);
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        CustomFloatingRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, double spread,
                                     const CurveContext& forecastCurveContext, const CurveContext& discountCurveContext);
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
