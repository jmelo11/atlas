#ifndef F41148D8_C863_45B8_B7B2_E88435ADFC89
#define F41148D8_C863_45B8_B7B2_E88435ADFC89

#include <atlas/cashflows/legs/fixedrateleg.hpp>
#include <atlas/cashflows/legs/floatingrateleg.hpp>

namespace Atlas {

    template <typename adouble, class LegType>
    class MakeLeg {
       public:
        MakeLeg() : leg_() {}

        MakeLeg& notional(adouble notional) {
            notional_ = notional;
            return *this;
        }

        MakeLeg& paymentFrequency(Frequency frequency) {
            paymentFrequency_ = frequency;
            return *this;
        }

        MakeLeg& startDate(const Date& startDate) {
            startDate_ = startDate;
            return *this;
        }

        MakeLeg& endDate(const Date& endDate) {
            endDate_ = endDate;
            return *this;
        }

        MakeLeg& currency(const Currency& currency) {
            currency_ = currency;
            return *this;
        }

        MakeLeg& discountCurveContext(const Context<YieldTermStructure<adouble>>* discountCurve) {
            discountCurveContext_ = discountCurve;
            return *this;
        }

        MakeLeg& paymentConvention(BusinessDayConvention paymentConvention) {
            paymentConvention_ = paymentConvention;
            return *this;
        }

        MakeLeg& redemptions(std::vector<adouble>& redemptions) {
            redemptions_ = redemptions;
            return *this;
        }

        MakeLeg& calendar(const Calendar& calendar) {
            calendar_ = calendar;
            return *this;
        }

        MakeLeg& rate(const InterestRate<adouble>& rate) {
            static_assert(std::is_same_v<LegType, FixedRateLeg<adouble>>, "Only FixedRateLeg is supported for rate.");
            rate_ = rate;
            return *this;
        }

        MakeLeg& spread(adouble spread) {
            static_assert(std::is_same_v<LegType, FloatingRateLeg<adouble>>, "Only FloatingRateLeg is supported for spread.");
            spread_ = spread;
            return *this;
        }

        MakeLeg& rateIndexContext(const Context<RateIndex<adouble>>* index) {
            static_assert(std::is_same_v<LegType, FloatingRateLeg<adouble>>, "Only FloatingRateLeg is supported.");
            rateIndexContext_ = index;
            return *this;
        }

        LegType build() {
            Schedule schedule = MakeSchedule()
                                    .from(startDate_)
                                    .to(endDate_)
                                    .withFrequency(paymentFrequency_)
                                    .withCalendar(calendar_)
                                    .withConvention(paymentConvention_);
            auto dates = schedule.dates();
            LegType leg;

            std::vector<adouble> couponNotionals = setRedemptions(dates, leg);

            if constexpr (std::is_same_v<LegType, FixedRateLeg<adouble>>) {
                for (size_t i = 0; i < dates.size() - 1; i++) {
                    double amount = 0.0;
                    if constexpr (std::is_same_v<adouble, double>) {
                        amount = couponNotionals[i];
                    } else {
                        amount = val(couponNotionals[i]);
                    }
                    FixedRateCoupon<adouble> coupon(dates[i], dates[i + 1], amount, rate_);
                    setFlow(coupon);
                    leg.addCoupon(coupon);
                }
            } else if constexpr (std::is_same_v<LegType, FloatingRateLeg<adouble>>) {
                for (size_t i = 0; i < dates.size() - 1; i++) {
                    double amount = 0.0;
                    if constexpr (std::is_same_v<adouble, double>) {
                        amount = couponNotionals[i];
                    } else {
                        amount = val(couponNotionals[i]);
                    }
                    FloatingRateCoupon<adouble> coupon(dates[i], dates[i + 1], amount, spread_, *rateIndexContext_);
                    setFlow(coupon);
                    leg.addCoupon(coupon);
                }
            }
            return leg;
        }

       private:
        template <class Flow>
        void setFlow(Flow& flow) {
            if (currency_ != Currency()) flow.currency(currency_);
            if (discountCurveContext_ != nullptr) flow.discountCurveContext(*discountCurveContext_);
        }

        std::vector<adouble> setRedemptions(const std::vector<Date>& dates, LegType& leg) {
            std::vector<adouble> couponAmounts;
            size_t n_dates = dates.size();

            if (redemptions_.size() == 0) {
                Redemption<adouble> redemption(endDate_, notional_);
                setFlow(redemption);
                leg.addRedemption(redemption);
                couponAmounts.resize(n_dates - 1, notional_);
            } else if (redemptions_.size() == n_dates - 1) {
                adouble totalRedemption = notional_;
                couponAmounts.reserve(n_dates);
                couponAmounts.push_back(notional_);

                for (size_t i = 1; i < n_dates; i++) {
                    Redemption<adouble> redemption(dates[i], redemptions_[i - 1]);
                    totalRedemption -= redemptions_[i - 1];
                    couponAmounts.push_back(totalRedemption);
                    setFlow(redemption);
                    leg.addRedemption(redemption);
                }
            } else {
                throw std::runtime_error("Redemptions size does not match schedule size");
            }

            return couponAmounts;
        }

        adouble notional_;
        Frequency paymentFrequency_;
        Date startDate_;
        Date endDate_;
        const Context<RateIndex<adouble>>* rateIndexContext_ = nullptr;
        const Context<YieldTermStructure<adouble>>* discountCurveContext_ = nullptr;
        BusinessDayConvention paymentConvention_ = BusinessDayConvention::Unadjusted;
        Calendar calendar_                       = NullCalendar();
        Currency currency_                       = Currency();
        std::vector<adouble> redemptions_;
        LegType leg_;
        InterestRate<adouble> rate_;
        adouble spread_;
    };
}  // namespace Atlas

#endif /* F41148D8_C863_45B8_B7B2_E88435ADFC89 */
