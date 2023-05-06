#ifndef BF2714F9_8C95_4D44_B693_CA617F9C3A64
#define BF2714F9_8C95_4D44_B693_CA617F9C3A64

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>

namespace Atlas {

    /**
     * @brief A class for floating rate bullet instruments.
     *
     */
    template <typename adouble>
    class FloatingRateBulletInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate date of the first coupon
         * @param endDate  date of the last coupon
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                     const Context<RateIndex<adouble>>& forecastCurveContext,
                                     const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : FloatingRateBulletInstrument(startDate, endDate, notional, spread, forecastCurveContext) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };

        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate date of the first coupon
         * @param endDate  date of the last coupon
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                     const Context<RateIndex<adouble>>& forecastCurveContext)
        : FloatingRateInstrument<adouble>(startDate, endDate, notional, spread) {
            const auto& index = forecastCurveContext.object();
            Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(index.fixingFrequency());
            Date firstDate    = Date();
            for (const auto& endDate : schedule.dates()) {
                if (firstDate != Date()) {
                    FloatingRateCoupon<adouble> coupon(firstDate, endDate, notional, spread, forecastCurveContext);
                    this->leg().addCoupon(coupon);
                }
                firstDate = endDate;
            }

            Redemption<adouble> redemption(schedule.endDate(), notional);
            this->leg().addRedemption(redemption);

            adouble disbursement = -notional;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
        };
    };
}  // namespace Atlas

#endif /* BF2714F9_8C95_4D44_B693_CA617F9C3A64 */
