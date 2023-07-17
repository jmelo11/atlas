#ifndef C8964333_99C5_4B58_B60E_1881A7E5EE1F
#define C8964333_99C5_4B58_B60E_1881A7E5EE1F

#include <atlas/rates/interestrate.hpp>
#include <atlas/rates/yieldtermstructure.hpp>
#include <atlas/rates/yieldtermstructurestrategy.hpp>
namespace Atlas {

    /**
     * @brief A class representing a flat forward curve
     *
     * @tparam adouble
     */
    template <typename adouble>
    class FlatForwardStrategy : public YieldTermStructureStrategy<adouble> {
       public:
        FlatForwardStrategy(const Date& refDate, adouble rate, const DayCounter& dayCounter = Actual360(), Compounding comp = Compounding::Simple,
                            Frequency freq = Frequency::Annual)
        : rate_(InterestRate<adouble>(rate, dayCounter, comp, freq)) {
            this->refDate_ = refDate;
        }

        FlatForwardStrategy(const Date& refDate, const InterestRate<adouble>& rate) : rate_(rate) { this->refDate_ = refDate; }

        adouble discount(const Date& date) const override {
            if (date < this->refDate_) { throw std::invalid_argument("Date is before reference date"); }
            double t = rate_.dayCounter().yearFraction(this->refDate_, date);
            return discount(t);
        };

        adouble discount(double t) const override {
            adouble df = 1 / rate_.compoundFactor(t);
            return df;
        };

        adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                            Frequency freq) const override {
            if (startDate < this->refDate_) { throw std::invalid_argument("Start date is before reference date"); }
            adouble compound = discount(startDate) / discount(endDate);
            return InterestRate<adouble>::impliedRate(compound, dayCounter, comp, freq, startDate, endDate).rate();
        };

        std::unique_ptr<YieldTermStructureStrategy<adouble>> clone() const override {
            return std::make_unique<FlatForwardStrategy<adouble>>(this->refDate_, rate_);
        };

       private:
        InterestRate<adouble> rate_;
    };

    /**
     * @brief More ergonomic class. Should replace DiscountStrategy. Naming convention will remain.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class FlatForwardTermStructure : public BaseYieldTermStructure<adouble> {
       private:
        class Strategy : public BaseYieldTermStructure<adouble>::Strategy {
           public:
            explicit Strategy(const Date& refDate, adouble rate, const DayCounter& dayCounter = Actual360(), Compounding comp = Compounding::Simple,
                              Frequency freq = Frequency::Annual)
            : BaseYieldTermStructure<adouble>::Strategy(refDate), rate_(InterestRate<adouble>(rate, dayCounter, comp, freq)) {}

            explicit Strategy(const Date& refDate, const InterestRate<adouble>& rate)
            : BaseYieldTermStructure<adouble>::Strategy(refDate), rate_(rate) {}

            adouble discount(const Date& date) const override {
                if (date < this->refDate_) { throw std::invalid_argument("Date is before reference date"); }
                double t = rate_.dayCounter().yearFraction(this->refDate_, date);
                return discount(t);
            };

            adouble discount(double t) const override {
                adouble df = 1 / rate_.compoundFactor(t);
                return df;
            };

            adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                                Frequency freq) const override {
                if (startDate < this->refDate_) { throw std::invalid_argument("Start date is before reference date"); }
                adouble compound = discount(startDate) / discount(endDate);
                return InterestRate<adouble>::impliedRate(compound, dayCounter, comp, freq, startDate, endDate).rate();
            };

            std::unique_ptr<typename BaseYieldTermStructure<adouble>::Strategy> clone() const override { return std::make_unique<Strategy>(*this); };

            void enableExtrapolation(bool e) override { throw std::invalid_argument("Extrapolation not supported"); };

           private:
            InterestRate<adouble> rate_;
        };

       public:
        explicit FlatForwardTermStructure(const Date& refDate, adouble rate, const DayCounter& dayCounter = Actual360(),
                                          Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual)
        : BaseYieldTermStructure<adouble>(std::make_unique<Strategy>(refDate, rate, dayCounter, comp, freq)){};

        explicit FlatForwardTermStructure(const Date& refDate, const InterestRate<adouble>& rate)
        : BaseYieldTermStructure<adouble>(std::make_unique<Strategy>(refDate, rate)){};
    };
}  // namespace Atlas

#endif /* C8964333_99C5_4B58_B60E_1881A7E5EE1F */
