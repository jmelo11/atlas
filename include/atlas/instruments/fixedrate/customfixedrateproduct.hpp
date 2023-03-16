#ifndef CA42F9F0_E48D_4774_8366_AE0752280DAC
#define CA42F9F0_E48D_4774_8366_AE0752280DAC

#include <atlas/instruments/fixedrateproduct.hpp>

namespace Atlas {
    class CustomFixedRateProduct : public FixedRateProduct {
       public:
        CustomFixedRateProduct(std::vector<Date> dates, std::vector<double> redemptions, const InterestRate& rate);

        void accept(Visitor& visitor) override;

        void accept(ConstVisitor& visitor) const override;
    };
}  // namespace Atlas

#endif /* CA42F9F0_E48D_4774_8366_AE0752280DAC */
