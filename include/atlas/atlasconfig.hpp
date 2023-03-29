#ifndef AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D
#define AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D

#include <ql/compounding.hpp>
#include <ql/interestrate.hpp>
#include <ql/termstructures/yield/discountcurve.hpp>
#include <ql/time/businessdayconvention.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/daycounters/actual365fixed.hpp>
#include <ql/time/daycounters/thirty360.hpp>
#include <ql/time/frequency.hpp>
#include <ql/time/period.hpp>
#include <ql/time/schedule.hpp>
#include <cmath>
#include <cppad/cppad.hpp>
#include <iostream>

#define USE_AUTODIFF_1

namespace Atlas {

    // QuantLib types, to be replaced with our own types

    using Date                  = QuantLib::Date;
    using DayCounter            = QuantLib::DayCounter;
    using Calendar              = QuantLib::Calendar;
    using Period                = QuantLib::Period;
    using Frequency             = QuantLib::Frequency;
    using BusinessDayConvention = QuantLib::BusinessDayConvention;
    using Compounding           = QuantLib::Compounding;
    using Schedule              = QuantLib::Schedule;
    using MakeSchedule          = QuantLib::MakeSchedule;
    using Month                 = QuantLib::Month;

    // QuantLib day counters, to be replaced with our own day counters

    using Actual360      = QuantLib::Actual360;
    using Actual365Fixed = QuantLib::Actual365Fixed;
    using Thirty360      = QuantLib::Thirty360;

    using DiscountCurve = QuantLib::DiscountCurve;

#ifdef USE_AUTODIFF_1

    using adouble        = CppAD::AD<double>;
    using QLInterestRate = QuantLib::InterestRate;

    class InterestRate {
       public:
        InterestRate();

        InterestRate(adouble r, DayCounter dc, Compounding comp, Frequency freq) : r_(r), dc_(std::move(dc)), comp_(comp), freqMakesSense_(false) {
            if (comp_ == QuantLib::Compounded || comp_ == QuantLib::SimpleThenCompounded || comp_ == QuantLib::CompoundedThenSimple) {
                freqMakesSense_ = true;
                QL_REQUIRE(freq != QuantLib::Once && freq != QuantLib::NoFrequency, "frequency not allowed for this interest rate");
                freq_ = double(freq);
            }
        };

        adouble rate() const { return r_; }

        inline const DayCounter& dayCounter() const { return dc_; }
        inline Compounding compounding() const { return comp_; }
        inline Frequency frequency() const { return freqMakesSense_ ? Frequency(int(freq_)) : Frequency::NoFrequency; }

        adouble discountFactor(double t) const { return 1.0 / compoundFactor(t); }

        //! discount factor implied by the rate compounded between two dates
        adouble discountFactor(const Date& d1, const Date& d2, const Date& refStart = Date(), const Date& refEnd = Date()) const {
            double t = dc_.yearFraction(d1, d2, refStart, refEnd);
            return discountFactor(t);
        }

        adouble compoundFactor(double t) const {
            switch (comp_) {
                case Compounding::Simple:
                    return 1.0 + r_ * t;
                case Compounding::Compounded:
                    return CppAD::pow(1.0 + r_ / freq_, freq_ * t);
                case Compounding::Continuous:
                    return CppAD::exp(r_ * t);
                case Compounding::SimpleThenCompounded:
                    if (t <= 1.0 / freq_)
                        return 1.0 + r_ * t;
                    else
                        return CppAD::pow(1.0 + r_ / freq_, freq_ * t);
                case Compounding::CompoundedThenSimple:
                    if (t > 1.0 / freq_)
                        return 1.0 + r_ * t;
                    else
                        return CppAD::pow(1.0 + r_ / freq_, freq_ * t);
                default:
                    QL_FAIL("unknown compounding convention");
            }
        };

        adouble compoundFactor(const Date& d1, const Date& d2, const Date& refStart = Date(), const Date& refEnd = Date()) const {
            double t = dc_.yearFraction(d1, d2, refStart, refEnd);
            return compoundFactor(t);
        }
        static InterestRate impliedRate(adouble compound, const DayCounter& resultDC, Compounding comp, Frequency freq, double t)  {
            QL_REQUIRE(compound > 0.0, "positive compound factor required");

            adouble r;
            if (compound == 1.0) {
                QL_REQUIRE(t >= 0.0, "non negative time (" << t << ") required");
                r = 0.0;
            } else {
                QL_REQUIRE(t > 0.0, "positive time (" << t << ") required");
                switch (comp) {
                    case Compounding::Simple:
                        r = (compound - 1.0) / t;
                        break;
                    case Compounding::Compounded:
                        r = (CppAD::pow(compound, 1.0 / (double(freq) * t)) - 1.0) * double(freq);
                        break;
                    case Compounding::Continuous:
                        r = CppAD::log(compound) / t;
                        break;
                    case Compounding::SimpleThenCompounded:
                        if (t <= 1.0 / double(freq))
                            r = (compound - 1.0) / t;
                        else
                            r = (CppAD::pow(compound, 1.0 / (double(freq) * t)) - 1.0) * double(freq);
                        break;
                    case Compounding::CompoundedThenSimple:
                        if (t > 1.0 / double(freq))
                            r = (compound - 1.0) / t;
                        else
                            r = (CppAD::pow(compound, 1.0 / (double(freq) * t)) - 1.0) * double(freq);
                        break;
                    default:
                        QL_FAIL("unknown compounding convention (" << int(comp) << ")");
                }
            }
            return InterestRate(r, resultDC, comp, freq);
        };

        static InterestRate impliedRate(adouble compound, const DayCounter& resultDC, Compounding comp, Frequency freq, const Date& d1,
                                        const Date& d2, const Date& refStart = Date(), const Date& refEnd = Date())  {
            QL_REQUIRE(d2 >= d1, "d1 (" << d1
                                        << ") "
                                           "later than d2 ("
                                        << d2 << ")");
            double t = resultDC.yearFraction(d1, d2, refStart, refEnd);
            return impliedRate(compound, resultDC, comp, freq, t);
        }

       private:
        adouble r_;
        DayCounter dc_;
        Compounding comp_;
        bool freqMakesSense_;
        double freq_;
    };

    /**
     * @brief operator<<
     *
     * @return std::ostream&
     */
    std::ostream& operator<<(std::ostream&, const InterestRate&);

#else

    using adouble      = double;
    using InterestRate = QuantLib::InterestRate;

    template <typename T>
    static T Value(T ad) {
        return ad;
    }

#endif
}  // namespace Atlas

#endif /* AC3D1435_E543_466D_8A3E_A4DD7B9D5E8D */
