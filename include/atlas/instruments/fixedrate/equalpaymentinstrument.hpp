#ifndef AC01F282_C361_4B38_B915_0868E05A75BC
#define AC01F282_C361_4B38_B915_0868E05A75BC

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateinstrument.hpp>
#include <type_traits>

namespace Atlas {

    /**
     * @brief An class for equalk payment instruments.
     *
     */
    template <typename adouble>
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
         * @param recalcNotionals recalculate notionals based on the given rate
         */
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                               bool recalcNotionals = false)
        : FixedRateInstrument<adouble>(startDate, endDate, rate, notional), recalcNotionals_(recalcNotionals) {
            Schedule schedule = MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

            dates_ = schedule.dates();

            // calculate redemptions for a equal payment structure
            calculateRedemptions(dates_, rate, notional);

            // calculate each corresponding notional
            calculateNotionals(dates_, rate);
            this->disbursement_ = Cashflow<adouble>(startDate, -notional);
        };
        /**
         * @brief Construct a new Equal Payment Instrument object
         *
         * @param startDate start date of the instrument
         * @param endDate end date of the instrument
         * @param freq payment frequency of the instrument
         * @param notional notional of the instrument
         * @param rate rate of the instrument
         * @param recalcNotionals recalculate notionals based on the given rate
         * @param discountCurveContext discount curve context of the instrument
         */
        EqualPaymentInstrument(const Date& startDate, const Date& endDate, Frequency freq, double notional, const InterestRate<adouble>& rate,
                               const CurveContext& discountCurveContext, bool recalcNotionals = false)
        : EqualPaymentInstrument(startDate, endDate, freq, notional, rate, recalcNotionals) {
            this->leg().discountCurveContext(discountCurveContext);
            this->disbursement_.discountCurveContext(discountCurveContext);
        };

        void rate(const InterestRate<adouble>& rate) override {
            this->rate_ = rate;
            if (recalcNotionals_) {
                std::vector<size_t> redemptionIdxs;
                std::vector<size_t> couponIdxs;
                auto& redemptions = this->leg_.redemptions();
                auto& coupons     = this->leg_.coupons();
                size_t nCoupons   = coupons.size();
                for (size_t i = 0; i < nCoupons; ++i) {
                    redemptionIdxs.push_back(redemptions.at(i).dfIdx());
                    couponIdxs.push_back(coupons.at(i).dfIdx());
                }
                redemptions.clear();
                coupons.clear();
                calculateRedemptions(dates_, this->rate_, this->notional_);
                calculateNotionals(dates_, this->rate_);
                for (size_t i = 0; i < nCoupons; ++i) {
                    redemptions.at(i).dfIdx(redemptionIdxs.at(i));
                    coupons.at(i).dfIdx(couponIdxs.at(i));
                }
            } else {
                for (auto& coupon : this->leg_.coupons()) { coupon.rate(this->rate_); }
            }
        };

        void rate(adouble rate) override {
            InterestRate<adouble> tmpR(rate, this->rate_.dayCounter(), this->rate_.compounding(), this->rate_.frequency());
            rate(tmpR);
        };

        InterestRate<adouble> rate() const { return this->rate_; };

       private:
        void calculateRedemptions(const std::vector<Date>& dates, const InterestRate<adouble>& rate, double notional) {
            size_t pN = dates.size() - 1;
            size_t kN = pN + 1;
            QuantLib::Array factors(kN, 0), B(kN, 0), K;

            if constexpr (std::is_same_v<adouble, double>) {
                for (size_t i = 1; i <= pN; i++) factors[i - 1] = rate.compoundFactor(dates.at(i - 1), dates.at(i)) - 1;
            } else {
                for (size_t i = 1; i <= pN; i++) factors[i - 1] = rate.compoundFactor(dates.at(i - 1), dates.at(i)).val - 1;
            }

            factors[kN - 1] = -1;

            B = -notional * factors;

            QuantLib::Matrix A(kN, kN, 0);
            for (size_t j = 0; j < kN; j++) {
                for (size_t i = 0; i < kN; i++) {
                    if (j == 0 && i < kN - 1) {
                        A[i][j] = -1;
                    } else if (j > 0 && i == j - 1) {
                        A[i][j] = 1;
                    } else if (j > 0 && j <= i && i < kN - 1) {
                        A[i][j] = -factors[i];
                    } else if (i == kN - 1 && j > 0) {
                        A[i][j] = 1;
                    }
                }
            }
            // slow, multicore?
            K = inverse(A) * B;
            for (size_t i = 1; i < dates.size(); i++) {
                Redemption<adouble> redemption(dates[i], K[i]);
                this->leg_.addRedemption(redemption);
            }
        };

        bool recalcNotionals_;
        std::vector<Date> dates_;
    };

}  // namespace Atlas

#endif /* AC01F282_C361_4B38_B915_0868E05A75BC */
