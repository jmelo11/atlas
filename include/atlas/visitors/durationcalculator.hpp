#ifndef FEEC155C_A524_4B89_99CA_672817BB79CD
#define FEEC155C_A524_4B89_99CA_672817BB79CD

#include <ql/cashflows/cashflows.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>
#include <ql/cashflows/simplecashflow.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <type_traits>

namespace Atlas {
    template <typename adouble>
    class DurationCalculator : public ConstVisitor<adouble> {
       public:
        DurationCalculator(const MarketData<adouble>& marketData = MarketData<adouble>(), double delta = 0.0001)
        : marketData_(marketData), delta_(delta){};

        adouble results() const { return results_; }

        void clear() { results_ = 0.0; }

        void visit(const FloatingRateInstrument<adouble>& inst) const override { floatingInstSens(inst); };

        void visit(const FixedRateInstrument<adouble>& inst) const override { fixedInstSens(inst); };

       private:
        template <typename T>
        void fixedInstSens(const T& inst) const {
            T tmpProd = inst;
            NPVCalculator<adouble> npvCacl(marketData_);
            npvCacl.visit(tmpProd);
            adouble npv = npvCacl.results();
            npvCacl.clear();

            auto rate = tmpProd.rate();
            tmpProd.rate(rate.rate() + delta_);
            npvCacl.visit(tmpProd);
            adouble npv_ = npvCacl.results();

            results_ = (npv_ - npv) / npv / delta_;
        };

        template <typename T>
        void floatingInstSens(const T& inst) const {
            T tmpProd = inst;
            NPVCalculator npvCacl(marketData_);
            npvCacl.visit(tmpProd);
            adouble npv = npvCacl.results();
            npvCacl.clear();
            tmpProd.spread(tmpProd.spread() + delta_);
            npvCacl.visit(tmpProd);
            adouble npv_ = npvCacl.results();

            results_ = (npv_ - npv) / npv / delta_;
        };

        double delta_;
        const MarketData<adouble>& marketData_;
        mutable adouble results_ = 0.0;
    };

}  // namespace Atlas

#endif /* FEEC155C_A524_4B89_99CA_672817BB79CD */
