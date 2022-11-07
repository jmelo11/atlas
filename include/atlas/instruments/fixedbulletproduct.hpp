#ifndef D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE
#define D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE

#include <ql/interestrate.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {
    class FixedBulletProduct : public Instrument {
       public:
        FixedBulletProduct(const QuantLib::Date& start, const QuantLib::Date& end,
                           QuantLib::Frequency freq, double notional, QuantLib::InterestRate rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* D00AFD6C_AA3F_43AC_B12A_E9BC237A26AE */
