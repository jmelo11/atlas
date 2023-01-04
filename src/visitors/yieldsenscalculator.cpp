#include <atlas/models/staticyieldmodel.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>
#include <atlas/visitors/yieldsenscalculator.hpp>

namespace Atlas {

    YieldSensCalculator::YieldSensCalculator(const QuantLib::InterestRate& yield, const QuantLib::Date& refDate, const MarketData& marketData,
                                             QuantLib::Duration::Type durationType)
    : yield_(yield), refDate_(refDate), marketData_(marketData), durationType_(durationType){};

    void YieldSensCalculator::visit(const Deposit& inst) const {
        calculateFixedProductSens(inst);
    };

    void YieldSensCalculator::visit(const FixedRateBulletProduct& inst) const {
        calculateFixedProductSens(inst);
    };

    void YieldSensCalculator::visit(const EqualPaymentProduct& inst) const {
        calculateFixedProductSens(inst);
    };

    void YieldSensCalculator::visit(const FloatingRateBulletProduct& inst) const {
        calculateFloatingProductSens(inst);
    };

    void YieldSensCalculator::visit(const CustomFloatingRateProduct& inst) const {
        calculateFloatingProductSens(inst);
    };

    void YieldSensCalculator::visit(const CustomFixedRateProduct& inst) const {
        calculateFixedProductSens(inst);
    };

    QuantLib::Leg YieldSensCalculator::translateFixedRateLeg(const FixedRateLeg& leg) const {
        QuantLib::Leg qlLeg;
        for (const auto& coupon : leg.constCoupons()) {
            auto qlCoupon = boost::make_shared<QuantLib::FixedRateCoupon>(coupon.paymentDate(), coupon.notional(), coupon.rate(), coupon.startDate(),
                                                                          coupon.endDate());
            qlLeg.push_back(qlCoupon);
        }
        addRedemptionsToLeg(leg, qlLeg);

        std::sort(qlLeg.begin(), qlLeg.end(), [](const boost::shared_ptr<QuantLib::CashFlow>& lhs, const boost::shared_ptr<QuantLib::CashFlow>& rhs) {
            return lhs->date() < rhs->date();
        });
        return qlLeg;
    };

    QuantLib::Leg YieldSensCalculator::translateFloatingRateLeg(const FloatingRateLeg& leg) const {
        QuantLib::Leg qlLeg;
        ForwardRateForecaster forecaster(marketData_);

        for (const auto& coupon : leg.constCoupons()) {
            double fixing     = coupon.fixing();
            double spread     = coupon.spread();
            const auto& index = coupon.index();
            QuantLib::InterestRate rate(fixing + spread, index.dayCounter(), index.rateCompounding(), index.rateFrequency());
            auto qlCoupon =
                boost::make_shared<QuantLib::FixedRateCoupon>(coupon.paymentDate(), coupon.notional(), rate, coupon.startDate(), coupon.endDate());
            qlLeg.push_back(qlCoupon);
        }

        addRedemptionsToLeg(leg, qlLeg);

        std::sort(qlLeg.begin(), qlLeg.end(), [](const boost::shared_ptr<QuantLib::CashFlow>& lhs, const boost::shared_ptr<QuantLib::CashFlow>& rhs) {
            return lhs->date() < rhs->date();
        });
        return qlLeg;
    };

}  // namespace Atlas