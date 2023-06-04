#ifndef C2B2159C_6F91_45A1_BF6E_0727152BBB0E
#define C2B2159C_6F91_45A1_BF6E_0727152BBB0E

#include <atlas/cashflows/legs/makeleg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/instruments/mixins/twolegmixin.hpp>

namespace Atlas {
    /**
     * @defgroup Derivatives Derivatives
     * @ingroup Instruments
     */

    /**
     * @brief A class for fixed-float swap instruments.     
     * @ingroup Derivatives
     */
    template <typename adouble>
    class FixFloatSwap : public Instrument<adouble>, public TwoLegMixin<FixedRateLeg<adouble>, FloatingRateLeg<adouble>> {
       public:
        enum Side { PAY = -1, RECIEVE = 1 };

        FixFloatSwap(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble> rate, adouble spread, Frequency fixFreq,
                     const Context<RateIndex<adouble>>& floatIndex, Side side)
        : TwoLegMixin<FixedRateLeg<adouble>, FloatingRateLeg<adouble>>(), side_(side) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
            side_            = side;

            this->firstLeg_ = MakeLeg<adouble, FixedRateLeg<adouble>>()
                                  .startDate(startDate)
                                  .endDate(endDate)
                                  .notional(notional)
                                  .rate(rate)
                                  .paymentFrequency(fixFreq)
                                  .createRedemptions(false)
                                  .build();

            this->secondLeg_ = MakeLeg<adouble, FloatingRateLeg<adouble>>()
                                   .startDate(startDate)
                                   .endDate(endDate)
                                   .notional(notional)
                                   .rateIndexContext(&floatIndex)
                                   .spread(spread)
                                   .createRedemptions(false)
                                   .build();

            fixFreq_   = fixFreq;
            floatFreq_ = floatIndex.object().fixingFrequency();
            rate_      = rate;
            spread_    = spread;
        };

        FixFloatSwap(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble> rate, adouble spread, Frequency fixFreq,
                     const Context<RateIndex<adouble>>& floatIndex, Side side, const Context<YieldTermStructure<adouble>>& discountCurve)
        : FixFloatSwap(startDate, endDate, notional, rate, spread, fixFreq, floatIndex, side) {
            this->firstLeg().discountCurveContext(discountCurve);
            this->secondLeg().discountCurveContext(discountCurve);
        }

        void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); }

        void accept(ConstVisitor<adouble>& visitor) const override { visitor.visit(*this); }

        inline Side side() const { return side_; }

        inline Frequency fixPaymentFrequency() const { return fixFreq_; }

        inline Frequency floatPaymentFrequency() const { return floatFreq_; }

        inline InterestRate<adouble> rate() const { return rate_; }

        inline adouble spread() const { return spread_; }

       private:
        Side side_;
        Frequency fixFreq_;
        Frequency floatFreq_;
        InterestRate<adouble> rate_;
        adouble spread_;
    };

}  // namespace Atlas

#endif /* C2B2159C_6F91_45A1_BF6E_0727152BBB0E */
