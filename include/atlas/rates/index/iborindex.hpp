#ifndef B9DDDEF2_F7EF_4207_B393_F42E83F1C639
#define B9DDDEF2_F7EF_4207_B393_F42E83F1C639

#include <atlas/rates/index/index.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {
    /**
     * @class IborIndex
     * @brief This class has the fixings and rate definition of a given index.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class IborIndex : public Index<adouble> {
       private:
        class Strategy : public Index<adouble>::Strategy {
           public:
            Strategy(const YieldTermStructure<adouble>& curve, const Period& period, const RateDefinition& rateDef)
            : curve_(curve), Index<adouble>::Strategy(period, rateDef){};

            std::unique_ptr<typename Index<adouble>::Strategy> clone() const override { return std::make_unique<Strategy>(*this); };

            adouble forecastRate(const Date& fixingDate, const Date& startDate, const Date& endDate) const override {
                Date fd = fixingDate == Date() ? fixingDate : startDate;
                if (fixingDate <= this->historicalFixings_.begin()->first) {
                    return this->fixing(fixingDate);
                } else {
                    return curve_.forwardRate(startDate, endDate, this->rateDef_.dayCounter(), this->rateDef_.compounding(),
                                              this->rateDef_.frequency());
                }
            };

           private:
            YieldTermStructure<adouble> curve_;
        };

       public:
        explicit IborIndex(const YieldTermStructure<adouble>& curve, const Period& period, const RateDefinition& rateDef = RateDefinition())
        : Index<adouble>(std::make_unique<Strategy>(curve, period, rateDef)){};

        explicit IborIndex(const YieldTermStructure<adouble>& curve, Frequency period, const RateDefinition& rateDef = RateDefinition())
        : Index<adouble>(std::make_unique<Strategy>(curve, Period(period), rateDef)){};
    };
}  // namespace Atlas

#endif /* B9DDDEF2_F7EF_4207_B393_F42E83F1C639 */
