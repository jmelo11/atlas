#ifndef A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D
#define A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D

#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/instruments/mixins/onelegmixin.hpp>

namespace Atlas {

    /**
     * @defgroup FixedRateInstruments Fixed Rate Instruments
     * @ingroup Instruments
     */

    /**
     * @class FixedRateInstrument
     * @brief An class for fixed-rate-single-legged instruments.
     * @ingroup FixedRateInstruments
     *
     * @tparam adouble The type of number used for calculation. It can be either double or an AAD-enabled type.
     */
    template <typename adouble = double>
    class FixedRateInstrument : public Instrument<adouble>, public OneLegMixin<FixedRateLeg, adouble> {
       public:
        /**
         * @brief Construct a new Fixed Rate Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param rate rate of the instrument
         * @param notional notional of the instrument
         * @param leg leg of the instrument
         */
        FixedRateInstrument(const Date& startDate, const Date& endDate, const InterestRate<adouble>& rate, Side side = Side::Long,
                            double notional = 0.0, const FixedRateLeg<adouble>& leg = FixedRateLeg<adouble>())
        : OneLegMixin<FixedRateLeg, adouble>(leg), rate_(rate) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->side_      = side;
            this->notional_  = notional;
            for (auto& coupon : this->leg().coupons()) { coupon.rate(rate_); }
        };

        /**
         * @brief Destroy the Fixed Rate Instrument object
         *
         */
        virtual ~FixedRateInstrument(){};

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        virtual void rate(const InterestRate<adouble>& r) {
            rate_ = r;
            for (auto& coupon : this->leg().coupons()) { coupon.rate(r); }
        };

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        virtual void rate(adouble r) {
            InterestRate<adouble> tmpR(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
            rate(tmpR);
        };

        /**
         * @brief Returns the rate of the instrument.
         *
         * @return InterestRate
         */
        InterestRate<adouble> rate() const { return rate_; };

       protected:
        InterestRate<adouble> rate_;
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
