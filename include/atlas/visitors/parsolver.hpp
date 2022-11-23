#ifndef D897AD55_2529_41AF_98E6_ECC9CA3FF5CB
#define D897AD55_2529_41AF_98E6_ECC9CA3FF5CB

#include <ql/math/solvers1d/brent.hpp>
#include <atlas/visitors/npvcalculator.hpp>

namespace Atlas {


    class ParSolver : public ConstVisitor {
       public:
        ParSolver(const MarketData& marketData) : marketData_(marketData){};

        void clear() { value_ = 0.0; };

        double results() const { return value_; };

        void visit(const Deposit& inst) const override;
        void visit(const FixedBulletProduct& inst) const override;
        void visit(const EqualPaymentProduct& inst) const override;
        void visit(const FloatingRateBulletProduct& inst) const override;

       private:
        template <typename T>
        void evalFixedRateProd(const T& inst) const {
            T evalInst(inst);
            NPVCalculator calc(marketData_);
            double startDf = marketData_.dfs.at(inst.dfIdx());

            auto f = [&](double r) {
                evalInst.rate(r);
                calc.visit(evalInst);
                double npv      = calc.results();
                double notional = inst.notional();
                calc.clear();
                return npv - notional * startDf;
            };
            value_ = solver_.solve(f, accuracy_, guess_, step_);
        };

        template <typename T>
        void evalFloatingRateProd(const T& inst) const {
            T evalInst(inst);
            NPVCalculator calc(marketData_);
            double startDf = marketData_.dfs.at(inst.dfIdx());

            auto f = [&](double s) {
                evalInst.spread(s);
                calc.visit(evalInst);
                double npv      = calc.results();
                double notional = inst.notional();
                calc.clear();
                return npv - notional * startDf;
            };
            value_ = solver_.solve(f, accuracy_, guess_, step_);
        };

        const MarketData& marketData_;
        QuantLib::Brent solver_;
        mutable double value_ = 0.0;
        double guess_         = 0.0;
        double accuracy_      = 0.0001;
        double step_          = 0.0001;
    };
}  // namespace Atlas

#endif /* D897AD55_2529_41AF_98E6_ECC9CA3FF5CB */
