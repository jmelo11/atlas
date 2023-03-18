#ifndef EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406
#define EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406

#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    class CashflowIndexer : public Visitor {
       public:
        CashflowIndexer(){};

        void visit(FixedRateInstrument& inst) override;
        void visit(FloatingRateInstrument& inst) override;

        void setRequest(MarketRequest& request);

        void clear();

       private:
        void indexCashflow(Cashflow& cashflow);
        void indexFloatingCoupon(FloatingRateCoupon& coupon);

        std::vector<MarketRequest::Rate> fwds_;
        std::vector<MarketRequest::DiscountFactor> dfs_;
    };
}  // namespace Atlas

#endif /* EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406 */
