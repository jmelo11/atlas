#ifndef FF810950_C49F_4282_9ADF_B5DA1F8DB7B1
#define FF810950_C49F_4282_9ADF_B5DA1F8DB7B1

#include <ql/math/solvers1d/brent.hpp>
#include <atlas/atlasconfig.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateinstrument.hpp>
#include <atlas/visitors/forwardrateforecaster.hpp>
#include <type_traits>

namespace Atlas {

    /**
     * @brief ZSpreadCalculator class
     *
     * @tparam adouble
     */
    template <typename adouble>
    class ZSpreadCalculator : public ConstVisitor<adouble> {
       public:
        /**
         * @brief Construct a new ZSpreadCalculator object
         *
         * @param marketData market data from a model
         * @param targetNPV target npv to solve
         * @param dayCounter implied rate day counter
         * @param comp implied rate compounding
         * @param freq implied rate frequency
         * @param guess search guess
         * @param accuracy accuracy
         * @param maxIter maximum iterations
         */
        ZSpreadCalculator(const MarketData<adouble>& marketData, adouble targetNPV, const DayCounter& dayCounter = Actual360(),
                          Compounding comp = Compounding::Compounded, Frequency freq = Frequency::Semiannual, double guess = 0.01,
                          double accuracy = 1e-8, size_t maxIter = 100)
        : marketData_(marketData),
          targetNPV_(targetNPV),
          dayCounter_(dayCounter),
          comp_(comp),
          freq_(freq),
          guess_(guess),
          accuracy_(accuracy),
          maxIter_(maxIter){};

        void setTargetNPV(adouble targetNPV) { targetNPV_ = targetNPV; }
        void visit(const FixedRateInstrument<adouble>& inst) const override { zspread(inst); }
        void visit(const FloatingRateInstrument<adouble>& inst) const override { zspread(inst); }

        void clear() { value_ = 0.0; };

        adouble results() const { return value_; }

       private:
        template <class T>
        void zspread(const T& inst) const {
            T evalInst(inst);

            if constexpr (std::is_base_of_v<FloatingRateInstrument<adouble>, T>) {
                ForwardRateForecaster<adouble> forecaster(marketData_);
                evalInst.accept(forecaster);
            }

            auto f = [&](adouble z) {
                adouble npv = 0.0;
                auto& leg   = evalInst.leg();
                for (auto& coupon : leg.coupons()) {
                    adouble df = marketData_.dfs.at(coupon.dfIdx());
                    adouble r =
                        InterestRate<adouble>::impliedRate(1 / df, dayCounter_, comp_, freq_, marketData_.refDate, coupon.paymentDate()).rate();
                    r += z;
                    npv += coupon.amount() / fastCompoundFactor(r, dayCounter_, comp_, freq_, marketData_.refDate, coupon.paymentDate());
                }

                for (auto& redemption : leg.redemptions()) {
                    adouble df = marketData_.dfs.at(redemption.dfIdx());
                    adouble r =
                        InterestRate<adouble>::impliedRate(1 / df, dayCounter_, comp_, freq_, marketData_.refDate, redemption.paymentDate()).rate();
                    r += z;
                    npv += redemption.amount() / fastCompoundFactor(r, dayCounter_, comp_, freq_, marketData_.refDate, redemption.paymentDate());
                }
                return targetNPV_ - npv;
            };

            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                value_ = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g = [&](double x) { return val(f(x)); };
                value_ = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        mutable adouble value_ = 0.0;
        const MarketData<adouble>& marketData_;
        adouble targetNPV_;
        DayCounter dayCounter_;
        Compounding comp_;
        Frequency freq_;
        double guess_;
        double accuracy_;
        size_t maxIter_;
    };
}  // namespace Atlas

#endif /* FF810950_C49F_4282_9ADF_B5DA1F8DB7B1 */
