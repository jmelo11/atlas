#ifndef EC8B8DF3_FC72_439D_9B5C_58E1BD780311
#define EC8B8DF3_FC72_439D_9B5C_58E1BD780311

#include <atlas/rates/index/index.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {
    template <typename adouble = double>
    class OvernightPriceIndex : public Index {
       public:
        OvernightPriceIndex(const YieldTermStructure& curve, const RateDefinition& rateDef = RateDefinition())
        : Index(Period(1, TimeUnit::Days), rateDef){};

       private:
        class Strategy : public Index<adouble>::Strategy {
           public:
            Strategy(const YieldTermStructure& curve, const RateDefinition& rateDef)
            : curve_(curve), Index<adouble>::Strategy(Period(1, TimeUnit::Days), rateDef){};

            std::unique_ptr<typename Index<adouble>::Strategy> clone() const override { return std::make_unique<Strategy>(*this); };

            adouble forecastRate(const Date& fixingDate, const Date& startDate, const Date& endDate) const override {
                Date refDate = this->refDate();
                if (startDate >= refDate && endDate <= refDate) {
                    adouble current = this->fixing(fixingDate);
                    adouble prev    = this->fixing(startDate);
                    adouble comp    = (current / prev) - 1.0;
                    adouble r1      = InterestRate<adouble>::impliedRate(comp, this->rateDef_.dayCounter(), this->rateDef_.compounding(),
                                                                         this->rateDef_.frequency(), startDate, fixingDate);

                    adouble r2 =
                        curve_.forwardRate(startDate, endDate, this->rateDef_.dayCounter(), this->rateDef_.compounding(), this->rateDef_.frequency());

                    return (r1 + r2) / 2.0;  // should be wa
                } else if (startDate < refDate && endDate <= refDate) {
                    adouble current = this->fixing(fixingDate);
                    adouble prev    = this->fixing(startDate);
                    adouble comp    = (current / prev) - 1.0;
                    return InterestRate<adouble>::impliedRate(comp, this->rateDef_.dayCounter(), this->rateDef_.compounding(),
                                                              this->rateDef_.frequency(), startDate, fixingDate);
                } else if (startDate >= refDate && endDate > refDate) {
                    adouble r2 =
                        curve_.forwardRate(startDate, endDate, this->rateDef_.dayCounter(), this->rateDef_.compounding(), this->rateDef_.frequency());
                    return r2;
                } else {
                    return 0.0;
                }
            };

           protected:
            YieldTermStructure curve_;
        };
    }
}  // namespace Atlas

#endif /* EC8B8DF3_FC72_439D_9B5C_58E1BD780311 */
