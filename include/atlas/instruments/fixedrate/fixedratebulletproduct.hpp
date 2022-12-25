#ifndef D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE
#define D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateproduct.hpp>

namespace Atlas {
    class FixedRateBulletProduct : public FixedRateProduct {
       public:
        FixedRateBulletProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate, QuantLib::Frequency freq, double notional,
                               const QuantLib::InterestRate& rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE */
