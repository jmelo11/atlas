#ifndef A44280F0_C2FC_4939_991C_18E6A449EEA6
#define A44280F0_C2FC_4939_991C_18E6A449EEA6

#include <atlas/rates/interestrate.hpp>
#include <atlas/rates/yieldtermstructure.hpp>

namespace Atlas {

    /**
     * @brief A class representing a zero rate curve
     * 
     * @tparam adouble 
     * @tparam Interpol 
     */
    template <typename adouble, typename Interpol>
    class ZeroRateStrategy : public YieldTermStructureStrategy<adouble> {
       public:
        ZeroRateStrategy(const std::vector<Date>& dates, const std::vector<adouble>& zeroRates, const DayCounter& dayCounter,
                         Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual)
        : dayCounter_(dayCounter), dates_(dates), zeroRates_(zeroRates), comp_(comp), freq_(freq) {
            if (dates.size() != zeroRates.size()) { throw std::invalid_argument("dates and zeroRates must have the same size"); }
            if (dates.size() < 2) { throw std::invalid_argument("dates and zeroRates must have at least 2 elements"); }
            times_    = std::vector<double>(dates.size());
            times_[0] = 0.0;

            for (auto i = 1; i < dates.size(); ++i) {
                if (dates[i] < dates[i - 1]) { throw std::invalid_argument("dates must be sorted"); }
                double t = dayCounter.yearFraction(dates[0], dates[i]);
                if (t < 0) throw std::invalid_argument("dates must be greater than reference date");
                times_[i] = t;
            }
            this->refDate_ = dates[0];
            interpol_      = Interpol(times_, zeroRates_);
        }

        adouble discount(const Date& date) const override  {
            double t = dayCounter_.yearFraction(this->refDate_, date);
            if (t < 0) throw std::invalid_argument("date must be greater than reference date");
            InterestRate<adouble> rate(interpol_(t), dayCounter_, comp_, freq_);
            return rate.discountFactor(t);
        };

        adouble discount(double t) const override {
            InterestRate<adouble> rate(interpol_(t), dayCounter_, comp_, freq_);
            return rate.discountFactor(t);
        };

        adouble forwardRate(const Date& startDate, const Date& endDate, const DayCounter& dayCounter, Compounding comp,
                            Frequency freq) const override {
            double t1     = dayCounter_.yearFraction(this->refDate_, startDate);
            double t2     = dayCounter_.yearFraction(this->refDate_, endDate);
            adouble value = discount(t1) / discount(t2);
            auto r        = InterestRate<adouble>::impliedRate(value, dayCounter, comp, freq, startDate, endDate);
            return r.rate();
        };

        std::unique_ptr<YieldTermStructureStrategy<adouble>> clone() const override {
            return std::make_unique<ZeroRateStrategy<adouble, Interpol>>(*this);
        };

       private:
        DayCounter dayCounter_;
        std::vector<Date> dates_;
        std::vector<adouble> zeroRates_;
        std::vector<double> times_;
        Compounding comp_;
        Frequency freq_;
        Interpol interpol_;
    };
}  // namespace Atlas

#endif /* A44280F0_C2FC_4939_991C_18E6A449EEA6 */
