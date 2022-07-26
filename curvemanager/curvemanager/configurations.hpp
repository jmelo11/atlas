#pragma once

#include "qlconfig.hpp"

namespace CurveManager {
	/*
	* Configuration structs for each ratehelper
	* attributes should be pass by value or pointer to a new value
	*/
	
	struct Configuration {};
	
	struct BaseConfiguration: Configuration {
		enum ConfigType {
			CURVE,
			RATEHELPER,
			INDEX,
		};
		enum ConfigSubType {
			DEPOSIT,
			BOND,
			SWAP,
			FXSWAP,
			XCCY,
			IBORIBOR,
			OISIBOR,
			OIS,
			NONE,
		};
		const Date refDate;
		ConfigType type;
		ConfigSubType subType;
		boost::shared_ptr<Configuration> subConfig;
	};

	struct CurveConfig : Configuration {
		int curve_id;
		const std::string& name;
		const DayCounter dayCounter = Act360();
		bool enableExtrapolaiton = true;
		bool unregisterFromEvalDate = true;
	};

	struct DepositConfig : Configuration {
		double rate;
		const Period tenor;
		double fixingDays = 0;
		const Calendar calendar = Chile();
		BusinessDayConvention convention = BusinessDayConvention::Unadjusted;
		bool endOfMonth = false;
		const DayCounter dayCounter = Act360();		
	};
	
	struct FixedBondConfig : Configuration {
		const Handle<Quote> price;
		int settlementDays = 1;
		double faceAmount = 100;
		const Schedule schedule;
		const std::vector<double> coupons;
		const DayCounter dayCounter;
		BusinessDayConvention paymentConv = BusinessDayConvention::ModifiedFollowing;
		double redemption = 100.0;
		const Date issueDate = Date();
		const Calendar paymentCalendar = Chile();
		const Period exCouponPeriod = Period();
		const Calendar exCouponCalendar = Calendar();
		BusinessDayConvention exCouponConvention = BusinessDayConvention::Unadjusted;
		bool exCouponEndOfMonth = false;
		//Bond::Price::Type priceType = Bond::Price::Clean;
	};
	
	struct SwapConfig : Configuration {
		const Handle<Quote> rate;
		const Period& tenor;
		Calendar calendar = JoinCalendar(Chile(),UnitedStates());
		// fixed leg
		Frequency fixedFrequency = Frequency::Semiannual;
		BusinessDayConvention fixedConvention = BusinessDayConvention::ModifiedFollowing;
		DayCounter fixedDayCount = Act360();
		// floating leg
		const boost::shared_ptr<IborIndex>& iborIndex;
		Handle<Quote> spread = Handle<Quote>();
		const Period fwdStart = 0 * TimeUnit::Days;
		// exogenous discounting curve
		Handle<YieldTermStructure> discountingCurve = Handle<YieldTermStructure>();
		double settlementDays = 2;
	};

	struct FxSwapConfig : Configuration {
		const Handle<Quote>& fwdPoint;
		Handle<Quote> spotFx;
		const Period& tenor;
		const double fixingDays = 1;
		const Calendar calendar = JoinCalendar(Chile(), UnitedStates());
		BusinessDayConvention convention = BusinessDayConvention::ModifiedFollowing;
		bool endOfMonth = false;
		bool isFxBaseCurrencyCollateralCurrency;
		Handle<YieldTermStructure> collateralCurve;
		Calendar tradingCalendar = JoinCalendar(Chile(), UnitedStates());
	};
	
	struct OISRateConfig : Configuration {
		const double settlementDays;
		const Period& tenor; // swap maturity
		const Handle<Quote>& fixedRate;
		boost::shared_ptr<OvernightIndex> overnightIndex;
		// exogenous discounting curve
		Handle<YieldTermStructure> discountingCurve = Handle<YieldTermStructure>();
		bool telescopicValueDates = false;
		const double paymentLag = 0;
		BusinessDayConvention paymentConvention = BusinessDayConvention::ModifiedFollowing;
		Frequency paymentFrequency = Frequency::Annual;
		Calendar paymentCalendar = JoinCalendar(Chile(), UnitedStates());
		const Period& forwardStart = 0 * TimeUnit::Days;
		const double overnightSpread = 0.0;
	};
	
	struct XCCYRateConfig : Configuration {
		const Handle<Quote>& rate;
		const Handle<Quote>& spotFx;
		double settlementDays = 2;
		const Calendar paymentCalendar = JoinCalendar(Chile(),UnitedStates());
		BusinessDayConvention paymentConvention = BusinessDayConvention::ModifiedFollowing;
		const Period& tenor;
		const Currency& fixedCurrency;
		Frequency fixedFrequency = Frequency::Semiannual;
		BusinessDayConvention fixedConvention = BusinessDayConvention::ModifiedFollowing;;
		const DayCounter fixedDayCount = Act360();
		const boost::shared_ptr<IborIndex>& index;
		const Handle<YieldTermStructure>& floatDiscount;
		const Handle<Quote> spread = Handle<Quote>(); 
		bool endOfMonth = false;
	};	

	struct IborIborSwapConfig: Configuration {
		const double basis;
		const Period& tenor;
		const double settlementDays = 2;
		Calendar calendar;
		BusinessDayConvention convention;
		bool endOfMonth = false;
		const std::string& baseIndex;
		const std::string& otherInde;
		const std::string& discountHandle;
		bool bootstrapBaseCurve;
	};
	
	struct OvernightIborSwapConfig : Configuration {
		const Handle<Quote>& basis;
		const Period& tenor;
		double settlementDays = 2;
		Calendar calendar;
		BusinessDayConvention convention;
		bool endOfMonth;
		const boost::shared_ptr<OvernightIndex>& baseIndex;
		const boost::shared_ptr<IborIndex>& otherIndex;
		Handle<YieldTermStructure> discountHandle;
	};
}
	