
#include <ql/math/array.hpp>
#include <ql/math/matrix.hpp>
#include <ql/time/schedule.hpp>
#include <algorithm>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/equalpaymentproduct.hpp>
#include <atlas/instruments/fixedrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    EqualPaymentProduct::EqualPaymentProduct(const QuantLib::Date& start, const QuantLib::Date& end,
                                             QuantLib::Frequency freq, double notional,
                                             QuantLib::InterestRate rate) {
        QuantLib::Schedule schedule =
            QuantLib::MakeSchedule().from(start).to(end).withFrequency(freq);
        std::vector<Redemption> redemptions =
            FixedRateProduct::calculateRedemptions(schedule.dates(), rate, notional);

        std::vector<FixedRateCoupon> coupons = calculateNotionals(redemptions);
        std::vector<Cashflow> cashflows;

        cashflows.insert(cashflows().begin(), redemptions.begin(), redemptions.end());
        cashflows.insert(cashflows().begin(), coupons.begin(), coupons.end());
        legs_.push_back(Leg(cashflows));
    }

    void EqualPaymentProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void EqualPaymentProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

    std::vector<Redemption> EqualPaymentProduct::calculateRedemptions(
        const std::vector<QuantLib::Date>& dates, const QuantLib::InterestRate& rate,
        double notional) const {
        size_t pN = dates.size() - 1;
        size_t kN = pN + 1;
        Array factors(kN, 0), B(kN, 0), K;

        for (size_t i = 1; i <= pN; i++)
            factors[i - 1] = (rate.compoundFactor(dates.at(i - 1), dates.at(i)) - 1);

        factors[kN - 1] = -1;

        B = -notional * factors;

        Matrix A(kN, kN, 0);
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
        std::vector<Cashflow> cashflows;
        for (size_t i = 0; i < dates.size(); i++) {
            Redemption redemption(K[i], dates[i]);
            cashflows.push_back(redemption);
        }
        return cashflows;
    }

}  // namespace Atlas
