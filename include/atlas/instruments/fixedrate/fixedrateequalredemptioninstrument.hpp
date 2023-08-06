#ifndef A4801BB0_4300_4C89_9784_55E269E00A56
#define A4801BB0_4300_4C89_9784_55E269E00A56

#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>

namespace Atlas {
    /**
     * @class FixedRateEqualRedemptionInstrument
     * @brief A class for fixed, single-legged, equal redemption instruments.
     * @ingroup FixedRateInstruments
     */
    template <typename adouble = double>
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
                                           const InterestRate<adouble>& rate, Side side = Side::Recieve)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional) {
            Schedule schedule        = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
            std::vector<Date> dates  = schedule.dates();
            adouble redemptionAmount = notional / (dates.size() - 1);
            std::vector<adouble> redemptions(dates.size() - 1, redemptionAmount);

            auto invSide = side == Side::Recieve ? Side::Pay : Side::Recieve;
            this->cashflows_.addDisbursement(Cashflow<adouble>(startDate, notional, invSide));
            double tmpNotional = notional;
            for (size_t i = 1; i < dates.size(); i++) {
                this->cashflows_.addRedemption(Cashflow<adouble>(dates.at(i), redemptions.at(i - 1), side));
                this->cashflows_.addFixedRateCoupon(FixedRateCoupon<adouble>(dates.at(i - 1), dates.at(i), tmpNotional, rate, side));
                tmpNotional -= redemptions.at(i - 1);
            }
        };

        /**
         * @brief Construct a new Fixed Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountContextIdx discount curve context of the instrument
         */
        FixedRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional,
                                           const InterestRate<adouble>& rate, size_t discountContextIdx, Side side = Side::Recieve)
        : FixedRateEqualRedemptionInstrument(startDate, endDate, freq, notional, rate, side) {
            this->cashflows_.discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* A4801BB0_4300_4C89_9784_55E269E00A56 */
