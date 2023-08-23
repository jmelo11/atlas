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
        struct Results {
            adouble parRate   = 0.0;
            adouble parSpread = 0.0;
        };

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
            const auto& coupons = inst.cashflows().fixedRateCoupons();
            adouble startDf     = marketData_.dfs.at(inst.cashflows().disbursement(0).dfIdx());
            adouble dfSum       = 0;
            for (const auto& coupon : coupons) dfSum += marketData_.dfs.at(coupon.dfIdx());
            adouble payment = startDf / dfSum;

            InterestRate<adouble> prodRate = inst.rate();

            auto f = [&](adouble r) {
                InterestRate<adouble> tmpRate(r, prodRate.dayCounter(), prodRate.compounding(), prodRate.frequency());
                adouble totalRedemption = 0;
                for (const auto& coupon : coupons)
                    totalRedemption += payment - (tmpRate.compoundFactor(coupon.startDate(), coupon.endDate()) - 1) * (1 - totalRedemption);
                return 1 - totalRedemption;
            };

            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                results_.parRate = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g           = [&](double r) { return val(f(r)); };
                results_.parRate = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        /**
         * @brief Calculates the par fixed rate of the instrument.
         *
         * @param inst ZeroCouponFixedRateInstrument
         */
        void operator()(const ZeroCouponFixedRateInstrument<adouble>& inst) const override { evalFixedRateProd(inst); };

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
         * @brief Calculates the par spread of the instrument.
         *
         */
        void operator()(const ZeroCouponFloatingRateInstrument<adouble>& inst) const override { evalFloatingRateProd(inst); };

        inline void reset() const { results_ = Results(); };

        inline Results getResults() const { return results_; };

       private:
        template <typename T>
        void evalFixedRateProd(const T& inst) const {
            T evalInst(inst);
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            auto f = [&](adouble r) {
                npvVisitor.reset();
                evalInst.rate(r);
                npvVisitor(evalInst);
                return npvVisitor.getResults().npv;
            };
            std::lock_guard<std::mutex> lock(mtx_);
            reset();
            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                results_.parRate = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g           = [&](double r) { return val(f(r)); };
                results_.parRate = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        template <typename T>
        void evalFloatingRateProd(const T& inst) const {
            T evalInst(inst);
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            FixingVisitor<adouble> fixingVisitor(marketData_);
            fixingVisitor(evalInst);

            auto f = [&](adouble s) {
                npvVisitor.reset();
                evalInst.spread(s);
                npvVisitor(evalInst);
                return npvVisitor.getResults().npv;
            };

            std::lock_guard<std::mutex> lock(mtx_);
            reset();
            QuantLib::Brent solver_;
            if constexpr (std::is_same_v<adouble, double>) {
                results_.parSpread = solver_.solve(f, accuracy_, guess_, 0.0001);
            } else {
                auto g             = [&](double s) { return val(f(s)); };
                results_.parSpread = solver_.solve(g, accuracy_, guess_, 0.0001);
            }
        };

        mutable std::mutex mtx_;
        const MarketData<adouble>& marketData_;
        double guess_;
        double accuracy_;
        double maxIter_;
        mutable Results results_ = Results();
    };
}  // namespace Atlas

#endif /* A1E2580C_4F8E_4D05_B898_6E62936CC252 */
