#ifndef D867AADA_2F45_4391_8716_07815C63DD2F
#define D867AADA_2F45_4391_8716_07815C63DD2F

#include <atlas/cashflows/legs/leg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/instruments/mixins/onelegmixin.hpp>

namespace Atlas {

    /**
     * @class FxForward
     * @brief A forward contract on an FX rate
     *
     * @tparam adouble
     * @ingroup Derivatives
     */
    template <typename adouble = double>
    class FxForward : public Instrument<adouble>, public OneLegMixin<adouble, Leg<adouble>> {
       public:
        /**
         * @brief Construct a new Forward object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param fwdPrice units of second currency per unit of first currency
         * @param ccy1 currency context for first currency
         * @param ccy2 currency context for second currency
         * @param notionalInSecondCcy notional in first currency
         * @param side Long or Short
         */
        FxForward(const Date& startDate, const Date& endDate, adouble fwdPrice, const Currency& ccy1, const Currency& ccy2,
                  double notionalInSecondCcy, Side side)
        : fwdPrice_(fwdPrice) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notionalInSecondCcy;
            this->side_      = side;

            Cashflow<adouble> cf1(endDate, notionalInSecondCcy);
            cf1.currency(ccy1);

            adouble notionalInFirstCcy = notionalInSecondCcy * fwdPrice;
            Cashflow<adouble> cf2(endDate, notionalInFirstCcy);
            cf2.currency(ccy2);

            this->leg().addRedemption(cf1);
            this->leg().addRedemption(cf2);
        }

        /**
         * @brief Construct a new Fx Forward object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param fwdPrice units of second currency per unit of first currency
         * @param notionalInFirstCcy notional in first currency
         * @param curr1 first currency
         * @param curr2 second currency
         * @param side  BUY or SELL
         * @param discountCurve discount curve context
         */
        FxForward(const Date& startDate, const Date& endDate, adouble fwdPrice, const Currency& curr1, const Currency& curr2,
                  double notionalInSecondCcy, Side side, const Context<YieldTermStructure<adouble>>& discountCurve)
        : FxForward(startDate, endDate, fwdPrice, curr1, curr2, notionalInSecondCcy, side) {
            this->leg().redemptions()[0].discountCurveContext(discountCurve);
            this->leg().redemptions()[1].discountCurveContext(discountCurve);
        }

        adouble fwdPrice() const { return fwdPrice_; }

        void fwdPrice(adouble fwdPrice) {
            adouble notionalInFirstCcy = this->notional_ * fwdPrice;
            this->leg().redemptions()[1].amount(notionalInFirstCcy);
        }

        void accept(Visitor<adouble>& v) override { v.visit(*this); }

        void accept(ConstVisitor<adouble>& v) const override { v.visit(*this); }

       private:
        adouble fwdPrice_;
        size_t ccy1_;
        size_t ccy2_;
        size_t discountContextIdx_;
    };
}  // namespace Atlas

#endif /* D867AADA_2F45_4391_8716_07815C63DD2F */
