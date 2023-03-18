#ifndef DFF76924_C230_47B5_BBF1_EC18E72E8DC4
#define DFF76924_C230_47B5_BBF1_EC18E72E8DC4

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {
    class CustomFloatingRateInstrument : public FloatingRateInstrument {
       public:
        CustomFloatingRateInstrument(std::vector<Date> dates, std::vector<double> redemptions, double spread,
                                     const RateIndexConfiguration& index = RateIndexConfiguration());
    };
}  // namespace Atlas

#endif /* DFF76924_C230_47B5_BBF1_EC18E72E8DC4 */
