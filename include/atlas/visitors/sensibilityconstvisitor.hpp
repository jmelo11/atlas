#ifndef D4CE9AFF_45B9_4BB9_B17A_D1462FC4F1D6
#define D4CE9AFF_45B9_4BB9_B17A_D1462FC4F1D6

#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>

namespace Atlas {

    /**
     * @class SensibilityConstVisitor
     * @brief Visitor in charge of calculating some sensibilities of a product.
     * @details For fixed income, the sensibility is calculated w.r.t the coupon rate (fixed rate instruments) or the coupon spread (floating rate
     * instruments).
     * @ingroup Visitors
     *
     * @tparam adouble
     */
    template <typename adouble>
    class SensibilityConstVisitor : public BaseConstVisitor<adouble> {
       public:
        /**
         * @brief Construct a new Sensibility Const Visitor object
         *
         * @param marketData  MarketData object
         * @param delta      Sensibility step
         * @param showLogs   Show logs
         */
        SensibilityConstVisitor(const MarketData<adouble>& marketData, double delta = 0.0001, bool showLogs = false)
        : BaseConstVisitor<adouble>(showLogs), marketData_(marketData), delta_(delta){};

        /**
         * @brief Get the Results object
         *
         * @return std::unordered_map<std::string, adouble> map of results
         */
        std::unordered_map<std::string, adouble> getResults() const { return results_; }

        void reset() { results_.clear(); }

        /**
         * @brief
         *
         * @param inst
         */
        void operator()(const std::monostate& inst) const override { this->template printLogs<SensibilityConstVisitor>(this, "monostate"); }

        /**
         * @brief Calculate the coupon sensibility of a FixedRateBulletInstrument.
         *
         * @param inst FixedRateBulletInstrument
         */
        void operator()(const FixedRateBulletInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const FloatingRateBulletInstrument<adouble>& inst) const override { floatingInstSens(inst); }

        void operator()(const EqualPaymentInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const ZeroCouponInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const CustomFixedRateInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const CustomFloatingRateInstrument<adouble>& inst) const override { floatingInstSens(inst); }

        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& inst) const override { floatingInstSens(inst); }

        void operator()(const FxForward<adouble>& inst) const override {
            throw std::runtime_error("SensibilityConstVisitor: FxForward not implemented");
        }

        void operator()(const FixFloatSwap<adouble>& inst) const override {
            throw std::runtime_error("SensibilityConstVisitor: FxSwap not implemented");
        }

       private:
        template <typename T>
        void fixedInstSens(const T& inst) const {
            T tmpProd = inst;
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            InterestRate<adouble> rate = tmpProd.rate();

            auto f = [&](adouble r) {
                npvVisitor.reset();
                tmpProd.rate(r);
                npvVisitor(tmpProd);
                return npvVisitor.getResults();
            };
            adouble npv                = f(rate.rate());
            adouble npv_               = f(rate.rate() + delta_);
            results_["couponRateSens"] = (npv_ - npv) / npv / delta_;
        };

        template <typename T>
        void floatingInstSens(const T& inst) const {
            T tmpProd      = inst;
            adouble spread = tmpProd.spread();
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            FixingVisitor<adouble> fixingVisitor(marketData_);
            fixingVisitor(tmpProd);

            auto f = [&](adouble s) {
                npvVisitor.reset();
                tmpProd.spread(s);
                npvVisitor(tmpProd);
                return npvVisitor.getResults();
            };
            adouble npv            = f(spread);
            adouble npv_           = f(spread + delta_);
            results_["spreadSens"] = (npv_ - npv) / npv / delta_;
        };

        const MarketData<adouble>& marketData_;
        double delta_;
        mutable std::unordered_map<std::string, adouble> results_;
    };
}  // namespace Atlas

#endif /* D4CE9AFF_45B9_4BB9_B17A_D1462FC4F1D6 */
