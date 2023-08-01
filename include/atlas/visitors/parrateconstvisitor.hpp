#ifndef A1E2580C_4F8E_4D05_B898_6E62936CC252
#define A1E2580C_4F8E_4D05_B898_6E62936CC252

#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>

namespace Atlas {

    /**
     * @class ParRateConstVisitor
     * @brief A class for calculating the par rate of an instrument. It is a const visitor, so it does not modify the instrument. It calculates the
     * needed spread, rate or other value that makes the NPV of the instrument equal to zero.
     * @ingroup Visitors
     *
     * @tparam adouble The number type used for the calculations. Can be double or an AAD type.
     */
    template <typename adouble>
    class ParRateConstVisitor : public BaseConstVisitor<adouble> {
       public:
        /**
         * @brief Construct a new Par Rate Const Visitor object
         *
         * @param marketData A MarketData object containing the market data needed for pricing.
         * @param guess Starting value for the solver.
         * @param accuracy Precision of the solver.
         * @param maxIter Maximum number of iterations for the solver.
         */
        ParRateConstVisitor(const MarketData<adouble>& marketData, double guess = 0.0, double accuracy = 0.00001, double maxIter = 100)
        : marketData_(marketData), guess_(guess), accuracy_(accuracy), maxIter_(maxIter){};

        void operator()(const std::monostate& inst) const override { this->template printLogs<ParRateConstVisitor>(this, "monostate"); }

        /**
         * @brief Calculates the par fixed rate of the instrument.
         *
         * @param inst CustomFixedRateInstrument
         */
        void operator()(const CustomFixedRateInstrument<adouble>& inst) const override { evalFixedRateProd(inst); };

        /**
         * @brief Calculates the par fixed rate of the instrument.
         *
         * @param inst EqualPaymentInstrument
         */
        void operator()(const FixedRateBulletInstrument<adouble>& inst) const override { evalFixedRateProd(inst); };

        /**
         * @brief Calculates the par fixed rate of the instrument.
         *
         * @param inst EqualPaymentInstrument
         */
        void operator()(const EqualPaymentInstrument<adouble>& inst) const override {
            const auto& coupons = inst.leg().coupons();
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
                results_["rate"] = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g           = [&](double r) { return val(f(r)); };
                results_["rate"] = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        /**
         * @brief Calculates the par fixed rate of the instrument.
         *
         * @param inst ZeroCouponInstrument
         */
        void operator()(const ZeroCouponInstrument<adouble>& inst) const override { evalFixedRateProd(inst); };

        /**
         * @brief Calculates the par spread of the instrument.
         *
         * @param inst CustomFloatingRateInstrument
         */
        void operator()(const CustomFloatingRateInstrument<adouble>& inst) const override { evalFloatingRateProd(inst); };

        /**
         * @brief Calculates the par spread of the instrument.
         *
         * @param inst FloatingRateBulletInstrument
         */
        void operator()(const FloatingRateBulletInstrument<adouble>& inst) const override { evalFloatingRateProd(inst); };

        /**
         * @brief Calculates the par spread of the instrument.
         *
         * @param inst FloatingRateEqualRedemptionInstrument
         */
        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& inst) const override { evalFloatingRateProd(inst); };

        /**
         * @brief Not implemented.
         *
         * @param inst FxForward
         */
        void operator()(const FxForward<adouble>& inst) const override { throw std::runtime_error("Not implemented"); };

        /**
         * @brief Not implemented.
         *
         * @param inst FixFloatSwap
         */
        void operator()(const FixFloatSwap<adouble>& inst) const override { throw std::runtime_error("Not implemented"); };

        void reset() const { results_.clear(); };

        std::unordered_map<std::string, adouble> getResults() const { return results_; };

       private:
        template <typename T>
        void evalFixedRateProd(const T& inst) const {
            T evalInst(inst);
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            size_t pos           = evalInst.disbursement().dfIdx();
            adouble startDf      = marketData_.dfs.at(pos);
            adouble disbursement = evalInst.disbursement().amount();
            adouble fx           = 1.0;
            if (evalInst.disbursement().applyCcy()) { fx = marketData_.fxs.at(evalInst.disbursement().fxIdx()); }

            auto f = [&](adouble r) {
                npvVisitor.reset();
                evalInst.rate(r);
                npvVisitor(evalInst);
                adouble npv = npvVisitor.getResults();

                adouble results = npv + disbursement * startDf / fx;
                return results;
            };

            reset();
            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                results_["rate"] = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g           = [&](double r) { return val(f(r)); };
                results_["rate"] = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        template <typename T>
        void evalFloatingRateProd(const T& inst) const {
            T evalInst(inst);
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            FixingVisitor<adouble> fixingVisitor(marketData_);
            fixingVisitor(evalInst);

            size_t pos           = evalInst.disbursement().dfIdx();
            adouble startDf      = marketData_.dfs.at(pos);
            adouble disbursement = evalInst.disbursement().amount();

            adouble fx = 1.0;
            if (evalInst.disbursement().applyCcy()) { fx = marketData_.fxs.at(evalInst.disbursement().fxIdx()); }

            auto f = [&](adouble s) {
                npvVisitor.reset();
                evalInst.spread(s);
                npvVisitor(evalInst);
                adouble npv    = npvVisitor.getResults();
                adouble result = npv + disbursement * startDf / fx;
                return result;
            };

            reset();
            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                results_["spread"] = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g             = [&](double s) { return val(f(s)); };
                results_["spread"] = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        const MarketData<adouble>& marketData_;
        mutable std::unordered_map<std::string, adouble> results_;
        double guess_;
        double accuracy_;
        double maxIter_;
    };
}  // namespace Atlas

#endif /* A1E2580C_4F8E_4D05_B898_6E62936CC252 */
