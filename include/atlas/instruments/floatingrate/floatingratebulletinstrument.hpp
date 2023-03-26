#ifndef BF2714F9_8C95_4D44_B693_CA617F9C3A64
#define BF2714F9_8C95_4D44_B693_CA617F9C3A64

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {

    /**
     * @brief A class for floating rate bullet instruments.
     *
     */
    class FloatingRateBulletInstrument : public FloatingRateInstrument {
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
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, double spread,
                                     const CurveContext& forecastCurveContext, const CurveContext& discountCurveContext);

        /**
         * @brief Construct a new Floating Rate Bullet Instrument object
         *
         * @param startDate date of the first coupon
         * @param endDate  date of the last coupon
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         */
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate, double notional, double spread,
                                     const CurveContext& forecastCurveContext);
    };
}  // namespace Atlas

#endif /* BF2714F9_8C95_4D44_B693_CA617F9C3A64 */
