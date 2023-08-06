#ifndef AFF213A1_1FA7_4798_B1BE_A686D920610F
#define AFF213A1_1FA7_4798_B1BE_A686D920610F

#include <atlas/instruments/fixedrate/customfixedrateinstrument.hpp>
#include <atlas/instruments/fixedrate/equalpaymentinstrument.hpp>
#include <atlas/instruments/fixedrate/fixedratebulletinstrument.hpp>
#include <atlas/instruments/fixedrate/zerocouponinstrument.hpp>
#include <atlas/instruments/floatingrate/customfloatingrateinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingratebulletinstrument.hpp>
#include <atlas/instruments/floatingrate/floatingrateequalredemptioninstrument.hpp>
#include <atlas/visitors/basevisitor.hpp>

namespace Atlas {

    template <typename adouble = double>
    class InstrumentCashflowsConstVisitor : public BaseConstVisitor<adouble> {
       public:
        struct Results {
            std::map<Date, double> redemptions;
            std::map<Date, double> disbursements;
            std::map<Date, double> fixedRateCoupons;
            std::map<Date, double> floatingRateCoupons;
        };

        InstrumentCashflowsConstVisitor() = default;

        void operator()(const std::monostate& inst) const override { this->template printLogs<InstrumentCashflowsConstVisitor>(this, "monostate"); }

        void operator()(const CustomFixedRateInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.cashflows().redemptions(), results_.redemptions);
            paymentProfile(instrument.cashflows().disbursements(), results_.disbursements);
            paymentProfile(instrument.cashflows().fixedRateCoupons(), results_.fixedRateCoupons);
        }

        void operator()(const EqualPaymentInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.cashflows().redemptions(), results_.redemptions);
            paymentProfile(instrument.cashflows().disbursements(), results_.disbursements);
            paymentProfile(instrument.cashflows().fixedRateCoupons(), results_.fixedRateCoupons);
        }

        void operator()(const FixedRateBulletInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.cashflows().redemptions(), results_.redemptions);
            paymentProfile(instrument.cashflows().disbursements(), results_.disbursements);
            paymentProfile(instrument.cashflows().fixedRateCoupons(), results_.fixedRateCoupons);
        }

        void operator()(const ZeroCouponInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.cashflows().redemptions(), results_.redemptions);
            paymentProfile(instrument.cashflows().disbursements(), results_.disbursements);
            paymentProfile(instrument.cashflows().fixedRateCoupons(), results_.fixedRateCoupons);
        }

        void operator()(const FloatingRateBulletInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.cashflows().redemptions(), results_.redemptions);
            paymentProfile(instrument.cashflows().disbursements(), results_.disbursements);
            paymentProfile(instrument.cashflows().floatingRateCoupons(), results_.floatingRateCoupons);
        }

        void operator()(const FloatingRateEqualRedemptionInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.cashflows().redemptions(), results_.redemptions);
            paymentProfile(instrument.cashflows().disbursements(), results_.disbursements);
            paymentProfile(instrument.cashflows().floatingRateCoupons(), results_.floatingRateCoupons);
        }

        void operator()(const CustomFloatingRateInstrument<adouble>& instrument) const override {
            paymentProfile(instrument.cashflows().redemptions(), results_.redemptions);
            paymentProfile(instrument.cashflows().disbursements(), results_.disbursements);
            paymentProfile(instrument.cashflows().floatingRateCoupons(), results_.floatingRateCoupons);
        }

        void reset() { results_ = Results(); }

        inline Results getResults() const { return results_; }

       private:
        template <template <typename> typename C>
        void paymentProfile(const std::vector<C<adouble>>& cashflows, std::map<Date, double>& map) const {
            for (const auto& cashflow : cashflows) {
                if (map.find(cashflow.paymentDate()) == map.end()) map[cashflow.paymentDate()] = 0.0;
                map[cashflow.paymentDate()] += cashflow.amount();
            }
        }

        mutable Results results_;
    };
}  // namespace Atlas

#endif /* AFF213A1_1FA7_4798_B1BE_A686D920610F */
