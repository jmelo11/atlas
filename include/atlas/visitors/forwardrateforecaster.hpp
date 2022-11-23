#ifndef B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31
#define B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31

#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    class ForwardRateForecaster : public Visitor {
       public:
        ForwardRateForecaster(const MarketData& marketData) : marketData_(marketData){};

        void visit(Deposit& inst) override;
        void visit(FixedBulletProduct& inst) override;
        void visit(EqualPaymentProduct& inst) override;
        void visit(FloatingRateBulletProduct& inst) override;

       private:
        void fixFloatingCoupons(FloatingRateLeg& leg);
        const MarketData& marketData_;
    };
}  // namespace Atlas

#endif /* B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31 */
