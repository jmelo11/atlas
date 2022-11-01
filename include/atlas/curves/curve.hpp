/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef D47BBB89_822D_4EF2_A7B1_3C262C68105A
#define D47BBB89_822D_4EF2_A7B1_3C262C68105A

#include <ql/compounding.hpp>
#include <ql/interestrate.hpp>
#include <ql/math/interpolations/linearinterpolation.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <vector>

namespace Atlas {

    using namespace QuantLib;

    class YieldCurve {
       public:
        YieldCurve(const DayCounter& dayCounter) : dayCounter_(dayCounter) {}
        YieldCurve(const Date& refDate, const DayCounter& dayCounter);

        virtual double df(double time) const { return 0; };

        virtual double df(const Date& date) const { return 0; };

        virtual double forwardDf(double t1, double t2);

        virtual double forwardDf(const Date& d1, const Date& d2);

        virtual double forwardRate(double t1, double t2, Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual) const;

        virtual double forwardRate(const Date& d1, const Date& d2, Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual) const;
        Date refDate() const { return refDate_; }

       protected:
        Date refDate_;
        DayCounter dayCounter_;
    };

    template <class Interpolator = Linear>
    class ZeroRateCurve : public YieldCurve {
       public:
        ZeroRateCurve(const std::vector<Date>& dates, const std::vector<double>& rates, const DayCounter& dayCounter = Actual360(),
                      Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual, const Interpolator& interpolator = Interpolator())
        : YieldCurve(dates[0], dayCounter), comp_(comp), freq_(freq) {
            if (dates.size() != rates.size()) throw std::runtime_error("dates and rates size do not match");

            std::vector<std::pair<Date, double>> nodes;
            for (size_t i = 0; i < dates.size(); i++) nodes.push_back({dates[i], rates[i]});

            std::sort(nodes.begin(), nodes.end());
            std::vector<Date> nodeDates;
            std::vector<double> nodeRates;

            for (const auto& node : nodes) {
                nodeDates.push_back(node.first);
                nodeRates.push_back(node.second);
            }

            for (const Date& date : nodeDates) times_.push_back(dayCounter.yearFraction(refDate_, date));
            rates_ = nodeRates;

            interpol_ = interpolator.interpolate(times_.begin(), times_.end(), rates_.begin());
            interpol_.enableExtrapolation();
        }

        ZeroRateCurve(const Date& refDate, const std::vector<double>& times, const std::vector<double>& rates,
                      const DayCounter& dayCounter = Actual360(), Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual,
                      const Interpolator& interpolator = Interpolator())
        : YieldCurve(refDate, dayCounter), comp_(comp), freq_(freq) {
            if (times.size() != rates.size()) throw std::runtime_error("dates and rates size do not match");

            std::vector<std::pair<double, double>> nodes;
            for (size_t i = 0; i < times.size(); i++) nodes.push_back({times[i], rates[i]});

            std::sort(nodes.begin(), nodes.end());
            for (const auto& node : nodes) {
                times_.push_back(node.first);
                rates_.push_back(node.second);
            }
            interpol_ = interpolator.interpolate(times_.begin(), times_.end(), rates_.begin());
        }

        double df(double time) const override {
            if (time == 0) return 1;
            InterestRate r(interpol_(time), dayCounter_, comp_, freq_);
            return r.discountFactor(time);
        }

        double df(const Date& date) const override {
            double t = dayCounter_.yearFraction(refDate_, date);
            return df(t);
        }

       private:
        Compounding comp_;
        Frequency freq_;
        std::vector<double> times_;
        std::vector<double> rates_;
        Interpolation interpol_;
    };

    template <class Interpolator = Linear>
    class DiscountCurve : public YieldCurve {
       public:
        DiscountCurve(const std::vector<Date>& dates, const std::vector<double>& dfs, const DayCounter& dayCounter = Actual360(),
                      const Interpolator& interpolator = Interpolator())
        : YieldCurve(dayCounter) {
            if (dates.size() != dfs.size()) throw std::runtime_error("dates and dfs size do not match");

            std::vector<std::pair<Date, double>> nodes;
            for (size_t i = 0; i < dates.size(); i++) nodes.push_back({dates[i], dfs[i]});

            std::sort(nodes.begin(), nodes.end());
            std::vector<Date> nodeDates;
            std::vector<double> nodeDFs;
            for (const auto& node : nodes) {
                nodeDates.push_back(node.first);
                nodeDFs.push_back(node.second);
            }
            refDate_ = nodeDates[0];
            for (const Date& date : nodeDates) times_.push_back(dayCounter.yearFraction(refDate_, date));

            dfs_      = nodeDFs;
            interpol_ = interpolator.interpolate(times_.begin(), times_.end(), dfs_.begin());
            interpol_.enableExtrapolation();
        }

        DiscountCurve(const Date& refDate, const std::vector<double>& times, const std::vector<double>& dfs,
                      const DayCounter& dayCounter = Actual360(), const Interpolator& interpolator = Interpolator())
        : YieldCurve(refDate, dayCounter) {
            if (times.size() != dfs.size()) throw std::runtime_error("times and dfs size do not match");

            std::vector<std::pair<double, double>> nodes;
            for (size_t i = 0; i < times.size(); i++) nodes.push_back({times[i], dfs[i]});

            std::sort(nodes.begin(), nodes.end());

            for (const auto node : nodes) {
                times_.push_back(node.first);
                dfs_.push_back(node.second);
            }
            interpol_ = interpolator.interpolate(times_.begin(), times_.end(), dfs_.begin());
        }

        double df(double time) const override {
            if (time == 0) return 1;
            return interpol_(time);
        }

        double df(const Date& date) const override {
            double t = dayCounter_.yearFraction(refDate_, date);
            return df(t);
        }

       private:
        std::vector<double> times_;
        std::vector<double> dfs_;
        Interpolation interpol_;
    };
}  // namespace atlas

#endif /* D47BBB89_822D_4EF2_A7B1_3C262C68105A */
