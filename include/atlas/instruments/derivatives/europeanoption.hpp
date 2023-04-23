#ifndef EED489F2_8667_4535_BE82_9C96B2298390
#define EED489F2_8667_4535_BE82_9C96B2298390

#include <atlas/atlasconfig.hpp>
#include <atlas/instruments/instrument.hpp>

namespace Atlas {
    class EuropeanOption : public Instrument {
       public:
        EuropeanOption(const Date& endDate, double strike, double notional);
    }
}  // namespace Atlas

#endif /* EED489F2_8667_4535_BE82_9C96B2298390 */
