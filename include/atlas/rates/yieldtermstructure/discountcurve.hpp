#ifndef A1CBC2C8_49CC_41E1_9304_45973FAD8B80
#define A1CBC2C8_49CC_41E1_9304_45973FAD8B80

#include <atlas/rates/interestrate.hpp>
#include <atlas/rates/yieldtermstructure/yieldtermstructure.hpp>

namespace Atlas {

    template <typename adouble, typename Interpol>
    class DiscountCurveStrategy : public YieldTermStructureStrategy<adouble> {
       public:
        DiscountCurveStrategy(const std::vector<Date>& dates, const std::vector<adouble>& dfs, const DayCounter& dayCounter)
        : dayCounter_(dayCounter), dates_(dates), dfs_(dfs) {
            if (dates.size() != dfs.size()) { throw std::invalid_argument("dates and dfs must have the same size"); }
            if (dates.size() < 2) { throw std::invalid_argument("dates and dfs must have at least 2 elements"); }
            times_    = std::vector<adouble>(dates.size());
            times_[0] = 0.0;

            for (auto i = 1; i < dates.size(); ++i) {
                if (dates[i] < dates[i - 1]) { throw std::invalid_argument("dates must be sorted"); }
                adouble t = dayCounter.yearFraction(dates[0], dates[i]);
                if (t < 0) throw std::invalid_argument("dates must be greater than reference date");
                times_[i] = t;
            }
            this->refDate_ = dates[0];
            interpol_      = Interpol(times_, dfs_);
        }

        adouble discount(const Date& date) const override {
            adouble t = dayCounter_.yearFraction(this->refDate_, date);
            if (t < 0) throw std::invalid_argument("date must be greater than reference date");
            return interpol_(t);
        };

        adouble discount(adouble t) const override { return interpol_(t); };

        adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                            Frequency freq) const override {
            adouble t1    = dayCounter_.yearFraction(this->refDate_, startDate);
            adouble t2    = dayCounter_.yearFraction(this->refDate_, endDate);
            adouble value = interpol_(t1) / interpol_(t2);
            auto r        = InterestRate<adouble>::impliedRate(value, dayCounter, comp, freq, startDate, endDate);
            return r.rate();
        };

        std::unique_ptr<YieldTermStructureStrategy<adouble>> copy() const override {
            return std::make_unique<DiscountCurveStrategy<adouble, Interpol>>(dates_, dfs_, dayCounter_);
        };

        void enableExtrapolation() { interpol_.enableExtrapolation(); }

       private:
        DayCounter dayCounter_;
        std::vector<adouble> times_;
        std::vector<Date> dates_;
        std::vector<adouble> dfs_;
        Interpol interpol_;
    };

}  // namespace Atlas

#endif /* A1CBC2C8_49CC_41E1_9304_45973FAD8B80 */
