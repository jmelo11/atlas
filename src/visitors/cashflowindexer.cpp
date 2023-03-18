#include <atlas/visitors/cashflowindexer.hpp>

namespace Atlas {

    void CashflowIndexer::visit(FloatingRateInstrument& inst) {
        auto& leg         = inst.leg();
        auto& coupons     = leg.coupons();
        auto& redemptions = leg.redemptions();

        for (auto& coupon : coupons) {
            indexCashflow(coupon);
            indexFloatingCoupon(coupon);
        }

        for (auto& redemption : redemptions) { indexCashflow(redemption); }
    }

    void CashflowIndexer::visit(FixedRateInstrument& inst) {
        auto& leg         = inst.leg();
        auto& coupons     = leg.coupons();
        auto& redemptions = leg.redemptions();

        for (auto& coupon : coupons) { indexCashflow(coupon); }
        for (auto& redemption : redemptions) { indexCashflow(redemption); }
    }

    void CashflowIndexer::setRequest(MarketRequest& request) {
        auto& dfs  = request.dfs;
        auto& fwds = request.fwds;
        dfs.insert(dfs.end(), dfs_.begin(), dfs_.end());
        fwds.insert(fwds.end(), fwds_.begin(), fwds_.end());
    }

    void CashflowIndexer::indexCashflow(Cashflow& cashflow) {
        size_t idx              = cashflow.dfIdx();
        const auto& paymentDate = cashflow.paymentDate();
        dfs_.push_back({idx, paymentDate});
        cashflow.dfIdx(dfs_.size() - 1);
    }

    void CashflowIndexer::indexFloatingCoupon(FloatingRateCoupon& coupon) {
        size_t fwdIdx = coupon.fwdIdx();
        fwds_.push_back({fwdIdx, coupon.startDate(), coupon.endDate()});
        coupon.fwdIdx(fwds_.size() - 1);
    }

    void CashflowIndexer::clear() {
        dfs_.clear();
        fwds_.clear();
    }
}  // namespace Atlas