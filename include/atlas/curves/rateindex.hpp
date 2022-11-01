/*
 * Created on Mon Oct 31 2022
 *
 * Author: Jose Melo
 */

#ifndef A53B011D_D929_446B_92A0_CDFF70167D4D
#define A53B011D_D929_446B_92A0_CDFF70167D4D

#include <ql/compounding.hpp>
#include <ql/currencies/america.hpp>
#include <ql/time/date.hpp>
#include <ql/time/daycounters/actual360.hpp>
#include <ql/time/frequency.hpp>
#include <string>

namespace Atlas {

    using namespace QuantLib;

    class RateIndex {
       public:
        RateIndex(){};
        RateIndex(const std::string& name, const DayCounter& dayCounter, Frequency fixingFreq, const Currency& currency,
                  Frequency rateFreq = Frequency::Annual, Compounding rateComp = Compounding::Simple)
        : name_(name), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateComp_(rateComp), currency_(currency){};
        std::string name_      = "undefined";
        DayCounter dayCounter_ = Actual360();
        Frequency fixingFreq_  = Frequency::Semiannual;
        Frequency rateFreq_    = Frequency::Annual;
        Compounding rateComp_  = Compounding::Simple;
        Currency currency_     = USDCurrency();
        // fixings?
    };

    class LIBOR3M : public RateIndex {
       public:
        LIBOR3M() : RateIndex("LIBOR3M", Actual360(), Frequency::Quarterly, USDCurrency()) {}
    };
    class LIBOR1M : public RateIndex {
       public:
        LIBOR1M() : RateIndex("LIBOR1M", Actual360(), Frequency::Monthly, USDCurrency()) {}
    };
    class LIBOR6M : public RateIndex {
       public:
        LIBOR6M() : RateIndex("LIBOR6M", Actual360(), Frequency::Semiannual, USDCurrency()) {}
    };

    class LIBOR12M : public RateIndex {
       public:
        LIBOR12M() : RateIndex("LIBOR12M", Actual360(), Frequency::Annual, USDCurrency()) {}
    };

    class ICP : public RateIndex {
       public:
        ICP() : RateIndex("ICP", Actual360(), Frequency::Semiannual, CLPCurrency()) {}
    };

}  // namespace atlas

#endif /* A53B011D_D929_446B_92A0_CDFF70167D4D */
