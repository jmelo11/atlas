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

        RateIndex(const std::string& name, QuantLib::DayCounter dayCounter,
                  QuantLib::Frequency fixingFreq,
                  QuantLib::Frequency rateFreq   = QuantLib::Frequency::Annual,
                  QuantLib::Compounding rateComp = QuantLib::Compounding::Simple)
        : name_(name), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateComp_(rateComp){};

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

    class ICP : public RateIndex {
       public:
        ICP() : RateIndex("ICP", QuantLib::Actual360(), QuantLib::Frequency::Semiannual) {}
    };

}  // namespace Atlas

#endif /* A53B011D_D929_446B_92A0_CDFF70167D4D */
