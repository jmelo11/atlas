#ifndef F4126FFB_083C_4F17_8D47_AFE69F5C4B5A
#define F4126FFB_083C_4F17_8D47_AFE69F5C4B5A

#include <atlas/instruments/floatingrateproduct.hpp>

namespace Atlas {
    class FloatingRateEqualRedemptionProduct : public FloatingRateProduct {
       public:
        FloatingRateEqualRedemptionProduct(const QuantLib::Date& startDate, const QuantLib::Date& endDate, double notional, double spread,
                                           const RateIndex& index);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* F4126FFB_083C_4F17_8D47_AFE69F5C4B5A */
