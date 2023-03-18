#ifndef BF2714F9_8C95_4D44_B693_CA617F9C3A64
#define BF2714F9_8C95_4D44_B693_CA617F9C3A64

#include <atlas/instruments/floatingrateinstrument.hpp>

namespace Atlas {

    class RateIndex;

    class FloatingRateBulletInstrument : public FloatingRateInstrument {
       public:
        FloatingRateBulletInstrument(const Date& startDate, const Date& endDate,
                                  double notional, double spread, const RateIndexConfiguration& index);

       
    };
}  // namespace Atlas

#endif /* BF2714F9_8C95_4D44_B693_CA617F9C3A64 */
