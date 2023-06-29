#ifndef D897AD55_2529_41AF_98E6_ECC9CA3FF5CB
#define D897AD55_2529_41AF_98E6_ECC9CA3FF5CB

#include <ql/math/solvers1d/brent.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/visitors/npvcalculator.hpp>

namespace Atlas {

    template <typename adouble>
    class ParSolver : public ConstVisitor<adouble> {
        using ConstVisitor<adouble>::visit;

       public:
        ParSolver(const MarketData<adouble>& marketData, double guess = 0.0, double accuracy = 0.00001, double maxIter = 100)
        : marketData_(marketData), guess_(guess), accuracy_(accuracy), maxIter_(maxIter){};

        void clear() { value_ = 0.0; };

        adouble results() const { return value_; };

        void visit(const FloatingRateInstrument<adouble>& inst) const override { evalFloatingRateProd(inst); };
        void visit(const FixedRateInstrument<adouble>& inst) const override { evalFixedRateProd(inst); };
        void visit(const EqualPaymentInstrument<adouble>& inst) const {
            const auto& coupons = inst.constLeg().constCoupons();
            adouble startDf     = marketData_.dfs.at(inst.disbursement().dfIdx());
            adouble dfSum       = 0;
            for (const auto& coupon : coupons) dfSum += marketData_.dfs.at(coupon.dfIdx());
            adouble payment = startDf / dfSum;

            InterestRate<adouble> prodRate = inst.rate();

            auto f = [&](adouble r) {
                InterestRate<adouble> tmpRate(r, prodRate.dayCounter(), prodRate.compounding(), prodRate.frequency());
                adouble totalRedemption = 0;
                for (const auto& coupon : coupons)
                    totalRedemption += payment - (tmpRate.compoundFactor(coupon.startDate(), coupon.endDate()) - 1) * (1 - totalRedemption);
                return pow(1 - totalRedemption, 2);
            };
            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                value_ = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g = [&](double r) { return val(f(r)); };
                value_ = solver_.solve(f, guess_, accuracy_, maxIter_);
            }
        };

       private:
        template <typename T>
        void evalFixedRateProd(const T& inst) const {
            T evalInst(inst);
            NPVCalculator<adouble> calc(marketData_);
            size_t pos           = evalInst.disbursement().dfIdx();
            adouble startDf      = marketData_.dfs.at(pos);
            adouble disbursement = evalInst.disbursement().amount();

            auto f = [&](adouble r) {
                calc.clear();
                evalInst.rate(r);
                calc.visit(evalInst);
                adouble npv     = calc.results();
                adouble results = npv + disbursement * startDf;
                return results;
            };

            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                value_ = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g = [&](double r) { return val(f(r)); };
                value_ = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        template <typename T>
        void evalFloatingRateProd(const T& inst) const {
            T evalInst(inst);
            NPVCalculator<adouble> calc(marketData_);
            size_t pos           = evalInst.disbursement().dfIdx();
            adouble startDf      = marketData_.dfs.at(pos);
            adouble disbursement = evalInst.disbursement().amount();

            std::function<adouble(adouble)> f = [&](adouble s) {
                calc.clear();
                evalInst.spread(s);
                calc.visit(evalInst);
                adouble npv    = calc.results();
                adouble result = npv + disbursement * startDf;
                return result;
            };

            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                value_ = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g = [&](double s) { return val(f(s)); };
                value_ = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        const MarketData<adouble>& marketData_;

        mutable adouble value_ = 0.0;
        double guess_;
        double accuracy_;
        double maxIter_;
    };
}  // namespace Atlas

#endif /* D897AD55_2529_41AF_98E6_ECC9CA3FF5CB */
