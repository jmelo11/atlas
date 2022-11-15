
#include <ql/math/array.hpp>
#include <ql/math/matrix.hpp>
#include <ql/time/schedule.hpp>
#include <atlas/cashflows/fixedratecoupon.hpp>
#include <atlas/cashflows/redemption.hpp>
#include <atlas/instruments/fixedrate/equalpaymentproduct.hpp>
#include <atlas/instruments/fixedrateproduct.hpp>
#include <atlas/visitors/visitor.hpp>

namespace Atlas {
    EqualPaymentProduct::EqualPaymentProduct(const QuantLib::Date& startDate,
                                             const QuantLib::Date& endDate,
                                             QuantLib::Frequency freq, double notional,
                                             const QuantLib::InterestRate& rate)
    : FixedRateProduct(startDate, endDate, rate, notional) {
        QuantLib::Schedule schedule =
            QuantLib::MakeSchedule().from(startDate).to(endDate).withFrequency(freq);

        calculateRedemptions(schedule.dates(), rate, notional);
        calculateNotionals(schedule.dates(), rate);
    }

    void EqualPaymentProduct::accept(Visitor& visitor) {
        visitor.visit(*this);
    }

    void EqualPaymentProduct::accept(ConstVisitor& visitor) const {
        visitor.visit(*this);
    }

    void EqualPaymentProduct::calculateRedemptions(const std::vector<QuantLib::Date>& dates,
                                                   const QuantLib::InterestRate& rate,
                                                   double notional) {
        size_t pN = dates.size() - 1;
        size_t kN = pN + 1;
        QuantLib::Array factors(kN, 0), B(kN, 0), K;

        for (size_t i = 1; i <= pN; i++)
            factors[i - 1] = (rate.compoundFactor(dates.at(i - 1), dates.at(i)) - 1);

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
            Redemption redemption(dates[i], K[i]);
            leg_.addRedemption(redemption);
        }
    }

}  // namespace Atlas
