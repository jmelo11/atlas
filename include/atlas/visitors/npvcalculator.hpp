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

        double results() const { return npv_ + nonSensNPV_; };

        double sensNPV() const { return npv_; };

        double nonSensNPV() const { return nonSensNPV_; };

        void visit(Deposit& inst) override;
        void visit(FixedRateBulletProduct& inst) override;
        void visit(EqualPaymentProduct& inst) override;
        void visit(FloatingRateBulletProduct& inst) override;
        void visit(CustomFixedRateProduct& inst) override;
        void visit(CustomFloatingRateProduct& inst) override;

       private:
        void redemptionsNPV(const Leg& leg);
        void fixedLegNPV(const FixedRateLeg& leg);
        void floatingLegNPV(FloatingRateLeg& leg);

        double npv_                   = 0.0;
        double nonSensNPV_            = 0.0;
        const MarketData& marketData_ = MarketData();
    };
}  // namespace Atlas

#endif /* EA82B4A2_2A5C_4B6E_A0A0_0807E93983BE */
