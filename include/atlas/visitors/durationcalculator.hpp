#ifndef FEEC155C_A524_4B89_99CA_672817BB79CD
#define FEEC155C_A524_4B89_99CA_672817BB79CD

#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <atlas/visitors/cashflowindexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <type_traits>

namespace Atlas {
    class DurationCalculator : public ConstVisitor {
       public:
        DurationCalculator(const MarketData& marketData = MarketData(), double delta = 0.0001);

        double results() const { return results_; }

        void clear() { results_ = 0.0; }

        void visit(const Deposit& inst) const override;
        void visit(const FixedRateBulletProduct& inst) const override;
        void visit(const EqualPaymentProduct& inst) const override;
        void visit(const FloatingRateBulletProduct& inst) const override;
        void visit(const CustomFixedRateProduct& inst) const override;
        void visit(const CustomFloatingRateProduct& inst) const override;

       private:
        template <typename T>
        void calculateFixedProductSens(const T& inst) const {
            T tmpProd = inst;
            NPVCalculator npvCacl(marketData_);
            npvCacl.visit(tmpProd);
            double npv = npvCacl.results();
            npvCacl.clear();
            
            auto rate = tmpProd.rate();
            tmpProd.rate(rate.rate() + delta_);
            npvCacl.visit(tmpProd);
            double npv_ = npvCacl.results();

            results_ = (npv_ - npv) / npv / delta_;
        };

        template <typename T>
        void calculateFloatingProductSens(const T& inst) const {
            T tmpProd = inst;
            NPVCalculator npvCacl(marketData_);
            npvCacl.visit(tmpProd);
            double npv = npvCacl.results();
            npvCacl.clear();
            tmpProd.spread(tmpProd.spread() + delta_);
            npvCacl.visit(tmpProd);
            double npv_ = npvCacl.results();

            results_ = (npv_ - npv) / npv / delta_;
        };

        double delta_;
        const MarketData& marketData_;
        mutable double results_ = 0.0;
    };

}  // namespace Atlas

#endif /* FEEC155C_A524_4B89_99CA_672817BB79CD */
