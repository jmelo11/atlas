#ifndef C8964333_99C5_4B58_B60E_1881A7E5EE1F
#define C8964333_99C5_4B58_B60E_1881A7E5EE1F

#include <atlas/rates/interestrate.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {

    template <typename adouble>
    class FlatForwardCurveStrategy : public YieldTermStructureStrategy<adouble> {
       public:
        FlatForwardCurveStrategy(const Date& refDate, adouble rate, const DayCounter& dayCounter = Actual360(),
                                 Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual)
        : rate_(InterestRate<adouble>(rate, dayCounter, comp, freq)) {
            this->refDate_ = refDate;
        }

        FlatForwardCurveStrategy(const InterestRate<adouble>& rate) : rate_(rate) {}

        adouble discount(const Date& date) const override {
            if (date < this->refDate_) { throw std::invalid_argument("Date is before reference date"); }
            return rate_.discountFactor(this->refDate_, date);
        };

        adouble discount(adouble t) const override { return rate_.discountFactor(t); };

        adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                            Frequency freq) const override {
            adouble compound = rate_.compoundFactor(startDate, endDate);
            return InterestRate<adouble>::impliedRate(compound, dayCounter, comp, freq, startDate, endDate).rate();
        };

        std::unique_ptr<YieldTermStructureStrategy<adouble>> copy() const override {
            return std::make_unique<FlatForwardCurveStrategy<adouble>>(rate_);
        };

       private:
        InterestRate<adouble> rate_;
    };
}  // namespace Atlas

#endif /* C8964333_99C5_4B58_B60E_1881A7E5EE1F */
