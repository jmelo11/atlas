#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/math/solvers1d/brent.hpp>
#include <atlas/instruments/fixedrate/fixedrateinstrument.hpp>

namespace Atlas {

    /**
     * @class EqualPaymentInstrument
     * @brief An class for equal payment instruments.
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
         */
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                               Side side = Side::Receive)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, side, notional), paymentFrequency_(freq) {
            Schedule schedule                     = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);
            std::vector<Date> dates               = schedule.dates();
            std::vector<double> redemptionAmounts = calculateRedemptions(dates, this->rate_, this->notional_, static_cast<int>(this->side_));

            auto invSide = side == Side::Receive ? Side::Pay : Side::Receive;
            this->cashflows_.addDisbursement(Cashflow<adouble>(startDate, notional, invSide));
            double tmpNotional = notional;
            for (size_t i = 1; i < dates.size(); i++) {
                this->cashflows_.addRedemption(Cashflow<adouble>(dates.at(i), redemptionAmounts.at(i - 1), side));
                this->cashflows_.addFixedRateCoupon(FixedRateCoupon<adouble>(dates.at(i - 1), dates.at(i), tmpNotional, rate, side));
                tmpNotional -= redemptionAmounts.at(i - 1);
            }
        };
        /**
         * @brief Construct a new Equal Payment Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param discountContextIdx discount curve context of the instrument
         * @param side side of the instrument
         */
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                               size_t discountContextIdx, Side side = Side::Receive)
        : EqualPaymentInstrument(startDate, endDate, freq, notional, rate, side) {
            this->cashflows_.discountContextIdx(discountContextIdx);
        };

        /**
         * @brief Construct a new Equal Payment Instrument object
         *
         * @param instrument
         * @param rate
         * @return EqualPaymentInstrument
         */
        static EqualPaymentInstrument withRate(const EqualPaymentInstrument& instrument, adouble rate) {
            InterestRate<adouble> newRate(rate, instrument.rate_.dayCounter(), instrument.rate_.compounding(), instrument.rate_.frequency());
            return EqualPaymentInstrument(instrument.startDate_, instrument.endDate_, instrument.paymentFrequency_, instrument.notional_, newRate,
                                          instrument.side_);
        }

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

        Frequency paymentFrequency_;
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
