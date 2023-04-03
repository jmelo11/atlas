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

        adouble results() const { return results_; }

        void clear() { results_ = 0.0; }

        void visit(const FloatingRateInstrument<adouble>& inst) const override { floatingInstSens(inst); };

        void visit(const FixedRateInstrument<adouble>& inst) const override { fixedInstSens(inst); };

       private:
        template <typename T>
        void fixedInstSens(const T& inst) const {
            T tmpProd = inst;
            NPVCalculator<adouble> npvCacl(marketData_);
            auto rate = tmpProd.rate();

            auto f = [&](adouble r) {
                npvCacl.clear();
                InterestRate tmpRate(r, rate.dayCounter(), rate.compounding(), rate.frequency());
                tmpProd.rate();
                npvCacl.visit(tmpProd);
                return npvCacl.results();
            };

            if constexpr (std::is_same_v<adouble, double>) {
                adouble npv  = f(rate.rate());
                adouble npv_ = f(rate.rate() + delta_);
                results_     = (npv_ - npv) / npv / delta_;
            } else {
                results_ = derivative(f, wrt(rate), at(rate)) / 100;
            }
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

            if constexpr (std::is_same_v<adouble, double>) {
                adouble npv  = f(spread);
                adouble npv_ = f(spread + delta_);
                results_     = (npv_ - npv) / npv / delta_;
            } else {
                results_ = derivative(f, wrt(spread), at(spread)) / 100;
            }
        };

        double delta_;
        const MarketData<adouble>& marketData_;
        mutable adouble results_ = 0.0;
    };

}  // namespace Atlas

#endif /* FEEC155C_A524_4B89_99CA_672817BB79CD */
