#ifndef B50701D3_345F_46BA_A774_CC39E13E6595
#define B50701D3_345F_46BA_A774_CC39E13E6595

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>

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
        ZeroCouponInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble>& rate)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, notional) {
            FixedRateCoupon<adouble> coupon(startDate, endDate, notional, rate);  // interest coupon
            Redemption<adouble> redemption(endDate, notional);                    // notinal payment at the end

            this->leg().addCoupon(coupon);
            this->leg().addRedemption(redemption);
            this->disbursement_ = Cashflow<adouble>(startDate, -notional);
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
                             const CurveContext<adouble>& discountCurveContext)
        : ZeroCouponInstrument(startDate, endDate, notional, rate) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement_.discountCurveContext(discountCurveContext);
        };
    };
}  // namespace Atlas

#endif /* B50701D3_345F_46BA_A774_CC39E13E6595 */
