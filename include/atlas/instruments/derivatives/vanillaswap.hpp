#ifndef C2B2159C_6F91_45A1_BF6E_0727152BBB0E
#define C2B2159C_6F91_45A1_BF6E_0727152BBB0E

#include <atlas/cashflows/legs/makeleg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/instruments/mixins/twolegmixin.hpp>

namespace Atlas {

    template <typename adouble>
    class VanillaSwap : public Instrument<adouble>, public TwoLegMixin<FixedRateLeg<adouble>, FloatingRateLeg<adouble>> {
        enum Side { PAY = -1, RECIEVE = 1 };

       public:
        VanillaSwap(const Date& startDate, const Date& endDate, double notional, const FixedRateLeg<adouble>& fixLeg,
                    const FloatingRateLeg<adouble>& floatingLeg, Side side)
        : TwoLegMixin<FixedRateLeg<adouble>, FloatingRateLeg<adouble>>(fixLeg, floatingLeg) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
        }

        VanillaSwap(const Date& startDate, const Date& endDate, double notional, const InterestRate<adouble> rate, adouble spread, Frequency fixFreq,
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
                                  .build();
                                  
            this->secondLeg_ = MakeLeg<adouble, FloatingRateLeg<adouble>>()
                                   .startDate(startDate)
                                   .endDate(endDate)
                                   .notional(notional)
                                   .index(floatIndex)
                                   .spread(spread)
                                   .build();
        };

        void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); }

        void accept(ConstVisitor<adouble>& visitor) const override { visitor.visit(*this); }

       private:
        Side side_;
    };

}  // namespace Atlas

#endif /* C2B2159C_6F91_45A1_BF6E_0727152BBB0E */
