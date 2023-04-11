#ifndef D867AADA_2F45_4391_8716_07815C63DD2F
#define D867AADA_2F45_4391_8716_07815C63DD2F

#include <atlas/cashflows/legs/leg.hpp>
#include <atlas/instruments/doubleleginstrument.hpp>

namespace Atlas {

    /**
     * @brief FxForward instrument
     *
     * @tparam adouble
     */
    template <typename adouble>
    class FxForward : public DoubleLegInstrument<adouble, Leg<adouble>, Leg<adouble>> {
       public:
        /**
         * @brief Construct a new Forward object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param fwdPrice
         * @param notional
         * @param currency
         */
        FxForward(const Date& startDate, const Date& endDate, adouble fwdPrice, double notional, Currency curr1, Currency curr2, Side side)
        : DoubleLegInstrument<adouble, Leg<adouble>, Leg<adouble>>(startDate, endDate, notional, Leg<adouble>(), Leg<adouble>(), side),
          fwdPrice_(fwdPrice) {
            Cashflow<adouble> cashflow1(startDate, fwdPrice * notional, curr1.numericCode());
            Cashflow<adouble> cashflow2(endDate, notional, curr2.numericCode());
            this->firstLeg().addRedemption(cashflow1);
            this->secondLeg().addRedemption(cashflow2);
        }

        adouble fwdPrice() const { return fwdPrice_; }

        void fwdPrice(adouble fwdPrice) {
            fwdPrice_ = fwdPrice;
            this->firstLeg().redemptions()[0].amount(fwdPrice * this->notional());
        }

        void accept(Visitor<adouble>& v) override { v.visit(*this); }

        void accept(ConstVisitor<adouble>& v) const override { v.visit(*this); }

       private:
        adouble fwdPrice_;
    };
}  // namespace Atlas

#endif /* D867AADA_2F45_4391_8716_07815C63DD2F */
