/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef A912DD5A_E28A_4853_B842_2FB38CB28829
#define A912DD5A_E28A_4853_B842_2FB38CB28829

#include <ql/math/solvers1d/brent.hpp>
#include <atlas/data/marketdata.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {

    /* versiones individuales */
    class InterestCalculator : public Visitor {
       public:
        InterestCalculator(const MarketData& marketData) : marketData_(marketData) {}

        void visit(FloatingBullet& inst) override { forecast(inst.leg(), inst.spread()); }
        void visit(FloatingCustomStructure& inst) override { forecast(inst.leg(), inst.spread()); }

        void visit(FixedCustomStructure& inst) override { calculateInterests(inst.leg(), inst.rate()); };

        void visit(DAP& inst) override { calculateInterests(inst.leg(), inst.rate()); };

        void visit(EqualPayment& inst) override{
            // calculateInterests(inst.leg(), inst.rate());
        };

        void visit(FixedBullet& inst) override { calculateInterests(inst.leg(), inst.rate()); };

        void visit(FixFloatSwap& inst) override {
            calculateInterests(inst.legs()[0], inst.rate());
            forecast(inst.legs()[1], inst.spread());
        };

       private:
        void calculateInterests(Leg& leg, const InterestRate& rate) {
            size_t periods        = leg.size();
            const auto& notionals = leg.outstanding();
            const auto& dates     = leg.dates();
            auto& interest        = leg.interest();
            interest.clear();
            if (notionals.empty() || notionals.size() != periods - 1) leg.calculateOutstanding();
            for (size_t i = 0; i < periods; i++) interest.push_back((rate.compoundFactor(dates.at(i), dates.at(i + 1)) - 1) * notionals.at(i));
        }

        void forecast(Leg& leg, double spread) {
            if (leg.outstanding().empty()) leg.calculateOutstanding();
            const auto& notionals = leg.outstanding();
            const auto& dates     = leg.dates();
            const auto& fwdIdxs   = leg.FWDIdx();
            auto& interest        = leg.interest();
            const size_t n        = dates.size() - 1;
            const auto& index     = leg.metaData().index_;
            interest.clear();
            for (size_t i = 0; i < n; i++) {
                double fwd = marketData_.fwds[fwdIdxs.at(i)];
                InterestRate r(fwd + spread, index.dayCounter_, index.rateComp_, index.rateFreq_);
                // std::cout << dates.at(i) << dates.at(i + 1) << "\n";
                double compF = r.compoundFactor(dates.at(i), dates.at(i + 1)) - 1;
                double x     = notionals.at(i) * compF;
                interest.push_back(x);
            }
        }
        const MarketData marketData_;
    };

    class NPVCalculator : public ConstVisitor {
       public:
        NPVCalculator(const MarketData& marketData) : marketData_(marketData) {}

        void visit(const DAP& inst) const override { npv_ += npv(inst.constLeg()); }

        void visit(const EqualPayment& inst) const override { npv_ += npv(inst.constLeg()); }

        void visit(const FixedBullet& inst) const override { npv_ += npv(inst.constLeg()); }

        void visit(const FloatingBullet& inst) const override { npv_ += npv(inst.constLeg()); }

        void visit(const FloatingCustomStructure& inst) const override { npv_ += npv(inst.constLeg()); }

        void visit(const FixedCustomStructure& inst) const override { npv_ += npv(inst.constLeg()); }

        void visit(const FixFloatSwap& inst) const override {
            int flag = inst.side() == FixFloatSwap::Side::Receiver ? 1 : -1;
            npv_ += npv(inst.constLegs()[0]) * flag;
            npv_ += npv(inst.constLegs()[1]) * -flag;
        }

        void reset() const { npv_ = 0; }

        double result() const { return npv_; }

       private:
        /*descuenta los flujos, partiendo del primer cupon y no desde el desembolso*/
        double npv(const Leg& leg) const {
            const auto& v           = leg.constDFIdx();
            const auto& interest    = leg.constInterest();
            const auto& redemptions = leg.constRedemptions();
            double npv              = 0;
            for (size_t i = 0; i < interest.size(); i++) {
                size_t pos = v.at(i + 1);
                double df  = marketData_.dfs[pos];
                npv += (interest.at(i) + redemptions.at(i)) * df;
            }
            return npv;
        }

        const MarketData marketData_;
        mutable double npv_ = 0;  // cambiar a atomic_float
    };

    /* todo en uno */
    /* const por que no guarda los valores en los vectores de los instrumentos, solo guarda el npv de forma local */
    class InterestAndNPVCalculator : public ConstVisitor {
       public:
        InterestAndNPVCalculator(const MarketData& marketData) : marketData_(marketData) {}

        void visit(const DAP& inst) const override { npv_ += npvAndForecast(inst.constLeg(), inst.rate()); }

        void visit(const EqualPayment& inst) const override { npv_ += npvAndForecast(inst.constLeg(), inst.rate()); }

        void visit(const FixedBullet& inst) const override { npv_ += npvAndForecast(inst.constLeg(), inst.rate()); }

        void visit(const FloatingBullet& inst) const override { npv_ += npvAndForecast(inst.constLeg(), inst.spread()); }

        void visit(const FloatingCustomStructure& inst) const override { npv_ += npvAndForecast(inst.constLeg(), inst.spread()); }

        void visit(const FixedCustomStructure& inst) const override { npv_ += npvAndForecast(inst.constLeg(), inst.rate()); }

        void visit(const FixFloatSwap& inst) const override {
            int flag = inst.side() == FixFloatSwap::Side::Receiver ? 1 : -1;
            npv_ += npvAndForecast(inst.constLegs()[0], inst.rate()) * flag;
            npv_ += npvAndForecast(inst.constLegs()[1], inst.spread()) * -flag;
        }

        void reset() const { npv_ = 0; }

        double result() const { return npv_; }

       private:
        double npvAndForecast(const Leg& leg, double spread) const {
            if (leg.constOutstanding().empty()) leg.calculateOutstanding();

            size_t periods          = leg.size();
            const auto& notionals   = leg.constOutstanding();
            const auto& redemptions = leg.constRedemptions();

            const auto& dates = leg.dates();

            const auto& dfIdxs  = leg.constDFIdx();
            const auto& fwdIdxs = leg.constFWDIdx();

            const auto& index = leg.metaData().index_;

            double npv = 0;
            for (size_t i = 0; i < periods; i++) {
                size_t dfPos  = dfIdxs.at(i + 1);
                size_t fwdPos = fwdIdxs.at(i);
                double df     = marketData_.dfs[dfPos];
                double fwd    = marketData_.fwds[fwdPos];
                InterestRate r(fwd + spread, index.dayCounter_, index.rateComp_, index.rateFreq_);
                double interest = notionals.at(i) * (r.compoundFactor(dates.at(i), dates.at(i + 1)) - 1);
                npv += (interest + redemptions.at(i)) * df;
            }
            return npv;
        };

        double npvAndForecast(const Leg& leg, const InterestRate& rate) const {
            if (leg.constOutstanding().empty()) leg.calculateOutstanding();

            size_t periods          = leg.size();
            const auto& notionals   = leg.constOutstanding();
            const auto& redemptions = leg.constRedemptions();

            const auto& dates = leg.dates();

            const auto& dfIdxs = leg.constDFIdx();

            double npv = 0;
            for (size_t i = 0; i < periods; i++) {
                size_t dfPos    = dfIdxs.at(i + 1);
                double df       = marketData_.dfs[dfPos];
                double interest = notionals.at(i) * (rate.compoundFactor(dates.at(i), dates.at(i + 1)) - 1);
                npv += (interest + redemptions.at(i)) * df;
            }
            return npv;
        }

        const MarketData marketData_;
        mutable double npv_ = 0;
    };

    /*deberia ser const?*/
    /* si es un credito que ya partio, genera un error al tratar de calcular la tasa */
    class ParRateSolver : public Visitor {
       public:
        ParRateSolver(const MarketData& marketData, double accuracy = 0.0001, double guess = 0.01, double step = 0.0001)
        : marketData_(marketData), accuracy_(accuracy), guess_(guess), step_(step), npvCalc_(NPVCalculator(marketData)) {
            if (marketData_.dfs.empty()) throw std::runtime_error("MarketData does not contain any discount factors");
        }

        /* podria ser m�s eficiente al no ocupar el solver */
        void visit(DAP& inst) override {
            double startDF = marketData_.dfs[inst.constDFIdx().front()];
            if (startDF <= 0 + QL_EPSILON && startDF >= 0 - QL_EPSILON)
                throw std::runtime_error("Product start date must be greater or equal to the evaluation date");
            auto f = [&](double rate) {
                inst.rate(rate);
                inst.calculateInterests();

                npvCalc_.visit(inst);
                double npv = npvCalc_.result();
                npvCalc_.reset();
                return npv - inst.faceAmount() * startDF;
            };
            rate_ = solver_.solve(f, accuracy_, guess_, step_);
        }

        void visit(FixedBullet& inst) override {
            double startDF = marketData_.dfs[inst.constDFIdx().front()];
            if (startDF <= 0 + QL_EPSILON && startDF >= 0 - QL_EPSILON)
                throw std::runtime_error("Product start date must be greater or equal to the evaluation date");

            auto f = [&](double rate) {
                inst.rate(rate);
                inst.calculateInterests();

                npvCalc_.visit(inst);
                double npv = npvCalc_.result();
                npvCalc_.reset();
                return npv - inst.faceAmount() * startDF;
            };
            rate_ = solver_.solve(f, accuracy_, guess_, step_);
        }

        void visit(EqualPayment& inst) override {
            double startDF = marketData_.dfs[inst.constDFIdx().front()];
            if (startDF <= 0 + QL_EPSILON && startDF >= 0 - QL_EPSILON)
                throw std::runtime_error("Product start date must be greater or equal to the evaluation date");

            auto f = [&](double rate) {
                inst.rate(rate);
                // inst.calculateNotionals();
                inst.calculateInterests();

                npvCalc_.visit(inst);
                double npv = npvCalc_.result();
                npvCalc_.reset();
                return npv - inst.faceAmount() * startDF;
            };
            rate_ = solver_.solve(f, accuracy_, guess_, step_);
        }

        void visit(FloatingBullet& inst) override {}

        void visit(FloatingCustomStructure& inst) override {}

        void visit(FixedCustomStructure& inst) override {
            double startDF = marketData_.dfs[inst.constDFIdx().front()];
            if (startDF <= 0 + QL_EPSILON && startDF >= 0 - QL_EPSILON)
                throw std::runtime_error("Product start date must be greater or equal to the evaluation date");

            auto f = [&](double rate) {
                inst.rate(rate);
                inst.calculateInterests();

                npvCalc_.visit(inst);
                double npv = npvCalc_.result();
                npvCalc_.reset();
                return npv - inst.faceAmount() * startDF;
            };
            rate_ = solver_.solve(f, accuracy_, guess_, step_);
        }

        void visit(FixFloatSwap& inst) override{};

        double result() const { return rate_; }

       private:
        Brent solver_;
        double accuracy_ = 0;
        double guess_    = 0;
        double step_     = 0;

        NPVCalculator npvCalc_;
        const MarketData marketData_;
        double rate_ = 0;
    };

    class ParSpreadSolver : public Visitor {
       public:
        ParSpreadSolver(const MarketData& marketData, double accuracy = 0.0001, double guess = 0.01, double step = 0.0001)
        : marketData_(marketData), calc_(InterestAndNPVCalculator(marketData_)) {}

        void visit(DAP& inst) override {}

        void visit(FixedBullet& inst) override {}

        void visit(EqualPayment& inst) override {}

        void visit(FloatingBullet& inst) override {
            double startDF = marketData_.dfs[inst.constDFIdx().front()];
            if (startDF <= 0 + QL_EPSILON && startDF >= 0 - QL_EPSILON)
                throw std::runtime_error("Product start date must be greater or equal to the evaluation date");

            auto f = [&](double s) {
                inst.spread(s);

                calc_.visit(inst);
                double npv = calc_.result();
                calc_.reset();
                return npv - inst.faceAmount() * startDF;
            };
            spread_ = solver_.solve(f, accuracy_, guess_, step_);
        }

        void visit(FloatingCustomStructure& inst) override {
            double startDF = marketData_.dfs[inst.constDFIdx().front()];
            if (startDF <= 0 + QL_EPSILON && startDF >= 0 - QL_EPSILON)
                throw std::runtime_error("Product start date must be greater or equal to the evaluation date");

            auto f = [&](double s) {
                inst.spread(s);

                calc_.visit(inst);
                double npv = calc_.result();
                calc_.reset();
                return npv - inst.faceAmount() * startDF;
            };
            spread_ = solver_.solve(f, accuracy_, guess_, step_);
        }

        void visit(FixedCustomStructure& inst) override {}

        void visit(FixFloatSwap& inst) override{};

        double result() { return spread_; }

       private:
        InterestAndNPVCalculator calc_;
        const MarketData marketData_;
        double spread_ = 0;
        Brent solver_;
        double accuracy_ = 0;
        double guess_    = 0;
        double step_     = 0;
    };
}  // namespace atlas

#endif /* A912DD5A_E28A_4853_B842_2FB38CB28829 */
