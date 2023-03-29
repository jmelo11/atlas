#ifndef EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE
#define EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE

#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>
#include <iostream>

namespace Atlas {

    class NPVCalculator : public Visitor {
       public:
        NPVCalculator(const MarketData& marketData) : marketData_(marketData){};

        void clear() {
            npv_        = 0.0;
            nonSensNPV_ = 0.0;
        };

        inline adouble results() const { return npv_ + nonSensNPV_; };

        inline adouble sensNPV() const { return npv_; };

        inline double nonSensNPV() const { return nonSensNPV_; };

        void visit(FixedRateInstrument& inst) override;

        void visit(FloatingRateInstrument& inst) override;

       private:
        void redemptionsNPV(const Leg& leg);
        void fixedLegNPV(const FixedRateLeg& leg);
        void floatingLegNPV(FloatingRateLeg& leg);

        adouble npv_        = 0.0;
        double nonSensNPV_ = 0.0;
        const MarketData& marketData_;
    };
}  // namespace Atlas

#endif /* EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE */
