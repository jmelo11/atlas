#pragma once

#include <others/qlconfig.hpp>

namespace atlas {
	
    class RateIndex {
        public:
            RateIndex() {};
		    RateIndex(const std::string& name, const DayCounter& dayCounter, Frequency fixingFreq, const Currency& currency, Frequency rateFreq = Frequency::Annual, Compounding rateComp = Compounding::Simple):
			    name_(name), dayCounter_(dayCounter), fixingFreq_(fixingFreq), rateComp_(rateComp), currency_(currency) {};
            std::string name_ = "undefined";
		    DayCounter dayCounter_ = Act360();
		    Frequency fixingFreq_ = Frequency::Semiannual;
			Frequency rateFreq_ = Frequency::Annual;
		    Compounding rateComp_ = Compounding::Simple;
		    Currency currency_ = USDCurrency();
		    //fixings?
    };

    class LIBOR3M : public RateIndex {
    public:
		LIBOR3M():
            RateIndex( "LIBOR3M", Act360(), Frequency::Quarterly, USDCurrency() )
		{}
    };
    class LIBOR1M : public RateIndex {
    public:
        LIBOR1M() :
            RateIndex( "LIBOR1M", Act360(), Frequency::Monthly, USDCurrency() )
        {}
    };
    class LIBOR6M : public RateIndex {
    public:
        LIBOR6M() :
            RateIndex( "LIBOR6M", Act360(), Frequency::Semiannual , USDCurrency() )
        {}
    };

    class LIBOR12M : public RateIndex {
    public:
        LIBOR12M() :
            RateIndex( "LIBOR12M", Act360(), Frequency::Annual , USDCurrency() )
        {}
    };
	
    class ICP : public RateIndex {
    public:
        ICP() :
            RateIndex( "ICP", Act360(), Frequency::Semiannual, CLPCurrency())
        {}
    };

    /* ADD TABS */
}