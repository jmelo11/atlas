#ifndef EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406
#define EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406

#include <atlas/data/marketdata.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/visitor.hpp>

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

            for (auto& coupon : coupons) {
                indexCashflow(coupon);
                indexFloatingCoupon(coupon);
            }

            for (auto& redemption : redemptions) { indexCashflow(redemption); }
            indexCashflow(inst.disbursement());
        };

        void setRequest(MarketRequest& request) {
            auto& dfs   = request.dfs;
            auto& fwds  = request.fwds;
            auto& spots = request.spots;

            dfs.insert(dfs.end(), dfs_.begin(), dfs_.end());
            fwds.insert(fwds.end(), fwds_.begin(), fwds_.end());
            spots.insert(spots.end(), spots_.begin(), spots_.end());
        };

        MarketRequest request() {
            MarketRequest request;
            setRequest(request);
            return request;
        };

        void clear() {
            dfs_.clear();
            fwds_.clear();
            spots_.clear();
        };

       private:
        void indexCashflow(Cashflow<adouble>& cashflow) {
            if (!cashflow.hasDiscountContext()) { throw std::runtime_error("Cashflow does not have a discount curve context."); }
            size_t curveIdx         = cashflow.discountContextIdx();
            const auto& paymentDate = cashflow.paymentDate();
            dfs_.push_back({curveIdx, paymentDate});
            cashflow.dfIdx(dfs_.size() - 1);

            size_t ccyCode = cashflow.ccyCode();
            if (cashflow.applyCcy()) {
                spots_.push_back({ccyCode, paymentDate});
            } else {
                spots_.push_back({ccyCode});
            }
            cashflow.spotIdx(spots_.size() - 1);
        };

        void indexFloatingCoupon(FloatingRateCoupon<adouble>& coupon) {
            if (!coupon.hasForecastContext()) { throw std::runtime_error("Floating rate coupon does not have a forecast curve context."); }
            size_t curveIdx = coupon.forecastContextIdx();
            fwds_.push_back({curveIdx, coupon.startDate(), coupon.endDate()});
            coupon.fwdIdx(fwds_.size() - 1);
        };

        std::vector<MarketRequest::Rate> fwds_;
        std::vector<MarketRequest::DiscountFactor> dfs_;
        std::vector<MarketRequest::Spot> spots_;
    };
}  // namespace Atlas

#endif /* EDA3136B_5C3D_4D8A_8F4A_AE6D1B5AC406 */
