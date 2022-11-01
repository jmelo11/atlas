/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#include <atlas/curves/curve.hpp>

namespace Atlas {

    YieldCurve::YieldCurve(const Date& refDate, const DayCounter& dayCounter) : refDate_(refDate), dayCounter_(dayCounter) {}

    double YieldCurve::forwardDf(double t1, double t2) {
        if (t1 > t2) throw std::runtime_error("t2 must be less than t1");
        return df(t2) / df(t1);
    }

    double YieldCurve::forwardDf(const Date& d1, const Date& d2) {
        if (d1 > d2) throw std::runtime_error("d2 must be less than d1");
        return df(d2) / df(d1);
    }

    double YieldCurve::forwardRate(double t1, double t2, Compounding comp, Frequency freq) const {
        double df1 = df(t1);
        double df2 = df(t2);
        return InterestRate::impliedRate(df1 / df2, dayCounter_, comp, freq, t2 - t1).rate();
    }
    double YieldCurve::forwardRate(const Date& d1, const Date& d2, Compounding comp, Frequency freq) const {
        double t1 = dayCounter_.yearFraction(refDate_, d1);
        double t2 = dayCounter_.yearFraction(refDate_, d2);
        return forwardRate(t1, t2, comp, freq);
    }

}  // namespace atlas