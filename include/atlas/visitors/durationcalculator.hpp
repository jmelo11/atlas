#ifndef FEEC155C_A524_4B89_99CA_672817BB79CD
#define FEEC155C_A524_4B89_99CA_672817BB79CD

#include <atlas/instruments/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrateinstrument.hpp>
#include <atlas/visitors/indexer.hpp>
#include <atlas/visitors/npvcalculator.hpp>
#include <type_traits>

namespace Atlas {
    template <typename adouble>
    class DurationCalculator : public ConstVisitor<adouble> {
       public:
        DurationCalculator(const MarketData<adouble>& marketData = MarketData<adouble>(), double delta = 0.0001)
        : marketData_(marketData), delta_(delta){};

        double results() const { return results_; }

        void clear() { results_ = 0.0; }

        void visit(const FloatingRateInstrument<adouble>& inst) const override { floatingInstSens(inst); };

        void visit(const FixedRateInstrument<adouble>& inst) const override { fixedInstSens(inst); };

       private:
        template <typename T>
        void fixedInstSens(const T& inst) const {
            T tmpProd = inst;
            NPVCalculator<adouble> npvCacl(marketData_);
            InterestRate<adouble> rate = tmpProd.rate();
            adouble rateValue          = rate.rate();
            auto f = [&](adouble r) {
                npvCacl.clear();
                tmpProd.rate(r);
                npvCacl.visit(tmpProd);
                return npvCacl.results();
            };        
            adouble npv  = f(rate.rate());
            adouble npv_ = f(rate.rate() + delta_);
            results_     = (npv_ - npv) / npv / delta_;          
        };

        template <typename T>
        void floatingInstSens(const T& inst) const {
            T tmpProd      = inst;
            adouble spread = tmpProd.spread();
            NPVCalculator<adouble> npvCacl(marketData_);
            auto f = [&](adouble s) {
                npvCacl.clear();
                tmpProd.spread(s);
                npvCacl.visit(tmpProd);
                return npvCacl.results();
            };           
            adouble npv  = f(spread);
            adouble npv_ = f(spread + delta_);
            results_     = (npv_ - npv) / npv / delta_;          
        };

        const MarketData<adouble>& marketData_;
        double delta_;
        mutable double results_ = 0.0;
    };

}  // namespace Atlas

#endif /* FEEC155C_A524_4B89_99CA_672817BB79CD */
