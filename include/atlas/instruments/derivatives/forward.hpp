#ifndef D867AADA_2F45_4391_8716_07815C63DD2F
#define D867AADA_2F45_4391_8716_07815C63DD2F

#include <atlas/cashflows/legs/leg.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {

    /**
     * @brief Forward instrument
     *
     * @tparam adouble
     */
    template <typename adouble>
    class Forward : public Instrument<adouble> {
       public:
        enum Side { Pay = 1, Recieve = -1 };

        /**
         * @brief Construct a new Forward object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param fwdPrice
         * @param notional
         * @param currency
         */
        Forward(const Date& startDate, const Date& endDate, adouble fwdPrice, double notional, Currency curr1, Currency curr2, Side side)
        : Instrument<adouble>(startDate, endDate, notional), fwdPrice_(fwdPrice), side_(side) {
            Cashflow<adouble> cashflow1(startDate, fwdPrice * notional, curr1.numericCode());
            Cashflow<adouble> cashflow2(endDate, notional, curr2.numericCode());
            leg_.addRedemption(cashflow1);
            leg_.addRedemption(cashflow1);
        }

        adouble fwdPrice() const { return fwdPrice_; }

        void fwdPrice(adouble fwdPrice) {
            fwdPrice_ = fwdPrice;
            leg_.coupons()[0].amount(fwdPrice * this->notional());
        }

        const Leg<adouble>& leg() const { return leg_; }

        Leg<adouble>& leg() { return leg_; }

        void accept(Visitor<adouble>& v) override { v.visit(*this); }

        void accept(ConstVisitor<adouble>& v) const override { v.visit(*this); }

       private:
        adouble fwdPrice_;
        Side side_;
        Leg<adouble> leg_;
    };
}  // namespace Atlas

#endif /* D867AADA_2F45_4391_8716_07815C63DD2F */
