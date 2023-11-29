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
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param tenor tenor of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index forecast curve context of the instrument
         * @param side side of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, const Period& tenor, double notional, adouble spread,
                                     const InterestRateIndex<adouble>& index, Side side = Side::Receive)
        : FloatingRateInstrument<adouble>(startDate, endDate, notional, spread, side) {
            Schedule schedule       = MakeSchedule().from(startDate).to(endDate).withTenor(tenor);
            std::vector<Date> dates = schedule.dates();

            auto invSide = side == Side::Receive ? Side::Pay : Side::Receive;
            this->cashflows_.addDisbursement(Cashflow<adouble>(startDate, notional, invSide));
            this->cashflows_.addRedemption(Cashflow<adouble>(endDate, notional, side));
            for (size_t i = 1; i < dates.size(); i++) {
                this->cashflows_.addFloatingRateCoupon(
                    FloatingRateCoupon<adouble>(dates.at(i - 1), dates.at(i), notional, spread, index.rateDefinition(), side));
            }
        };

        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param tenor tenor of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index forecast curve context of the instrument
         * @param discountContextIdx index of the discount curve context of the instrument
         * @param indexContextIdx index of the interest rate index context of the instrument
         * @param side side of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, const Period& tenor, double notional, adouble spread,
                                     const InterestRateIndex<adouble>& index, size_t discountContextIdx, size_t indexContextIdx,
                                     Side side = Side::Receive)
        : FloatingRateBulletInstrument(startDate, endDate, tenor, notional, spread, index, side) {
            this->cashflows_.indexContextIdx(indexContextIdx);
            this->cashflows_.discountContextIdx(discountContextIdx);
        };

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
                                     const InterestRateIndex<adouble>& index, Side side = Side::Receive)
        : FloatingRateBulletInstrument(startDate, endDate, index.tenor(), notional, spread, index, side){};

        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate date of the first coupon
         * @param endDate  date of the last coupon
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param index forecast curve context of the instrument
         * @param discountContextIdx index of the discount curve context of the instrument
         * @param indexContextIdx index of the interest rate index context of the instrument
         * @param side side of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                     const InterestRateIndex<adouble>& index, size_t discountContextIdx, size_t indexContextIdx,
                                     Side side = Side::Receive)
        : FloatingRateBulletInstrument(startDate, endDate, index.tenor(), notional, spread, index, discountContextIdx, indexContextIdx, side){};
    };
}  // namespace Atlas

#endif /* BF2714F9_8C95_4D44_B693_CA617F9C3A64 */
