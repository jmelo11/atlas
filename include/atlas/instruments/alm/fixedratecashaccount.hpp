#ifndef F50C20F0_41E7_423F_837B_B3613AA8ACC9
#define F50C20F0_41E7_423F_837B_B3613AA8ACC9

#include <atlas/instruments/alm/cashaccount.hpp>
#include <atlas/rates/interestrate.hpp>

namespace Atlas {

    template <typename adouble = double>
    class FixedRateCashAccount : public CashAccount<FixedRateLeg, adouble> {
       public:
        FixedRateCashAccount(const Date& refDate, adouble initBalance, const Currency& ccy, const InterestRate<adouble>& rate)
        : CashAccount<FixedRateLeg, adouble>(refDate, initBalance, ccy), rate_(rate){};

        void createCoupons() override {
            auto notionals = this->notionalsFromBalance();
            auto& leg      = this->leg();
            leg.clear();

            Date startRefDate = this->refDate();
            adouble amount    = this->balance(startRefDate);
            for (auto& [date, notional] : notionals) {
                if (date != refDate_) {
                    leg.addCoupon(FixedRateCoupon<adouble>(startRefDate, date, notional, rate_));
                    startRefDate = date;
                }
            }
        };

        const InterestRate<adouble>& rate() const { return rate_; }

        void rate(const InterestRate<adouble>& rate) { rate_ = rate; }

       private:
        InterestRate<adouble> rate_;
    }
}  // namespace Atlas

#endif /* F50C20F0_41E7_423F_837B_B3613AA8ACC9 */
