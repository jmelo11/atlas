#ifndef B50701D3_345F_46BA_A774_CC39E13E6595
#define B50701D3_345F_46BA_A774_CC39E13E6595

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {

    /**
     * @brief A class for zero coupon instruments.
     *
     */
    class ZeroCouponInstrument : public FixedRateInstrument {
       public:
        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         */
        ZeroCouponInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate);

        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        ZeroCouponInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate& rate,
                             const CurveContext& discountCurveContext);
    };
}  // namespace Atlas

#endif /* B50701D3_345F_46BA_A774_CC39E13E6595 */
