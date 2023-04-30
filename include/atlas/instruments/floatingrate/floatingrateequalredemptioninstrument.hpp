#ifndef F4126FFB_083C_4F17_8D47_AFE69F5C4B5A
#define F4126FFB_083C_4F17_8D47_AFE69F5C4B5A

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for floating rate equal redemption instruments.
     *
     */
    template <typename adouble>
    class FloatingRateEqualRedemptionInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Floating Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const Context<RateIndex<adouble>>& forecastCurveContext)
        : FloatingRateInstrument<adouble>(startDate, endDate, notional, spread) {
            const auto& index = forecastCurveContext.object();
            Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
            const auto& dates = schedule.dates();
            std::vector<double> redemptions(schedule.size() - 1, notional / (schedule.size() - 1));

            double outstanding = notional;
            for (size_t i = 0; i < dates.size() - 1; ++i) {
                FloatingRateCoupon<adouble> coupon(dates.at(i), dates.at(i + 1), outstanding, spread, forecastCurveContext);
                this->leg().addCoupon(coupon);
                Redemption<adouble> redemption(dates.at(i + 1), redemptions.at(i));
                this->leg().addRedemption(redemption);
                outstanding -= redemptions.at(i);
            }

            this->disbursement(Cashflow<adouble>(startDate, -notional));
        };
        /**
         * @brief Construct a new Floating Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const Context<RateIndex<adouble>>& forecastCurveContext,
                                              const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : FloatingRateEqualRedemptionInstrument(startDate, endDate, notional, spread, forecastCurveContext) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* F4126FFB_083C_4F17_8D47_AFE69F5C4B5A */
