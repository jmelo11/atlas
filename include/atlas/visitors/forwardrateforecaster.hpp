#ifndef B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31
#define B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31

#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    class ForwardRateForecaster : public Visitor {
       public:
        ForwardRateForecaster(const MarketData& marketData) : marketData_(marketData){};

        void visit(FloatingRateInstrument& inst) override;

       private:
        const MarketData& marketData_;
    };
}  // namespace Atlas

#endif /* B9BF3F35_DABD_4ACD_8301_6BF2FADC4C31 */
