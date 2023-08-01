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
                                              const InterestRateIndex<adouble>& index, Side side = Side::Long)
        : FloatingRateInstrument<adouble>(startDate, endDate, side, notional, spread) {
            Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
            const auto& dates = schedule.dates();
            std::vector<double> redemptions(schedule.size() - 1, notional / (schedule.size() - 1));

            this->leg_ = MakeLeg<FloatingRateLeg, adouble>()
                             .dates(dates)
                             .redemptions(redemptions)
                             .spread(this->spread_)
                             .side(this->side_)
                             .interestRateIndex(index)
                             .build();

            int flag             = (this->side_ == Side::Long) ? 1 : -1;
            adouble disbursement = -this->notional_ * flag;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
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
                                              const InterestRateIndex<adouble>& index, size_t indexContextIdx, size_t discountContextIdx,
                                              Side side = Side::Long)
        : FloatingRateEqualRedemptionInstrument(startDate, endDate, notional, spread, index, side) {
            this->leg().indexContextIdx(discountContextIdx);
            this->leg().discountContextIdx(discountContextIdx);
            this->disbursement().discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* F4126FFB_083C_4F17_8D47_AFE69F5C4B5A */
