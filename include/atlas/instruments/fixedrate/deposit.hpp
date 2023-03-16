#ifndef B50701D3_345F_46BA_A774_CC39E13E6595
#define B50701D3_345F_46BA_A774_CC39E13E6595

#include <ql/interestrate.hpp>
#include <atlas/instruments/fixedrateproduct.hpp>

namespace Atlas {
    class Deposit : public FixedRateProduct {
       public:
        Deposit(const Date& startDate, const Date& endDate, double notional,
                const InterestRate& rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* B50701D3_345F_46BA_A774_CC39E13E6595 */
