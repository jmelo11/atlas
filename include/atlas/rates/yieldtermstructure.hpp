#ifndef AB28EE82_AE8E_4177_BC69_0AB9ADB32D04
#define AB28EE82_AE8E_4177_BC69_0AB9ADB32D04

#include <atlas/atlasconfig.hpp>
#include <atlas/rates/yieldtermstructurestrategy.hpp>

namespace Atlas {

    /**
     * @class YieldTermStructure
     * @brief A yield term structure. The strategy pattern is used to allow for different implementations of the yield
     * term structure.
     */
    template <typename adouble>
    class YieldTermStructure {
       public:
        YieldTermStructure(std::unique_ptr<YieldTermStructureStrategy<adouble>> strategy) : strategy_(std::move(strategy)) {}

        Date refDate() const { return strategy_->refDate(); };

        adouble discount(const Date& date) const { return strategy_->discount(date); };

        adouble discount(double t) const { return strategy_->discount(t); };

        adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter = Actual360(),
                            Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual) const {
            return strategy_->forwardRate(startDate, endDate, dayCounter, comp, freq);
        };

        std::unique_ptr<YieldTermStructure> clone() const { return std::make_unique<YieldTermStructure>(strategy_->clone()); }

        // copy constructor
        YieldTermStructure(const YieldTermStructure& other) : strategy_(other.strategy_->clone()) {}

        // copy assignment
        YieldTermStructure& operator=(const YieldTermStructure& other) {
            if (this != &other) { strategy_ = other.strategy_->clone(); }
            return *this;
        }

       private:
        std::unique_ptr<YieldTermStructureStrategy<adouble>> strategy_;
    };

    /**
     * @class BaseYieldTermStructure
     * @brief More ergonomic version of the YieldTermStructure class. To be renamed in future versions.
     */
    template <typename adouble>
    class BaseYieldTermStructure {
       protected:
        class Strategy {
           public:
            Strategy(const Date& refDate) : refDate_(refDate){};

            virtual ~Strategy() = default;

            virtual adouble discount(const Date& date) const = 0;

            virtual adouble discount(double t) const = 0;

            virtual adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                                        Frequency freq) const = 0;

            virtual std::unique_ptr<Strategy> clone() const = 0;

            Date refDate() const {
                if (refDate_ == Date()) { throw std::invalid_argument("Reference date not set"); }
                return refDate_;
            };

           protected:
            Date refDate_;
        };

        explicit BaseYieldTermStructure(std::unique_ptr<Strategy> strategy) : strategy_(std::move(strategy)) {}

       public:
        BaseYieldTermStructure() = default;

        inline adouble discount(const Date& date) const {
            if (empty()) throw std::runtime_error("No curve strategy defined.");
            strategy_->discount(date);
        };

        inline adouble discount(double t) const {
            if (empty()) throw std::runtime_error("No curve strategy defined.");
            strategy_->discount(t);
        };

        inline adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp, Frequency freq) const {
            if (empty()) throw std::runtime_error("No curve strategy defined.");
            strategy_->forwardRate(startDate, endDate, dayCounter, comp, freq);
        };

        inline Date refDate() { strategy_->refDate(); };

        inline bool empty() const { return !strategy_; }

       private:
        std::unique_ptr<Strategy> strategy_;
    };

}  // namespace Atlas

#endif /* AB28EE82_AE8E_4177_BC69_0AB9ADB32D04 */
