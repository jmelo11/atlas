#ifndef B50701D3_345F_46BA_A774_CC39E13E6595
#define B50701D3_345F_46BA_A774_CC39E13E6595

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>

namespace Atlas {

    /**
     * @brief A class for zero coupon instruments.
     *
     */
    template <typename adouble>
    class ZeroCouponInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         */
        ZeroCouponInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble>& rate, Side side = Side::Long)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional) {
            this->leg_ = MakeLeg<adouble, FixedRateLeg<adouble>>()
                             .startDate(this->startDate_)
                             .endDate(this->endDate_)
                             .notional(this->notional_)
                             .side(this->side_)
                             .rate(this->rate_)
                             .paymentFrequency(Frequency::Once)
                             .build();
            adouble disbursement = -this->notional_ * this->side_;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
        };

        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        ZeroCouponInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble>& rate,
                             const Context<YieldTermStructure<adouble>>& discountCurveContext, Side side = Side::Long)
        : ZeroCouponInstrument(startDate, endDate, notional, rate, side) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* B50701D3_345F_46BA_A774_CC39E13E6595 */
