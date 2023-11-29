#ifndef BC63B04C_97BE_4479_8930_4D9D1682AE61
#define BC63B04C_97BE_4479_8930_4D9D1682AE61

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/rates/index/interestrateindex.hpp>

namespace Atlas {
    /**
     * @class ZeroCouponInstrument
     * @brief A class for zero coupon instruments.
     *
     */
    template <typename adouble = double>
    class ZeroCouponFloatingRateInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index index of the instrument
         * @param side side of the instrument
         */
        ZeroCouponFloatingRateInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                         const InterestRateIndex<adouble>& index, Side side = Side::Receive)
        : FloatingRateInstrument<adouble>(startDate, endDate, notional, spread, side) {
            auto invSide = side == Side::Receive ? Side::Pay : Side::Receive;
            this->cashflows_.addDisbursement(Cashflow<adouble>(startDate, notional, invSide));
            this->cashflows_.addRedemption(Cashflow<adouble>(endDate, notional, side));
            this->cashflows_.addFloatingRateCoupon(FloatingRateCoupon<adouble>(startDate, endDate, notional, spread, index.rateDefinition(), side));
        };

        /**
         * @brief Construct a new Zero Coupon Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index index of the instrument
         * @param discountContextIdx discount curve context of the instrument
         * @param indexContextIdx index curve context of the instrument
         * @param side side of the instrument
         */
        ZeroCouponFloatingRateInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                         const InterestRateIndex<adouble>& index, size_t discountContextIdx, size_t indexContextIdx,
                                         Side side = Side::Receive)
        : ZeroCouponFloatingRateInstrument(startDate, endDate, notional, spread, index, side) {
            this->cashflows_.indexContextIdx(indexContextIdx);
            this->cashflows_.discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* BC63B04C_97BE_4479_8930_4D9D1682AE61 */
