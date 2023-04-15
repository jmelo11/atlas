#ifndef C2B2159C_6F91_45A1_BF6E_0727152BBB0E
#define C2B2159C_6F91_45A1_BF6E_0727152BBB0E

namespace Atlas{
    
        template <typename adouble>
        class Swap : public DoubleLegInstrument<adouble, FixedRateLeg<adouble>, FixedRateLeg<adouble>> {
        public:
            Swap(const Date& startDate, const Date& endDate, double notional, const FixedRateLeg<adouble>& firstLeg, const FixedRateLeg<adouble>& secondLeg, Side side)
                : DoubleLegInstrument<adouble, FixedRateLeg<adouble>, FixedRateLeg<adouble>>(startDate, endDate, notional, firstLeg, secondLeg, side) {}
    
            void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); }
        };
    
    }  // namespace Atlas
}

#endif /* C2B2159C_6F91_45A1_BF6E_0727152BBB0E */
