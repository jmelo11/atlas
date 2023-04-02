#ifndef F4126FFB_083C_4F17_8D47_AFE69F5C4B5A
#define F4126FFB_083C_4F17_8D47_AFE69F5C4B5A

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {
    /**
     * @brief A class for floating rate equal redemption instruments.
     *
     */
    template <typename adouble>
    class FloatingRateEqualRedemptionInstrument : public FloatingRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Floating Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const CurveContext& forecastCurveContext);
        /**
         * @brief Construct a new Floating Rate Equal Redemption Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param forecastCurveContext forecast curve context of the instrument
         * @param discountCurveContext discount curve context of the instrument
         */
        FloatingRateEqualRedemptionInstrument(const Date& startDate, const Date& endDate, double notional, adouble spread,
                                              const CurveContext& forecastCurveContext, const CurveContext& discountCurveContext);
    };
}  // namespace Atlas

#endif /* F4126FFB_083C_4F17_8D47_AFE69F5C4B5A */
