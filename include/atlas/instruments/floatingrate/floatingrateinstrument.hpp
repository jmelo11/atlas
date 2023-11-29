#ifndef EA6DF8D1_F778_4FBF_9CDE_CD72C677D078
#define EA6DF8D1_F778_4FBF_9CDE_CD72C677D078

#include <atlas/cashflows/cashflowstreammixins.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {
    /**
     * @defgroup FloatingRateInstruments Floating Rate Instruments
     * @ingroup Instruments
     */

    /**
     * @class FloatingRateInstrument
     * @brief An class for floating, single-legged, rate instruments.
     * @ingroup FloatingRateInstruments
     */
    template <typename adouble = double>
    class FloatingRateInstrument : public Instrument<adouble> {
       public:
        using Cashflows = CashflowStream<adouble, FloatingRateCouponStreamMixin, RedemptionStreamMixin, DisbursementStreamMixin>;
        /**
         * @brief Construct a new Floating Rate Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param side side of the instrument
         * @param notional notional of the instrument
         * @param spread spread of the instrument
         * @param leg leg of the instrument
         */
        FloatingRateInstrument(const Date& startDate, const Date& endDate, double notional = 0.0, adouble spread = 0.0, Side side = Side::Receive,
                               const Cashflows& cashflows = Cashflows())
        : Instrument<adouble>(startDate, endDate, notional, side), spread_(spread), cashflows_(cashflows){};

        /**
         * @brief Returns the spread of the instrument.
         *
         * @return double
         */
        inline adouble spread() const { return spread_; };

        /**
         * @brief Sets the spread of the instrument.
         *
         * @param s
         */
        inline void spread(adouble s) {
            spread_ = s;
            cashflows_.spread(s);
        };

        Cashflows& cashflows() { return cashflows_; };

        const Cashflows& cashflows() const { return cashflows_; };

       protected:
        adouble spread_;
        Cashflows cashflows_;
    };
}  // namespace Atlas

#endif /* EA6DF8D1_F778_4FBF_9CDE_CD72C677D078 */
