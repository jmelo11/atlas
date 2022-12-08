#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrateproduct.hpp>

namespace Atlas {
    class CustomFloatingRateProduct : public FloatingRateProduct {
       public:
        CustomFloatingRateProduct(std::vector<QuantLib::Date> dates, std::vector<double> redemptions, double spread,
                                  const RateIndex& index = RateIndex());

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
