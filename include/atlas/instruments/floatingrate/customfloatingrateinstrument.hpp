#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/rates/index/interestrateindex.hpp>

namespace Atlas {
    /**
     * @class CustomFloatingRateInstrument
     * @brief A class for custom floating rate instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble = double>
    class CustomFloatingRateInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Custom Floating Rate Instrument object
         *
         * @param dates  dates of payment for the instrument, starting with the start date of the first coupon
         * @param redemptions redemption amounts for the instrument
         * @param spread spread of the instrument
         * @param index interest rate index of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const InterestRateIndex<adouble>& index)
        : FloatingRateInstrument<adouble>(dates.front(), dates.back(), Side::Long, 0, spread) {
            this->leg_ = MakeLeg<FloatingRateLeg, adouble>().dates(dates).redemptions(redemptions).spread(spread).interestRateIndex(index).build();

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
         * @param index interest rate index of the instrument
         * @param discountContextIdx index of the discount curve context of the instrument
         * @param indexContextIdx index of the interest rate index context of the instrument
         */
        CustomFloatingRateInstrument(const std::vector<Date>& dates, const std::vector<double>& redemptions, adouble spread,
                                     const InterestRateIndex<adouble>& index, size_t discountContextIdx, size_t indexContextIdx)
        : CustomFloatingRateInstrument(dates, redemptions, spread, index) {
            this->leg().indexContextIdx(indexContextIdx);
            this->leg().discountContextIdx(discountContextIdx);
            this->disbursement_.discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
