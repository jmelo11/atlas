#ifndef EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406
#define EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406

#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    class CashflowIndexer : public Visitor {
       public:
        CashflowIndexer(){};

        void visit(Deposit& inst) override;
        void visit(FixedRateBulletProduct& inst) override;
        void visit(EqualPaymentProduct& inst) override;
        void visit(FixedRateEqualRedemptionProduct& inst) override;        
        void visit(FloatingRateBulletProduct& inst) override;
        void visit(FloatingRateEqualRedemptionProduct& inst) override;
        void visit(CustomFixedRateProduct& inst) override;
        void visit(CustomFloatingRateProduct& inst) override;


        void setRequest(MarketRequest& request);

        void clear();

       private:
        /*
         * agrega, adem�s del df de cada cupon,  el primer factor de descuento
         */

        void indexStartDf(Leg& leg) {
            const std::string& discountCurve = leg.discountCurve();
            dfs_.push_back(MarketRequest::DiscountFactor(discountCurve, leg.startDate())); 
            leg.dfIdx(dfs_.size() - 1);
        }

        void useFixedLeg(FixedRateLeg& leg);

        void useFloatingLeg(FloatingRateLeg& leg);

        std::vector<MarketRequest::Rate> fwds_;
        std::vector<MarketRequest::DiscountFactor> dfs_;
    };
}  // namespace Atlas

#endif /* EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406 */
