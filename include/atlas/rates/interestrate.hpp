#ifndef D55DBD41_ABBD_418C_9E65_3A7C969C955F
#define D55DBD41_ABBD_418C_9E65_3A7C969C955F

#include <ql/compounding.hpp>
#include <ql/time/calendar.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounter.hpp>
#include <ql/time/frequency.hpp>
#include <atlas/atlasdefine.hpp>
#include <cmath>
#include <type_traits>

namespace Atlas {

    template <typename adouble>
    class InterestRate {
        using DayCounter  = QuantLib::DayCounter;
        using Compounding = QuantLib::Compounding;
        using Frequency   = QuantLib::Frequency;
        using Date        = QuantLib::Date;

       public:
        InterestRate() = default;

        InterestRate(adouble r, DayCounter dc, Compounding comp = Compounding::Simple, Frequency freq = Frequency::Annual)
        : r_(r), dc_(std::move(dc)), comp_(comp), freqMakesSense_(false) {
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
                    return pow(1.0 + r_ / freq_, freq_ * t);
                case Compounding::Continuous:
                    return exp(r_ * t);
                case Compounding::SimpleThenCompounded:
                    if (t <= 1.0 / freq_) {
                        return 1.0 + r_ * t;
                    } else {
                        return pow(1.0 + r_ / freq_, freq_ * t);
                    }
                case Compounding::CompoundedThenSimple:
                    if (t > 1.0 / freq_) {
                        return 1.0 + r_ * t;
                    } else {
                        return pow(1.0 + r_ / freq_, freq_ * t);
                    }
                default:
                    QL_FAIL("unknown compounding convention");
            }
        };

        adouble compoundFactor(const Date& d1, const Date& d2, const Date& refStart = Date(), const Date& refEnd = Date()) const {
            double t = dc_.yearFraction(d1, d2, refStart, refEnd);
            return compoundFactor(t);
        }

        static InterestRate impliedRate(adouble compound, const DayCounter& resultDC, Compounding comp, Frequency freq, double t) {
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
                        r = (pow(compound, 1.0 / (double(freq) * t)) - 1.0) * double(freq);
                        break;
                    case Compounding::Continuous:
                        r = log(compound) / t;
                        break;
                    case Compounding::SimpleThenCompounded:
                        if (t <= 1.0 / double(freq))
                            r = (compound - 1.0) / t;
                        else
                            r = (pow(compound, 1.0 / (double(freq) * t)) - 1.0) * double(freq);
                        break;
                    case Compounding::CompoundedThenSimple:
                        if (t > 1.0 / double(freq))
                            r = (compound - 1.0) / t;
                        else
                            r = (pow(compound, 1.0 / (double(freq) * t)) - 1.0) * double(freq);
                        break;
                    default:
                        QL_FAIL("unknown compounding convention (" << int(comp) << ")");
                }
            }
            return InterestRate(r, resultDC, comp, freq);
        };

        static InterestRate impliedRate(adouble compound, const DayCounter& resultDC, Compounding comp, Frequency freq, const Date& d1,
                                        const Date& d2, const Date& refStart = Date(), const Date& refEnd = Date()) {
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

    template <typename adouble>
    inline adouble fastCompoundFactor(adouble r, const QuantLib::DayCounter& dc, QuantLib::Compounding comp, QuantLib::Frequency freq_,
                                      const QuantLib::Date& d1, const QuantLib::Date& d2) {
        double freq = double(freq_);
        double t    = dc.yearFraction(d1, d2);
        switch (comp) {
            case QuantLib::Compounding::Simple:
                return 1.0 + r * t;
            case QuantLib::Compounding::Compounded:
                return pow(1.0 + r / freq, freq * t);

            case QuantLib::Compounding::Continuous:
                return exp(r * t);

            case QuantLib::Compounding::SimpleThenCompounded:
                if (t <= 1.0 / freq) {
                    return 1.0 + r * t;
                } else {
                    return pow(1.0 + r / freq, freq * t);
                }
            case QuantLib::Compounding::CompoundedThenSimple:
                if (t > 1.0 / freq) {
                    return 1.0 + r * t;
                } else {
                    return pow(1.0 + r / freq, freq * t);
                }
            default:
                QL_FAIL("unknown compounding convention");
        };
    };
}  // namespace Atlas

#endif /* D55DBD41_ABBD_418C_9E65_3A7C969C955F */
