/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef A53B011D_D929_446B_92A0_CDFF70167D4D
#define A53B011D_D929_446B_92A0_CDFF70167D4D

#include <ql/compounding.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/frequency.hpp>
#include <string>

namespace Atlas {

    class RateIndex {
       public:
        RateIndex(){};

        RateIndex(const std::string& name, QuantLib::DayCounter dayCounter, QuantLib::Frequency fixingFreq,
                  QuantLib::Frequency rateFreq = QuantLib::Frequency::Annual, QuantLib::Compounding rateComp = QuantLib::Compounding::Simple)
        : name_(name), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateComp_(rateComp){};

        const std::string& name() const { return name_; }

        QuantLib::DayCounter dayCounter() const { return dayCounter_; }

        QuantLib::Frequency rateFrequency() const { return rateFreq_; }

        QuantLib::Compounding rateCompounding() const { return rateComp_; }

        QuantLib::Frequency fixingFrequency() const { return fixingFreq_; }

       private:
        std::string name_                = "undefined";
        QuantLib::DayCounter dayCounter_ = QuantLib::DayCounter();
        QuantLib::Frequency fixingFreq_  = QuantLib::Frequency::NoFrequency;
        QuantLib::Frequency rateFreq_    = QuantLib::Frequency::NoFrequency;
        QuantLib::Compounding rateComp_  = QuantLib::Compounding::Simple;
    };

    class LIBOR3M : public RateIndex {
       public:
        LIBOR3M() : RateIndex("LIBOR3M", QuantLib::Actual360(), QuantLib::Frequency::Quarterly) {}
    };
    class LIBOR1M : public RateIndex {
       public:
        LIBOR1M() : RateIndex("LIBOR1M", QuantLib::Actual360(), QuantLib::Frequency::Monthly) {}
    };
    class LIBOR6M : public RateIndex {
       public:
        LIBOR6M() : RateIndex("LIBOR6M", QuantLib::Actual360(), QuantLib::Frequency::Semiannual) {}
    };

    class LIBOR12M : public RateIndex {
       public:
        LIBOR12M() : RateIndex("LIBOR12M", QuantLib::Actual360(), QuantLib::Frequency::Annual) {}
    };

    class ICP1M : public RateIndex {
       public:
        ICP1M() : RateIndex("ICP1M", QuantLib::Actual360(), QuantLib::Frequency::Monthly) {}
    };

    class ICP3M : public RateIndex {
       public:
        ICP3M() : RateIndex("ICP3M", QuantLib::Actual360(), QuantLib::Frequency::Quarterly) {}
    };

    class ICP6M : public RateIndex {
       public:
        ICP6M() : RateIndex("ICP6M", QuantLib::Actual360(), QuantLib::Frequency::Semiannual) {}
    };

    class ICP12M : public RateIndex {
       public:
        ICP12M() : RateIndex("ICP12M", QuantLib::Actual360(), QuantLib::Frequency::Annual) {}
    };

    class TERMSOFR1M : public RateIndex {
       public:
        TERMSOFR1M() : RateIndex("TERMSOFR1M", QuantLib::Actual360(), QuantLib::Frequency::Monthly) {}
    };

    class TERMSOFR3M : public RateIndex {
       public:
        TERMSOFR3M() : RateIndex("TERMSOFR3M", QuantLib::Actual360(), QuantLib::Frequency::Quarterly) {}
    };

    class TERMSOFR6M : public RateIndex {
       public:
        TERMSOFR6M() : RateIndex("TERMSOFR6M", QuantLib::Actual360(), QuantLib::Frequency::Semiannual) {}
    };

    class TERMSOFR12M : public RateIndex {
       public:
        TERMSOFR12M() : RateIndex("TERMSOFR12M", QuantLib::Actual360(), QuantLib::Frequency::Annual) {}
    };

}  // namespace Atlas

#endif /* A53B011D_D929_446B_92A0_CDFF70167D4D */
