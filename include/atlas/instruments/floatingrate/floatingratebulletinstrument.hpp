#ifndef BF2714F9_8C95_4D44_B693_CA617F9C3A64
#define BF2714F9_8C95_4D44_B693_CA617F9C3A64

#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/rates/index/interestrateindex.hpp>

namespace Atlas {

    /**
     * @class FloatingRateBulletInstrument
     * @brief A class for floating rate bullet instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble = double>
    class FloatingRateBulletInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate date of the first coupon
         * @param endDate  date of the last coupon
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index forecast curve context of the instrument
         * @param side side of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                     const InterestRateIndex<adouble>& index, Side side = Side::Long)
        : FloatingRateInstrument<adouble>(startDate, endDate, side, notional, spread) {
            this->leg_ = MakeLeg<FloatingRateLeg, adouble>()
                             .startDate(this->startDate_)
                             .endDate(this->endDate_)
                             .notional(this->notional_)
                             .spread(this->spread_)
                             .side(this->side_)
                             .interestRateIndex(index)
                             .build();

            int flag             = (this->side_ == Side::Long) ? 1 : -1;
            adouble disbursement = -this->notional_ * flag;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
        };

        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate date of the first coupon
         * @param endDate  date of the last coupon
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index interest rate index of the instrument
         * @param discountContextIdx index of the discount curve context of the instrument
         * @param indexContextIdx index of the interest rate index context of the instrument
         * @param side side of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                     const InterestRateIndex<adouble>& index, size_t discountContextIdx, size_t indexContextIdx,
                                     Side side = Side::Long)
        : FloatingRateBulletInstrument(startDate, endDate, notional, spread, index, side) {
            this->leg().indexContextIdx(indexContextIdx);
            this->leg().discountContextIdx(discountContextIdx);
            this->disbursement().discountContextIdx(discountContextIdx);
        };
    };
}  // namespace Atlas

#endif /* BF2714F9_8C95_4D44_B693_CA617F9C3A64 */
