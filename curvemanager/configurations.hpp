#pragma once

#include "qlconfig.hpp"

namespace CurveManager {
	/*
	* Configuration structs for each ratehelper
	* attributes should be pass by value or pointer to a new value
	*/
	
	static struct Configuration {
		
	};
	
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
			OIS,
			NONE,
		};
		const Date refDate;
		ConfigType type;
		ConfigSubType subType;
		std::shared_ptr<Configuration> subConfig;
	};

	struct CurveConfig : Configuration {
		const std::string& name;
		const DayCounter dayCounter = Act360();
	};

	struct DepositConfig : Configuration {
		double rate;
		const Period tenor;
		double fixingDays = 0;
		const Calendar calendar = NullCalendar();
		BusinessDayConvention convention = BusinessDayConvention::Unadjusted;
		bool endOfMonth = false;
		const DayCounter dayCounter = Act360();		
	};
	
	struct FixedBondConfig : Configuration {
		const Handle<Quote>& price;
		const double settlementDays = 1;
		const double faceAmount = 100;
		const Schedule& schedule;
		const std::vector<double>& coupons;
		const DayCounter& dayCounter;
		BusinessDayConvention paymentConv = BusinessDayConvention::Following;
		double redemption = 100.0;
		const Date& issueDate = Date();
		const Calendar& paymentCalendar = Calendar();
		const Period& exCouponPeriod = Period();
		const Calendar& exCouponCalendar = Calendar();
		BusinessDayConvention exCouponConvention = BusinessDayConvention::Unadjusted;
		bool exCouponEndOfMonth = false;
		//Bond::Price::Type priceType = Bond::Price::Clean;
	};
	
	struct SwapConfig : Configuration {
		const double rate;
		const boost::shared_ptr<SwapIndex>& swapIndex;
		Handle<Quote> spread = Handle<Quote>();
		const Period& fwdStart = 0 * TimeUnit::Days;
		Handle<YieldTermStructure> discountingCurve = Handle<YieldTermStructure>();
	};

	struct FxSwapConfig : Configuration {
		const Handle<Quote>& fwdPoint;
		Handle<Quote> spotFx;
		const Period& tenor;
		const double fixingDays;
		const Calendar& calendar;
		BusinessDayConvention convention;
		bool endOfMonth;
		bool isFxBaseCurrencyCollateralCurrency;
		Handle<YieldTermStructure> collateralCurve;
		Calendar tradingCalendar = Calendar();
	};
	
	struct OISRateConfig {
		const double settlementDays;
		const Period& tenor; // swap maturity
		const Handle<Quote>& fixedRate;
		boost::shared_ptr<OvernightIndex> overnightIndex;
		// exogenous discounting curve
		Handle<YieldTermStructure> discountingCurve = Handle<YieldTermStructure>();
		bool telescopicValueDates = false;
		const double paymentLag = 0;
		BusinessDayConvention paymentConvention = BusinessDayConvention::Following;
		Frequency paymentFrequency = Frequency::Annual;
		Calendar paymentCalendar = Calendar();
		const Period& forwardStart = 0 * TimeUnit::Days;
		const double overnightSpread = 0.0;
	};
	
	struct XCCYRateConfig {
		const Handle<Quote>& basis;
		const Period& tenor;
		const double fixingDays;
		const Calendar& calendar;
		BusinessDayConvention convention;
		bool endOfMonth;
		const boost::shared_ptr<IborIndex>& baseCurrencyIndex;
		const boost::shared_ptr<IborIndex>& quoteCurrencyIndex;
		const Handle<YieldTermStructure>& collateralCurve;
		bool isFxBaseCurrencyCollateralCurrency;
		bool isBasisOnFxBaseCurrencyLeg;
	};	

	struct IborIborSwapConfig {
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
}
	