#ifndef D867AADA_2F45_4391_8716_07815C63DD2F
#define D867AADA_2F45_4391_8716_07815C63DD2F

#include <atlas/cashflows/legs/leg.hpp>
#include <atlas/instruments/instrument.hpp>
#include <atlas/instruments/mixins/onelegmixin.hpp>

namespace Atlas {

    /**
     * @brief FxForward instrument
     *
     * @tparam adouble
     */
    template <typename adouble>
    class FxForward : public Instrument<adouble>, public OneLegMixin<Leg<adouble>> {
       public:
        enum Side { BUY = 1, SELL = -1 };

        /**
         * @brief Construct a new Forward object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param fwdPrice units of second currency per unit of first currency
         * @param curr1 currency context for first currency
         * @param curr2 currency context for second currency
         * @param notionalInSecondCcy notional in first currency
         * @example FxForward(Date(1, 1, 2020), Date(1, 1, 2021), 823, CLP , USD, 100_000, Side::BUY)  // Buying USD 100K forward @ 823 CLP
         */
        FxForward(const Date& startDate, const Date& endDate, adouble fwdPrice, const CurrencyContext<adouble>& ccy1,
                  const CurrencyContext<adouble>& ccy2, double notionalInSecondCcy, Side side)
        : fwdPrice_(fwdPrice), side_(side) {
            this->startDate_ = startDate;
            this->endDate_   = endDate;
            this->notional_  = notionalInSecondCcy;

            this->leg().addRedemption(Cashflow<adouble>(endDate, notionalInSecondCcy));
            this->leg().redemptions()[0].currencyContext(ccy2);
            adouble notionalInFirstCcy = notionalInSecondCcy * fwdPrice;
            this->leg().addRedemption(Cashflow<adouble>(endDate, notionalInFirstCcy));
            this->leg().redemptions()[1].currencyContext(ccy1);
        }

        /**
         * @brief Construct a new Fx Forward object
         *
         * @param startDate
         * @param endDate
         * @param fwdPrice
         * @param notionalInFirstCcy
         * @param curr1
         * @param curr2
         * @param side
         * @param discountCurve
         * @example FxForward(Date(1, 1, 2020), Date(1, 1, 2021), 823, CLP, USD, 100_000, Side::BUY, discountCurve)  // Buying USD 100K forward at
         * 823 CLP, collateralized at discountCurve (e.g. CLP overnight curve)
         */
        FxForward(const Date& startDate, const Date& endDate, adouble fwdPrice, const CurrencyContext<adouble>& curr1,
                  const CurrencyContext<adouble>& curr2, double notionalInSecondCcy, Side side, const CurveContext<adouble>& discountCurve)
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

        Side side() const { return side_; }

       private:
        adouble fwdPrice_;
        size_t ccy1_;
        size_t ccy2_;
        size_t discountContextIdx_;
        Side side_;
    };
}  // namespace Atlas

#endif /* D867AADA_2F45_4391_8716_07815C63DD2F */
