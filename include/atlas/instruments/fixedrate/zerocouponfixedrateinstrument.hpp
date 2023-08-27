#ifndef B50701D3_345F_46BA_A774_CC39E13E6595
#define B50701D3_345F_46BA_A774_CC39E13E6595

#include <ql/interestrate.hpp>
#include <atlas/cashflows/cashflows.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
namespace Atlas {

    /**
     * @class ZeroCouponFixedRateInstrument
     * @brief A class for zero coupon instruments.
     *
     */
    template <typename adouble = double>
    class ZeroCouponFixedRateInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         */
        ZeroCouponFixedRateInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble>& rate,
                                      Side side = Side::Recieve)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional) {
            auto invSide = side == Side::Recieve ? Side::Pay : Side::Recieve;

            this->cashflows_.addDisbursement(Cashflow<adouble>(startDate, notional, invSide));
            this->cashflows_.addRedemption(Cashflow<adouble>(endDate, notional, side));
            this->cashflows_.addFixedRateCoupon(FixedRateCoupon<adouble>(startDate, endDate, notional, rate, side));
        };

        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountContextIdx discount curve context of the instrument
         */
        ZeroCouponFixedRateInstrument(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble>& rate,
                                      size_t discountContextIdx, Side side = Side::Recieve)
        : ZeroCouponFixedRateInstrument(startDate, endDate, notional, rate, side) {
            this->cashflows_.discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* B50701D3_345F_46BA_A774_CC39E13E6595 */
