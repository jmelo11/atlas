#ifndef BF2714F9_8C95_4D44_B693_CA617F9C3A64
#define BF2714F9_8C95_4D44_B693_CA617F9C3A64

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>

namespace Atlas {

    /**
     * @brief A class for floating rate bullet instruments.
     * @ingroup FloatingRateInstruments
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
         * @param rateIndexContext forecast curve context of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                     const Context<RateIndex<adouble>>& rateIndexContext, Side side = Side::Long)
        : FloatingRateInstrument<adouble>(startDate, endDate, side, notional, spread) {
            this->leg_ = MakeLeg<adouble, FloatingRateLeg<adouble>>()
                             .startDate(this->startDate_)
                             .endDate(this->endDate_)
                             .notional(this->notional_)
                             .spread(this->spread_)
                             .side(this->side_)
                             .rateIndexContext(&rateIndexContext)
                             .build();

            adouble disbursement = -this->notional_;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
        };

        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate date of the first coupon
         * @param endDate  date of the last coupon
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param rateIndexContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                     const Context<RateIndex<adouble>>& rateIndexContext,
                                     const Context<YieldTermStructure<adouble>>& discountCurveContext)
        : FloatingRateBulletInstrument(startDate, endDate, notional, spread, rateIndexContext) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* BF2714F9_8C95_4D44_B693_CA617F9C3A64 */
