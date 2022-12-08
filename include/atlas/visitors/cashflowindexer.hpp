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
        void visit(FloatingRateBulletProduct& inst) override;
        void visit(CustomFixedRateProduct& inst) override;
        void visit(CustomFloatingRateProduct& inst) override;

        void setRequest(MarketRequest& request);

        void clear();

       private:
        /*
         * agrega, adem�s del df de cada cupon,  el primer factor de descuento
         */
        template <typename T>
        void indexStartDf(T& inst) {
            const std::string& discountCurve = inst.constLeg().discountCurve();
            dfs_.push_back(MarketRequest::DiscountFactor(discountCurve, inst.startDate()));  // if QuantLib::Date()?
            inst.dfIdx(dfs_.size() - 1);
        }

        void useFixedLeg(FixedRateLeg& leg);

        void useFloatingLeg(FloatingRateLeg& leg);

        std::vector<MarketRequest::Rate> fwds_;
        std::vector<MarketRequest::DiscountFactor> dfs_;
    };
}  // namespace Atlas

#endif /* EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406 */
