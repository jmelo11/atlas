#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/math/solvers1d/brent.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>
#include <type_traits>

namespace Atlas {

    /**
     * @brief An class for equalk payment instruments.
     * @ingroup FixedRateInstruments
     */
    template <typename adouble = double>
    class EqualPaymentInstrument : public FixedRateInstrument<adouble> {
       public:
        /**
         * @brief Construct a new Equal Payment Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param side side of the instrument
         * @param recalcNotionals recalculate notionals based on the given rate
         */
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                               Side side = Side::Long, bool recalcNotionals = false)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional), recalcNotionals_(recalcNotionals) {
            Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
            this->dates_      = schedule.dates();

            int flag = (side == Side::Long) ? 1 : -1;

            std::vector<double> redemptionAmounts = calculateRedemptions(this->dates_, this->rate_, this->notional_, flag);
            this->leg_ = MakeLeg<FixedRateLeg, adouble>().dates(this->dates_).redemptions(redemptionAmounts).rate(this->rate_).build();

            adouble disbursement = -this->notional_ * flag;
            this->disbursement(Cashflow<adouble>(startDate, disbursement));
        };
        /**
         * @brief Construct a new Equal Payment Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountCurveContext discount curve context of the instrument
         * @param side side of the instrument
         * @param recalcNotionals recalculate notionals based on the given rate
         */
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                               const Context<YieldTermStructure<adouble>>& discountCurveContext, Side side = Side::Long, bool recalcNotionals = false)
        : EqualPaymentInstrument(startDate, endDate, freq, notional, rate, side, recalcNotionals) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement().discountCurveContext(discountCurveContext);
        };

        void rate(const InterestRate<adouble>& r) override {
            this->rate_ = r;
            if (recalcNotionals_) {
                std::vector<size_t> redemptionIdxs;
                std::vector<size_t> couponIdxs;
                auto& redemptions = this->leg().redemptions();
                auto& coupons     = this->leg().coupons();
                size_t nCoupons   = coupons.size();
                for (size_t i = 0; i < nCoupons; ++i) {
                    redemptionIdxs.push_back(redemptions.at(i).dfIdx());
                    couponIdxs.push_back(coupons.at(i).dfIdx());
                }

                int flag = (this->side_ == Side::Long) ? 1 : -1;
                std::vector<double> redemptionAmounts = calculateRedemptions(this->dates_, this->rate_, this->notional_, flag);
                this->leg_ = MakeLeg<FixedRateLeg, adouble>().dates(this->dates_).redemptions(redemptionAmounts).rate(this->rate_).build();

                for (size_t i = 0; i < nCoupons; ++i) {
                    this->leg_.redemption(i).dfIdx(redemptionIdxs.at(i));
                    this->leg_.coupon(i).dfIdx(couponIdxs.at(i));
                }
            };
        }

        void rate(adouble r) override {
            InterestRate<adouble> tmpR(r, this->rate_.dayCounter(), this->rate_.compounding(), this->rate_.frequency());
            rate(tmpR);
        };

        InterestRate<adouble> rate() const { return this->rate_; };

        void accept(Visitor<adouble>& visitor) override { visitor.visit(*this); };

        void accept(ConstVisitor<adouble>& visitor) const override { visitor.visit(*this); };

       private:
        std::vector<double> calculateRedemptions(const std::vector<Date>& dates, const InterestRate<adouble>& rate, double notional, int side) {
            auto k = [&](double payment) {
                double totalAmount = notional;
                for (size_t i = 1; i < dates.size(); i++) {
                    if constexpr (std::is_same_v<adouble, double>) {
                        double I = totalAmount * (rate.compoundFactor(dates[i - 1], dates[i]) - 1);
                        totalAmount -= payment - I;
                    } else {
                        double I = totalAmount * (val(rate.compoundFactor(dates[i - 1], dates[i])) - 1);
                        totalAmount -= payment - I;
                    }
                }
                return totalAmount;
            };

            double r_;
            if constexpr (std::is_same_v<adouble, double>) {
                r_ = rate.rate() / 12;
            } else {
                r_ = val(rate.rate()) / 12;
            }
            int p        = (int)dates.size() - 1;
            double guess = r_ / (1 - pow(1 + r_, -p)) * notional;
            // double guess = 1;
            QuantLib::Brent solver_;
            double payment = solver_.solve(k, 1e-4, guess, 0.1);

            std::vector<double> redemptions(dates.size() - 1);
            double totalAmount = notional;
            for (size_t i = 1; i < dates.size(); i++) {
                double k = 0.0;
                if constexpr (std::is_same_v<adouble, double>) {
                    double I = totalAmount * (rate.compoundFactor(dates[i - 1], dates[i]) - 1);
                    k        = payment - I;
                } else {
                    double I = totalAmount * (val(rate.compoundFactor(dates[i - 1], dates[i])) - 1);
                    k        = payment - I;
                }
                totalAmount -= k;
                redemptions[i - 1] = k * side;
            }
            return redemptions;
        }

        bool recalcNotionals_;
        std::vector<Date> dates_;
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
