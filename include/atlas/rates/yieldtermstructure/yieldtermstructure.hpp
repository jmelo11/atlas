#ifndef E7DD6186_376A_4E0B_8E66_0D5EECBB1A18
#define E7DD6186_376A_4E0B_8E66_0D5EECBB1A18

#include <atlas/atlasconfig.hpp>

namespace Atlas {
    /**
     * @class YieldTermStructure
     * @brief A yield term structure is an object that provides functionality to get discount factors and forward rates.
     *
     * @tparam adouble
     */
    template <typename adouble = double>
    class YieldTermStructure {
       protected:
        class Strategy {
           public:
            Strategy(const Date& refDate) : refDate_(refDate){};

            Strategy() = default;

            virtual ~Strategy() = default;

            virtual adouble discount(const Date& date) const = 0;

            virtual adouble discount(double t) const = 0;

            virtual adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                                        Frequency freq) const = 0;

            virtual std::unique_ptr<Strategy> clone() const = 0;

            virtual void enableExtrapolation(bool e) = 0;

            Date refDate() const {
                if (refDate_ == Date()) { throw std::invalid_argument("Reference date not set"); }
                return refDate_;
            };

           protected:
            Date refDate_;
        };

        explicit YieldTermStructure(std::unique_ptr<Strategy> strategy) : strategy_(std::move(strategy)) {}

       public:
        YieldTermStructure() = default;

        YieldTermStructure(const YieldTermStructure<adouble>& rhs) {
            if (rhs.strategy_ != nullptr) strategy_ = rhs.strategy_->clone();
        }

        YieldTermStructure<adouble>& operator=(const YieldTermStructure<adouble>& rhs) {
            if (this != &rhs && rhs.strategy_ != nullptr) { strategy_ = rhs.strategy_->clone(); }
            return *this;
        }

        inline adouble discount(const Date& date) const {
            if (empty()) throw std::runtime_error("No curve strategy defined.");
            return strategy_->discount(date);
        };

        inline adouble discount(double t) const {
            if (empty()) throw std::runtime_error("No curve strategy defined.");
            return strategy_->discount(t);
        };

        inline adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp, Frequency freq) const {
            if (empty()) throw std::runtime_error("No curve strategy defined.");
            return strategy_->forwardRate(startDate, endDate, dayCounter, comp, freq);
        };

        inline Date refDate() const { return strategy_->refDate(); };

        inline bool empty() const { return !strategy_; }

        inline void enableExtrapolation(bool enable) { strategy_->enableExtrapolation(enable); }

       private:
        std::unique_ptr<Strategy> strategy_;
    };
}  // namespace Atlas

#endif /* E7DD6186_376A_4E0B_8E66_0D5EECBB1A18 */
