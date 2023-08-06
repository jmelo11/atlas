#ifndef F4126FFB_083C_4F17_8D47_AFE69F5C4B5A
#define F4126FFB_083C_4F17_8D47_AFE69F5C4B5A

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/rates/index/interestrateindex.hpp>

namespace Atlas {
    /**
     * @class FloatingRateEqualRedemptionInstrument
     * @brief A class for floating rate equal redemption instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble = double>
    class FloatingRateEqualRedemptionInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Floating Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index forecast curve context of the instrument
         * @param side side of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const InterestRateIndex<adouble>& index, Side side = Side::Recieve)
        : FloatingRateInstrument<adouble>(startDate, endDate, notional, spread, side) {
            Schedule schedule        = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
            std::vector<Date> dates  = schedule.dates();
            adouble redemptionAmount = notional / (dates.size() - 1);
            std::vector<adouble> redemptions(dates.size() - 1, redemptionAmount);

            auto invSide = side == Side::Recieve ? Side::Pay : Side::Recieve;
            this->cashflows_.addDisbursement(Cashflow<adouble>(startDate, notional, invSide));
            double tmpNotional = notional;
            for (size_t i = 1; i < dates.size(); i++) {
                this->cashflows_.addRedemption(Cashflow<adouble>(dates.at(i), redemptions.at(i - 1), side));
                this->cashflows_.addFloatingRateCoupon(
                    FloatingRateCoupon<adouble>(dates.at(i - 1), dates.at(i), tmpNotional, spread, index.rateDefinition(), side));
                tmpNotional -= redemptions.at(i - 1);
            }
        };
        /**
         * @brief Construct a new Floating Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index interest rate index of the instrument
         * @param discountContextIdx index of the discount curve context of the instrument
         * @param indexContextIdx index of the interest rate index context of the instrument
         * @param side side of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const InterestRateIndex<adouble>& index, size_t discountContextIdx, size_t indexContextIdx,
                                              Side side = Side::Recieve)
        : FloatingRateEqualRedemptionInstrument(startDate, endDate, notional, spread, index, side) {
            this->cashflows_.indexContextIdx(indexContextIdx);
            this->cashflows_.discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* F4126FFB_083C_4F17_8D47_AFE69F5C4B5A */
