#ifndef ADC0FE9F_28D9_4EB4_B1D8_D0F19961AC60
#define ADC0FE9F_28D9_4EB4_B1D8_D0F19961AC60

#include <atlas/curves/rateindex.hpp>

namespace Atlas {

    struct MetaData {
        MetaData(){};

        MetaData(const std::string& discountCurve, const RateIndex& index) : discountCurve_(discountCurve), index_(index) {}

        std::string discountCurve_ = "undefined";
        RateIndex index_           = RateIndex();
    };

}  // namespace atlas

#endif /* ADC0FE9F_28D9_4EB4_B1D8_D0F19961AC60 */
