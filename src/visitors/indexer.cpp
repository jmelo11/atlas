#include <atlas/visitors/indexer.hpp>

namespace Atlas {

    void Indexer::visit(FloatingRateInstrument& inst) {
        auto& leg         = inst.leg();
        auto& coupons     = leg.coupons();
        auto& redemptions = leg.redemptions();

        for (auto& coupon : coupons) {
            indexCashflow(coupon);
            indexFloatingCoupon(coupon);
        }

        for (auto& redemption : redemptions) { indexCashflow(redemption); }
    }

    void Indexer::visit(FixedRateInstrument& inst) {
        auto& leg         = inst.leg();
        auto& coupons     = leg.coupons();
        auto& redemptions = leg.redemptions();

        for (auto& coupon : coupons) { indexCashflow(coupon); }
        for (auto& redemption : redemptions) { indexCashflow(redemption); }
    }

    void Indexer::setRequest(MarketRequest& request) {
        auto& dfs  = request.dfs;
        auto& fwds = request.fwds;
        dfs.insert(dfs.end(), dfs_.begin(), dfs_.end());
        fwds.insert(fwds.end(), fwds_.begin(), fwds_.end());
    }

    void Indexer::indexCashflow(Cashflow& cashflow) {
        if (!cashflow.hasDiscountContext()) { throw std::runtime_error("Cashflow does not have a discount curve context."); }
        size_t curveIdx         = cashflow.discountContextIdx();
        const auto& paymentDate = cashflow.paymentDate();
        dfs_.push_back({curveIdx, paymentDate});
        cashflow.dfIdx(dfs_.size() - 1);
    }

    void Indexer::indexFloatingCoupon(FloatingRateCoupon& coupon) {
        if (!coupon.hasForecastContext()) { throw std::runtime_error("Floating rate coupon does not have a forecast curve context."); }
        size_t curveIdx = coupon.forecastContextIdx();
        fwds_.push_back({curveIdx, coupon.startDate(), coupon.endDate()});
        coupon.fwdIdx(fwds_.size() - 1);
    }

    void Indexer::clear() {
        dfs_.clear();
        fwds_.clear();
    }
}  // namespace Atlas