#ifndef AB28EE82_AE8E_4177_BC69_0AB9ADB32D04
#define AB28EE82_AE8E_4177_BC69_0AB9ADB32D04

#include <atlas/atlasconfig.hpp>

namespace Atlas {

    template <typename adouble>
    class YieldTermStructureStrategy {
       public:
        virtual ~YieldTermStructureStrategy() = default;

        virtual adouble discount(const Date& date) const = 0;

        virtual adouble discount(double t) const = 0;

        virtual adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                                    Frequency freq) const = 0;

        virtual std::unique_ptr<YieldTermStructureStrategy> copy() const = 0;

        Date refDate() const {
            if (refDate_ == Date()) { throw std::invalid_argument("Reference date not set"); }
            return refDate_;
        };

       protected:
        Date refDate_;
    };

    /**
     * @class YieldTermStructure
     * @brief A yield term structure.
     */
    template <typename adouble>
    class YieldTermStructure {
       public:
        YieldTermStructure(std::unique_ptr<YieldTermStructureStrategy<adouble>> strategy) : strategy_(std::move(strategy)) {}

        Date refDate() const { return strategy_->refDate(); };

        adouble discount(const Date& date) const { return strategy_->discount(date); };

        adouble discount(adouble t) const { return strategy_->discount(t); };

        adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter = Actual360(),
                            Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual) const {
            return strategy_->forwardRate(startDate, endDate, dayCounter, comp, freq);
        };

        std::unique_ptr<YieldTermStructure> copy() const { return std::make_unique<YieldTermStructure>(strategy_->copy()); }

       private:
        std::unique_ptr<YieldTermStructureStrategy<adouble>> strategy_;
    };
}  // namespace Atlas

#endif /* AB28EE82_AE8E_4177_BC69_0AB9ADB32D04 */
