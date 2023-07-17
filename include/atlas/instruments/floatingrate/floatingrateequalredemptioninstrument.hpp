#ifndef F4126FFB_083C_4F17_8D47_AFE69F5C4B5A
#define F4126FFB_083C_4F17_8D47_AFE69F5C4B5A

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>

namespace Atlas {
    /**
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
         * @param rateIndexContext forecast curve context of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const Context<RateIndex<adouble>>& rateIndexContext, Side side = Side::Long)
        : FloatingRateInstrument<adouble>(startDate, endDate, side, notional, spread) {
            const auto& index = rateIndexContext.object();
            Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
            const auto& dates = schedule.dates();
            std::vector<double> redemptions(schedule.size() - 1, notional / (schedule.size() - 1));

            this->leg_ = MakeLeg<FloatingRateLeg, adouble>()
                             .dates(dates)
                             .redemptions(redemptions)
                             .spread(this->spread_)
                             .side(this->side_)
                             .rateIndexContext(&rateIndexContext)
                             .build();

            adouble disbursement = -this->notional_;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
        };
        /**
         * @brief Construct a new Floating Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param rateIndexContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const Context<RateIndex<adouble>>& rateIndexContext,
                                              const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : FloatingRateEqualRedemptionInstrument(startDate, endDate, notional, spread, rateIndexContext) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* F4126FFB_083C_4F17_8D47_AFE69F5C4B5A */
