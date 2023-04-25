#ifndef C2B2159C_6F91_45A1_BF6E_0727152BBB0E
#define C2B2159C_6F91_45A1_BF6E_0727152BBB0E

#include <atlas/instruments/instrument.hpp>
#include <atlas/instruments/mixins/twolegmixin.hpp>

namespace Atlas {

    template <typename adouble>
    class VanillaSwap : public Instrument<adouble>, TwoLegMixin<FixedRateLeg<adouble>, FloatingRateLeg<adouble>> {
       public:
        VanillaSwap(const Date& startDate, const Date& endDate, double notional, const FixedRateLeg<adouble>& fixLeg,
                    const FixedRateLeg<adouble>& floatingLeg, Side side)
        : TwoLegMixin(fixLeg, floatingLeg) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notional;
        }

        void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); }

        void accept(ConstVisitor<adouble>& visitor) const override { visitor.visit(*this); }
    };

}  // namespace Atlas

#endif /* C2B2159C_6F91_45A1_BF6E_0727152BBB0E */
