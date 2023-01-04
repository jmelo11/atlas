#ifndef FEEC155C_A524_4B89_99CA_672817BB79CD
#define FEEC155C_A524_4B89_99CA_672817BB79CD

#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <type_traits>

namespace Atlas {
    class YieldSensCalculator : public ConstVisitor {
       public:
        YieldSensCalculator(const QuantLib::InterestRate& yield, const QuantLib::Date& refDate, const MarketData& marketData = MarketData(),
                            QuantLib::Duration::Type durationType = QuantLib::Duration::Modified);

        double duration() const { return duration_; }
        double convexity() const { return convexity_; }
        double yieldNPV() const { return yieldNPV_; }

        double basisPointValue() const {
            double delta = -duration_ * yieldNPV_;
            double gamma = (convexity_ / 100.0) * yieldNPV_;
            double shift = 0.0001;
            delta *= shift;
            gamma *= shift * shift;
            return delta + 0.5 * gamma;
        }

        void clear() {
            duration_ = 0.0;
            convexity_ = 0.0;
            yieldNPV_ = 0.0;
        }

        void yield(const QuantLib::InterestRate& y) { yield_ = y; }

        QuantLib::InterestRate yield() const { return yield_; }

        void visit(const Deposit& inst) const override;
        void visit(const FixedRateBulletProduct& inst) const override;
        void visit(const EqualPaymentProduct& inst) const override;
        void visit(const FloatingRateBulletProduct& inst) const override;
        void visit(const CustomFixedRateProduct& inst) const override;
        void visit(const CustomFloatingRateProduct& inst) const override;

       private:
        template <typename T>
        void addRedemptionsToLeg(const T& atlasLeg, QuantLib::Leg& qlLeg) const {
            for (const auto& redemption : atlasLeg.constRedemptions()) {
                auto qlRedemption = boost::make_shared<QuantLib::Redemption>(redemption.amount(), redemption.paymentDate());
                qlLeg.push_back(qlRedemption);
            }
        }

        QuantLib::Leg translateFixedRateLeg(const FixedRateLeg& leg) const;

        QuantLib::Leg translateFloatingRateLeg(const FloatingRateLeg& leg) const;

        template <typename T>
        void calculateFixedProductSens(const T& inst) const {
            auto leg = translateFixedRateLeg(inst.constLeg());
            yieldNPV_  = QuantLib::CashFlows::npv(leg, yield_, false, refDate_, refDate_);
            duration_  = QuantLib::CashFlows::duration(leg, yield_, QuantLib::Duration::Modified, false, refDate_, refDate_);
            convexity_ = QuantLib::CashFlows::convexity(leg, yield_, false, refDate_, refDate_);
        };

        template <typename T>
        void calculateFloatingProductSens(const T& inst) const {
            auto leg   = translateFloatingRateLeg(inst.constLeg());
            yieldNPV_  = QuantLib::CashFlows::npv(leg, yield_, false, refDate_, refDate_);
            duration_  = QuantLib::CashFlows::duration(leg, yield_, QuantLib::Duration::Modified, false, refDate_, refDate_);
            convexity_ = QuantLib::CashFlows::convexity(leg, yield_, false, refDate_, refDate_);
        };

        QuantLib::InterestRate yield_;
        const QuantLib::Date& refDate_;
        const MarketData& marketData_;
        const QuantLib::Duration::Type durationType_;

        mutable double duration_  = 0.0;
        mutable double convexity_ = 0.0;
        mutable double yieldNPV_  = 0.0;
    };

}  // namespace Atlas

#endif /* FEEC155C_A524_4B89_99CA_672817BB79CD */
