#ifndef BF2714F9_8C95_4D44_B693_CA617F9C3A64
#define BF2714F9_8C95_4D44_B693_CA617F9C3A64

#include <atlas/instruments/floatingrateproduct.hpp>

namespace Atlas {

    class RateIndex;

    class FloatingRateBulletProduct : public FloatingRateProduct {
       public:
        FloatingRateBulletProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate,
                                  double notional, double spread, const RateIndex& index);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* BF2714F9_8C95_4D44_B693_CA617F9C3A64 */
