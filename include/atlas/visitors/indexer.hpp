#ifndef EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406
#define EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>
#include <set>
#include <unordered_map>

namespace Atlas {

    template <typename adouble>
    class Indexer : public Visitor<adouble> {
       public:
        Indexer(){};

        void visit(FixedRateInstrument<adouble>& inst) override {
            auto& leg         = inst.leg();
            auto& coupons     = leg.coupons();
            auto& redemptions = leg.redemptions();

            for (auto& coupon : coupons) { indexCashflow(coupon); }
            for (auto& redemption : redemptions) { indexCashflow(redemption); }

            indexCashflow(inst.disbursement());
        };

        void visit(FloatingRateInstrument<adouble>& inst) override {
            auto& leg         = inst.leg();
            auto& coupons     = leg.coupons();
            auto& redemptions = leg.redemptions();

            for (auto& coupon : coupons) { indexCashflow(coupon); }
            for (auto& redemption : redemptions) { indexCashflow(redemption); }
            indexCashflow(inst.disbursement());
        };

        void visit(FxForward<adouble>& inst) override {
            indexCashflow(inst.firstLeg().redemptions()[0]);
            indexCashflow(inst.secondLeg().redemptions()[0]);
        };

        void setRequest(MarketRequest& request) {
            request.dfs   = dfsVector_;
            request.fwds  = fwdsVector_;
            request.spots = spotsVector_;
        };

        MarketRequest request() {
            MarketRequest request;
            setRequest(request);
            return request;
        };

        void clear() {
            dfsMap_.clear();
            dfsVector_.clear();
            fwdsMap_.clear();
            fwdsVector_.clear();
            spotsMap_.clear();
            spotsVector_.clear();
        };

       private:
        /**
         * @brief Indexes a cashflow
         *
         * @tparam Flow
         * @param cashflow
         */
        template <typename Flow>
        void indexCashflow(Flow& cashflow) {
            if (!cashflow.hasDiscountContext()) { throw std::runtime_error("Cashflow does not have a discount curve context."); }
            size_t curveIdx         = cashflow.discountContextIdx();
            const Date& paymentDate = cashflow.paymentDate();
            evaluationDates_.insert(paymentDate);

            MarketRequest::DiscountFactor df(curveIdx, paymentDate);

            // If the discount factor has not been indexed, add it to the vector
            if (dfsMap_.find(df) == dfsMap_.end()) {
                dfsVector_.push_back(df);
                dfsMap_[df] = dfsVector_.size() - 1;
            }
            cashflow.dfIdx(dfsMap_[df]);

            size_t ccyIdx = cashflow.currencyContextIdx();
            MarketRequest::Spot spot(ccyIdx, paymentDate);
            if (spotsMap_.find(spot) == spotsMap_.end()) {
                spotsVector_.push_back(spot);
                spotsMap_[spot] = spotsVector_.size() - 1;
            }
            cashflow.spotIdx(spotsMap_[spot]);

            if constexpr (std::is_same_v<Flow, FloatingRateCoupon<adouble>>) {
                if (!cashflow.hasForecastContext()) { throw std::runtime_error("Cashflow does not have a forecast curve context."); }
                size_t curveIdx = cashflow.forecastContextIdx();
                MarketRequest::ForwardRate fwd(cashflow.startDate(), cashflow.endDate(), curveIdx);
                if (fwdsMap_.find(fwd) == fwdsMap_.end()) {
                    fwdsVector_.push_back(fwd);
                    fwdsMap_[fwd] = fwdsVector_.size() - 1;
                }
                cashflow.fwdIdx(fwdsMap_[fwd]);
            }
        };

        std::unordered_map<MarketRequest::ForwardRate, size_t> fwdsMap_;
        std::vector<MarketRequest::ForwardRate> fwdsVector_;

        std::unordered_map<MarketRequest::DiscountFactor, size_t> dfsMap_;
        std::vector<MarketRequest::DiscountFactor> dfsVector_;

        std::vector<MarketRequest::Spot> spotsVector_;
        std::unordered_map<MarketRequest::Spot, size_t> spotsMap_;

        std::set<Date> evaluationDates_;
    };
}  // namespace Atlas

#endif /* EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406 */
