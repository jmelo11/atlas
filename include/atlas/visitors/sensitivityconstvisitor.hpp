#ifndef D4CE9AFF_45B9_4BB9_B17A_D1462FC4F1D6
#define D4CE9AFF_45B9_4BB9_B17A_D1462FC4F1D6

#include <atlas/visitors/fixingvisitor.hpp>
#include <atlas/visitors/npvconstvisitor.hpp>

namespace Atlas {

    /**
     * @class SensitivityConstVisitor
     * @brief Visitor in charge of calculating some sensibilities of a product.
     * @details For fixed income, the sensibility is calculated w.r.t the coupon rate (fixed rate instruments) or the coupon spread (floating rate
     * instruments).
     * @ingroup Visitors
     *
     * @tparam adouble
     */
    template <typename adouble>
    class SensitivityConstVisitor : public BaseConstVisitor<adouble> {
       public:
        struct Results {
            adouble couponSens = 0.0;
            adouble spreadSens = 0.0;
        };

        /**
         * @brief Construct a new Sensitivity Const Visitor object
         *
         * @param marketData  MarketData object
         * @param delta      Sensitivity step
         * @param showLogs   Show logs
         */
        SensitivityConstVisitor(const MarketData<adouble>& marketData, double delta = 0.0001, bool showLogs = false)
        : BaseConstVisitor<adouble>(showLogs), marketData_(marketData), delta_(delta){};

        /**
         * @brief Get the Results object
         *
         * @return Results map of results
         */
        inline Results getResults() const { return results_; }

        /**
         * @brief Reset the results
         *
         */
        void reset() const { results_ = Results(); }

        /**
         * @brief
         *
         * @param inst
         */
        void operator()(const std::monostate& inst) const override { this->template printLogs<SensitivityConstVisitor>(this, "monostate"); }

        /**
         * @brief Calculate the coupon sensibility of a FixedRateBulletInstrument.
         *
         * @param inst FixedRateBulletInstrument
         */
        void operator()(const FixedRateBulletInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const FloatingRateBulletInstrument<adouble>& inst) const override { floatingInstSens(inst); }

        void operator()(const EqualPaymentInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const ZeroCouponFixedRateInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const FixedRateEqualRedemptionInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const CustomFixedRateInstrument<adouble>& inst) const override { fixedInstSens(inst); }

        void operator()(const CustomFloatingRateInstrument<adouble>& inst) const override { floatingInstSens(inst); }

        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& inst) const override { floatingInstSens(inst); }

        void operator()(const ZeroCouponFloatingRateInstrument<adouble>& inst) const override { floatingInstSens(inst); }

       private:
        template <typename T>
        void fixedInstSens(const T& inst) const {
            T tmpProd = inst;
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            InterestRate<adouble> rate = tmpProd.rate();

            npvVisitor(tmpProd);
            adouble sensNPV    = npvVisitor.getResults().fixedRateCouponsNPV;
            adouble nonSensNPV = -npvVisitor.getResults().disbursementsNPV;

            npvVisitor.reset();

            tmpProd.rate(rate.rate() + delta_);
            npvVisitor(tmpProd);
            adouble sensNPV_ = npvVisitor.getResults().fixedRateCouponsNPV;

            std::lock_guard<std::mutex> lock(mtx_);
            reset();
            results_.couponSens = (sensNPV_ - sensNPV) / nonSensNPV / delta_;
        };

        template <typename T>
        void floatingInstSens(const T& inst) const {
            T tmpProd      = inst;
            adouble spread = tmpProd.spread();
            NPVConstVisitor<adouble> npvVisitor(marketData_);
            FixingVisitor<adouble> fixingVisitor(marketData_);
            fixingVisitor(tmpProd);

            npvVisitor(tmpProd);
            adouble sensNPV    = npvVisitor.getResults().floatingRateCouponsNPV;
            adouble nonSensNPV = -npvVisitor.getResults().disbursementsNPV;

            npvVisitor.reset();

            tmpProd.spread(spread + delta_);
            npvVisitor(tmpProd);
            adouble sensNPV_ = npvVisitor.getResults().floatingRateCouponsNPV;

            std::lock_guard<std::mutex> lock(mtx_);
            reset();
            results_.spreadSens = (sensNPV_ - sensNPV) / nonSensNPV / delta_;
        };

        mutable std::mutex mtx_;
        const MarketData<adouble>& marketData_;
        double delta_;
        mutable Results results_ = Results();
    };
}  // namespace Atlas

#endif /* D4CE9AFF_45B9_4BB9_B17A_D1462FC4F1D6 */
