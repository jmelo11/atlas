#include <atlas/visitors/cashflowindexer.hpp>

namespace Atlas {

    void CashflowIndexer::visit(Deposit& inst) {
        indexStartDf(inst);
        useFixedLeg(inst.leg());
    }

    void CashflowIndexer::visit(FixedBulletProduct& inst) {
        indexStartDf(inst);
        useFixedLeg(inst.leg());
    }

    void CashflowIndexer::visit(EqualPaymentProduct& inst) {
        indexStartDf(inst);
        useFixedLeg(inst.leg());
    }

    void CashflowIndexer::visit(FloatingRateBulletProduct& inst) {
        indexStartDf(inst);
        useFloatingLeg(inst.leg());
    }

    void CashflowIndexer::setRequest(MarketRequest& request) {
        auto& dfs  = request.dfs;
        auto& fwds = request.fwds;
        dfs.insert(dfs.end(), dfs_.begin(), dfs_.end());
        fwds.insert(fwds.end(), fwds_.begin(), fwds_.end());
    }

    void CashflowIndexer::useFixedLeg(FixedRateLeg& leg) {
        auto& coupons     = leg.coupons();
        auto& redemptions = leg.redemptions();

        const std::string& discountCurve = leg.discountCurve();
        for (auto& coupon : coupons) {
            dfs_.push_back(MarketRequest::DiscountFactor(discountCurve, coupon.paymentDate()));
            coupon.dfIdx(dfs_.size() - 1);
        }
        for (auto& redemption : redemptions) {
            dfs_.push_back(MarketRequest::DiscountFactor(discountCurve, redemption.paymentDate()));
            redemption.dfIdx(dfs_.size() - 1);
        }
    }

    void CashflowIndexer::useFloatingLeg(FloatingRateLeg& leg) {
        auto& coupons     = leg.coupons();
        auto& redemptions = leg.redemptions();

        const std::string& discountCurve = leg.discountCurve();
        const std::string& forecastCurve = leg.forecastCurve();

        for (auto& coupon : coupons) {
            dfs_.push_back(MarketRequest::DiscountFactor(discountCurve, coupon.paymentDate()));
            coupon.dfIdx(dfs_.size() - 1);

            const auto& index = coupon.index();
            fwds_.push_back(
                MarketRequest::Rate(forecastCurve, coupon.startDate(), coupon.endDate(), index.dayCounter(), index.rateCompounding(), index.rateFrequency()));
            coupon.fwdIdx(fwds_.size() - 1);
        }

        for (auto& redemption : redemptions) {
            dfs_.push_back(MarketRequest::DiscountFactor(discountCurve, redemption.paymentDate()));
            redemption.dfIdx(dfs_.size() - 1);
        }
    }

    void CashflowIndexer::clear() {
        dfs_.clear();
        fwds_.clear();
    }
}  // namespace Atlas