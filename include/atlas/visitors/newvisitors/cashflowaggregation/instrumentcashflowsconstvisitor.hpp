#ifndef AFF213A1_1FA7_4798_B1BE_A686D920610F
#define AFF213A1_1FA7_4798_B1BE_A686D920610F

#include <atlas/instruments/derivatives/fixfloatswap.hpp>
#include <atlas/instruments/derivatives/fxforward.hpp>
#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/newvisitors/basevisitor.hpp>

namespace Atlas {

    template <typename adouble = double>
    class InstrumentCashflowsConstVisitor : public BaseConstVisitor<adouble> {
       public:
        InstrumentCashflowsConstVisitor() = default;

        void operator()(const std::monostate& inst) const override { this->template printLogs<InstrumentCashflowsConstVisitor>(this, "monostate"); }

        void operator()(const CustomFixedRateInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.leg().coupons());
            paymentProfile(instrument.leg().redemptions());
        }

        void operator()(const EqualPaymentInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.leg().coupons());
            paymentProfile(instrument.leg().redemptions());
        }

        void operator()(const FixedRateBulletInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.leg().coupons());
            paymentProfile(instrument.leg().redemptions());
        }

        void operator()(const ZeroCouponInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.leg().coupons());
            paymentProfile(instrument.leg().redemptions());
        }

        void operator()(const FloatingRateBulletInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.leg().coupons());
            paymentProfile(instrument.leg().redemptions());
        }

        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.leg().coupons());
            paymentProfile(instrument.leg().redemptions());
        }

        void operator()(const CustomFloatingRateInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.leg().coupons());
            paymentProfile(instrument.leg().redemptions());
        }

        void operator()(const FxForward<adouble>& instrument) const override { paymentProfile(instrument.leg().redemptions()); }

        void operator()(const FixFloatSwap<adouble>& instrument) const override {
            paymentProfile(instrument.firstLeg().coupons());
            paymentProfile(instrument.firstLeg().redemptions());
            paymentProfile(instrument.secondLeg().coupons());
            paymentProfile(instrument.secondLeg().redemptions());
        }

        void reset() { paymentProfile_.clear(); }

        std::map<std::string, std::map<Date, double>> getResults() const { return paymentProfile_; }

       private:
        template <template <typename> typename C>
        void paymentProfile(const std::vector<C<adouble>>& cashflows) const {
            for (const auto& coupon : cashflows) {
                if constexpr (std::is_base_of_v<Coupon<adouble>, C<adouble>>) {
                    if (paymentProfile_["interests"].find(coupon.paymentDate()) == paymentProfile_["interests"].end())
                        paymentProfile_["interests"][coupon.paymentDate()] = 0.0;

                    paymentProfile_["interests"][coupon.paymentDate()] += coupon.amount();
                } else if constexpr (std::is_same_v<Redemption<adouble>, C<adouble>>) {
                    if (paymentProfile_["redemptions"].find(coupon.paymentDate()) == paymentProfile_["redemptions"].end())
                        paymentProfile_["redemptions"][coupon.paymentDate()] = 0.0;
                    paymentProfile_["redemptions"][coupon.paymentDate()] += coupon.amount();
                }
            }
        }

        mutable std::map<std::string, std::map<Date, double>> paymentProfile_;
    };
}  // namespace Atlas

#endif /* AFF213A1_1FA7_4798_B1BE_A686D920610F */
