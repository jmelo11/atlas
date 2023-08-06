#ifndef A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D
#define A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D

#include <atlas/cashflows/cashflowstreammixins.hpp>
#include <atlas/instruments/instrument.hpp>

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
    class FixedRateInstrument : public Instrument<adouble> {
       public:
        using Cashflows = CashflowStream<adouble, FixedRateCouponStreamMixin, RedemptionStreamMixin, DisbursementStreamMixin>;
        /**
         * @brief Construct a new Fixed Rate Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param rate rate of the instrument
         * @param notional notional of the instrument
         * @param leg leg of the instrument
         */
        FixedRateInstrument(const Date& startDate, const Date& endDate, const InterestRate<adouble>& rate, Side side = Side::Recieve,
                            double notional = 0.0, const Cashflows& cashflows = Cashflows())
        : Instrument<adouble>(startDate, endDate, notional, side), rate_(rate), cashflows_(cashflows){};

        /**
         * @brief Destroy the Fixed Rate Instrument object
         *
         */

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        void rate(const InterestRate<adouble>& r) {
            rate_ = r;
            cashflows_.rate(r);
        };

        /**
         * @brief Sets the rate of the instrument.
         *
         * @param rate
         */
        void rate(adouble r) {
            InterestRate<adouble> tmpR(r, rate_.dayCounter(), rate_.compounding(), rate_.frequency());
            rate(tmpR);
        };

        /**
         * @brief Returns the rate of the instrument.
         *
         * @return InterestRate
         */
        const InterestRate<adouble>& rate() const { return rate_; };

        Cashflows& cashflows() { return cashflows_; };

        const Cashflows& cashflows() const { return cashflows_; };

       protected:
        InterestRate<adouble> rate_;
        Cashflows cashflows_;
    };
}  // namespace Atlas

#endif /* A8BC3BBB_99D6_41C4_9EEB_436877D8DA6D */
