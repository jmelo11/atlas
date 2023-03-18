#ifndef CA42F9F0_E48D_4774_8366_AE0752280DAC
#define CA42F9F0_E48D_4774_8366_AE0752280DAC

#include <atlas/instruments/fixedrateinstrument.hpp>

namespace Atlas {
    class CustomFixedRateInstrument : public FixedRateInstrument {
       public:
        CustomFixedRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, const InterestRate& rate);
    };
}  // namespace Atlas

#endif /* CA42F9F0_E48D_4774_8366_AE0752280DAC */
