#include "curvemanager.hpp"
#include "..\parser\parser.h"
#include "..\database\database.hpp"

#include <boost/algorithm/string.hpp>
#include <ql/instruments/bonds/fixedratebond.hpp>

#include <stdexcept>

namespace CurveManager {
	
	void CurveBuilder::build(boost::shared_ptr<BaseConfiguration> config) {
		auto& [refDate, type, subType, subConfig] = *config;
		if (type == BaseConfiguration::ConfigType::CURVE)
		{
			/* set system date */
			Date currentDate = Settings::instance().evaluationDate();
			Settings::instance().evaluationDate() = refDate;

			boost::shared_ptr<CurveConfig> castedConfig = boost::static_pointer_cast<CurveConfig>(subConfig);
			auto& [curve_id, name, dayCounter, enableExtrapolaiton, unregisterFromEvalDate] = *castedConfig;

			/*build helpers*/
			auto rateHelperConfigs = makeHelperConfigs(refDate, curve_id);
			std::vector<boost::shared_ptr<RateHelper>> helpers;
			for (auto& rhc : rateHelperConfigs) {
				boost::shared_ptr<RateHelper> tmp = buildHelper(rhc);
				helpers.push_back(tmp);
			}

			/* define curve and add to store*/
			PiecewiseYieldCurve<ForwardRate, Linear>curve();
			boost::shared_ptr<YieldTermStructure> curvePtr = boost::make_shared<PiecewiseYieldCurve<ForwardRate, Linear>>(refDate, helpers, dayCounter);
			if (enableExtrapolaiton)
				curvePtr->enableExtrapolation();
			
			/* prevent curve from changing with settings evaldate */
			if (unregisterFromEvalDate)
				curvePtr->unregisterWith(Settings::instance().evaluationDate());
			
			Settings::instance().evaluationDate() = currentDate;
			curveStore_.addCurve(refDate, name, curvePtr);
		}
		else {			
			throw std::runtime_error("type is not curve type");
		}
	};

	/* makes QL ratehelpers */
	boost::shared_ptr<RateHelper> CurveBuilder::buildHelper(boost::shared_ptr<BaseConfiguration> config) {
		auto& [refDate, type, subType, subConfig] = *config;
		if (type == BaseConfiguration::ConfigType::RATEHELPER)
		{
			switch (subType)
			{
			case CurveManager::BaseConfiguration::DEPOSIT:{
				boost::shared_ptr<DepositConfig> castedConfig = boost::static_pointer_cast<DepositConfig>(subConfig);
				auto& [rate, tenor, fixingDays, calendar, convention, endOfMonth, dayCounter] = *castedConfig;
				return boost::make_shared<DepositRateHelper>(rate, tenor, fixingDays, calendar, convention, endOfMonth, dayCounter);
			}
			case CurveManager::BaseConfiguration::BOND: {
				boost::shared_ptr<FixedBondConfig> castedConfig = boost::static_pointer_cast<FixedBondConfig>(subConfig);
				
				auto& [price,
					settlementDays,
					faceAmount,
					schedule,
					coupons,
					dayCounter,
					paymentConv,
					redemption,
					issueDate,
					paymentCalendar,
					exCouponPeriod,
					exCouponCalendar,
					exCouponConvention,
					exCouponEndOfMonth] = *castedConfig;
				
				return boost::make_shared<FixedRateBondHelper>(price,
					settlementDays,
					faceAmount,
					schedule,
					coupons,
					dayCounter,
					paymentConv,
					redemption,
					issueDate,
					paymentCalendar,
					exCouponPeriod,
					exCouponCalendar,
					exCouponConvention,
					exCouponEndOfMonth);
			}
			case CurveManager::BaseConfiguration::SWAP:{
				boost::shared_ptr<SwapConfig> castedConfig = boost::static_pointer_cast<SwapConfig>(subConfig);
				auto& [rate, tenor,calendar,fixedFrequency, fixedConvention,fixedDayCount,iborIndex,spread,fwdStart,discountingCurve,settlementDays] = *castedConfig;
				return boost::make_shared<SwapRateHelper>(rate, tenor, calendar, fixedFrequency, fixedConvention, fixedDayCount, iborIndex, spread, fwdStart, discountingCurve, settlementDays);
			}
			case CurveManager::BaseConfiguration::FXSWAP:{
				boost::shared_ptr<FxSwapConfig> castedConfig = boost::static_pointer_cast<FxSwapConfig>(subConfig);
				auto& [fwdPoint, spotFx, tenor, fixingDays, calendar, convention, endOfMonth, isFxBaseCurrencyCollateralCurrency, collateralCurve, tradingCalendar] = *castedConfig;
				return boost::make_shared<FxSwapRateHelper>(fwdPoint, spotFx, tenor, fixingDays, calendar, convention, endOfMonth, isFxBaseCurrencyCollateralCurrency, collateralCurve, tradingCalendar);
			}
			case CurveManager::BaseConfiguration::OIS: {
				boost::shared_ptr<OISRateConfig> castedConfig = boost::static_pointer_cast<OISRateConfig>(subConfig);
				auto& [settlementDays,
					tenor,
					fixedRate,
					overnightIndex,
					discountingCurve,
					telescopicValueDates,
					paymentLag,
					paymentConvention,
					paymentFrequency,
					paymentCalendar,
					forwardStart,
					overnightSpread] = *castedConfig;
				return boost::make_shared<OISRateHelper>(settlementDays,
					tenor,
					fixedRate,
					overnightIndex,
					discountingCurve,
					telescopicValueDates,
					paymentLag,
					paymentConvention,
					paymentFrequency,
					paymentCalendar,
					forwardStart,
					overnightSpread);
			}
			case CurveManager::BaseConfiguration::XCCY: {
				boost::shared_ptr<XCCYRateConfig> castedConfig = boost::static_pointer_cast<XCCYRateConfig>(subConfig);
				auto& [rate,spotFx,
				settlementDays,paymentCalendar,paymentConvention,tenor,fixedCurrency,fixedFrequency,
				fixedConvention,fixedDayCount ,index,floatDiscount,
				spread ,endOfMonth ] = *castedConfig;
				return boost::make_shared<CrossCcyFixFloatSwapHelper>(rate, spotFx,
					settlementDays, paymentCalendar, paymentConvention, tenor, fixedCurrency, fixedFrequency,
					fixedConvention, fixedDayCount, index, floatDiscount,
					spread, endOfMonth);
			}
			case CurveManager::BaseConfiguration::NONE:
				break;
			default:
				break;
			}
		}
		else {
			throw std::runtime_error("type is not ratehelper");
		}
	}

	/* makes configs */
	std::vector<boost::shared_ptr<BaseConfiguration>> CurveBuilder::makeHelperConfigs(const Date& refDate, int curve_id) {
		
		std::vector<boost::shared_ptr<BaseConfiguration>> configurations;
		
		/* get all the helpers ids */
		pqxx::result result = db_.executePrepared("helpers", curve_id);
		
		for (const auto& row: result)
		{
			int helperId = row[0].as<int>();
			std::string type = row[1].as<std::string>();
						
			boost::shared_ptr<BaseConfiguration> helperConfig = makeHelperConfigFromDB(helperId, type, refDate);			
			configurations.push_back(helperConfig);
		}
		return configurations;
	};
	
	/* makes config from database fields */
	boost::shared_ptr<BaseConfiguration> CurveBuilder::makeHelperConfigFromDB(int helperId, const std::string& type, const Date& refDate) {
		/* get fields */
		pqxx::result helperConfig = db_.executePrepared("helper_configs", helperId);
		std::unordered_map<std::string, std::any> parameters;
		/* parse fields */
		for (const auto& row : helperConfig) {
			std::string field = row[0].as<std::string>();
			std::string value = row[1].as<std::string>();
			boost::to_upper(field);
			parameters[field] = parseStringFields(field, value, refDate);
		}


		if (type == "DEPOSIT") {
			DepositConfig depoConfig{ .rate = std::any_cast<double>(parameters["RATE"]),
									.tenor = std::any_cast<Period>(parameters["TENOR"]),
									.dayCounter = std::any_cast<DayCounter>(parameters["DAYCOUNTER"]) };

			BaseConfiguration config{ .refDate = refDate,
										.type = BaseConfiguration::ConfigType::RATEHELPER,
										.subType = BaseConfiguration::ConfigSubType::DEPOSIT,
										.subConfig = boost::make_shared<DepositConfig>(depoConfig) };
			return boost::make_shared<BaseConfiguration>(config);
		}
		else if (type == "BOND")
		{
			/* we need the bond price :( */
			int settlementDays = (parameters.find("SETTLEMENTDAYS") == parameters.end()) ? settlementDays = 1 : std::any_cast<int>(parameters["SETTLEMENTDAYS"]);
			Period tenor = std::any_cast<Period>(parameters["TENOR"]);

			/* coupon rate */
			double couponRate = std::any_cast<double>(parameters["COUPON"]);
			DayCounter couponDayCounter = std::any_cast<DayCounter>(parameters["COUPONDAYCOUNTER"]);
			InterestRate coupon(couponRate, couponDayCounter, Compounding::Simple, Frequency::Annual);

			/* irr */
			double rate = std::any_cast<double>(parameters["RATE"]);
			DayCounter irrDayCounter = std::any_cast<DayCounter>(parameters["IRDAYCOUNTER"]);

			/* bond params */
			Calendar calendar = std::any_cast<Calendar>(parameters["CALENDAR"]);
			Frequency frequency = std::any_cast<Frequency>(parameters["FREQUENCY"]);
			BusinessDayConvention convention = std::any_cast<BusinessDayConvention>(parameters["CONVENTION"]);

			Date startDate = (parameters.find("STARTDATE") == parameters.end()) ? Settings::instance().evaluationDate() : std::any_cast<Date>(parameters["STARTDATE"]);
			Date endDate = (parameters.find("ENDDATE") == parameters.end()) ? startDate + tenor : std::any_cast<Date>(parameters["ENDDATE"]);

			Schedule schedule = makeSchedule().from(startDate)
				.to(endDate)
				.withTenor(tenor)
				.withFrequency(frequency)
				.withCalendar(calendar)
				.withConvention(convention);



			FixedRateBond bond(settlementDays, 100, schedule, std::vector<InterestRate>{coupon});
			boost::shared_ptr<SimpleQuote> cleanPrice(boost::make_shared<SimpleQuote>(bond.cleanPrice(rate, irrDayCounter, Compounding::Compounded, Frequency::Annual)));
			Handle<Quote> handlePrice(cleanPrice);
			FixedBondConfig bondConfig{ .price = handlePrice,
										.settlementDays = settlementDays,
										.faceAmount = 100,
										.schedule = schedule,
										.coupons = std::vector<double>{couponRate},
										.dayCounter = couponDayCounter,
										.paymentConv = convention,
										.paymentCalendar = calendar };



			BaseConfiguration config{ .refDate = refDate,
										.type = BaseConfiguration::ConfigType::RATEHELPER,
										.subType = BaseConfiguration::ConfigSubType::BOND,
										.subConfig = boost::make_shared<FixedBondConfig>(bondConfig) };
			return boost::make_shared<BaseConfiguration>(config);
		}
		else {		
			throw std::runtime_error("Unknown helper type: " + type);
		}						
	}
	
	/* parses string fields */
	std::any CurveBuilder::parseStringFields(const std::string& field, const std::string& value, const Date& refDate)
	{				
		if (field == "RATE" || field == "SPOTFX" || field == "FWDPOINT" || field == "SPREAD") {
			/* check if ticker or value */
			std::string strDate = parser::parseDate(refDate, parser::DateFormat::MIXED);
			pqxx::result r = db_.executePrepared("prices", value, strDate);
			double price = r.front()[0].as<double>();
			return price;
								
		}
		else if (field == "TENOR") {
			return parser::parsePeriod(value);
		}
		else if (field == "SETTLEMENTDAYS" || field == "PAYMENTLAG" || field == "FWDSTART" || field == "FIXINGDAYS") {
			return std::stoi(value);
		}
		else if (field == "FREQUENCY") {
			return parser::parseFrequency(value);
		}
		else if (field == "CONVENTION") {
			return parser::parseDBConvention(value);
		}
		else if (field == "CALENDAR") {
			return parser::parseCalendar(value);
		}
		else if (field == "ENDOFMONTH") {
			return std::stoi(value);
		}
		else if (field == "COUPON") {
			return std::stod(value);
		}
		else if (field == "IRDAYCOUNTER" || field == "COUPONDAYCOUNTER" || field == "DAYCOUNTER") {
			return parser::parseDayCounter(value);
		}
		else if (field == "DISCOUNTINGCURVE") {
			boost::shared_ptr<YieldTermStructure> curve = get(refDate, value);
			Handle<YieldTermStructure>curveHandle(curve);
			return curveHandle;
		}
		else {
			//std::string error = "field " + field + " with value " + value + "not supported";
			throw std::runtime_error("error given field field: " + field);
		}
	}

	boost::shared_ptr<BaseConfiguration> CurveBuilder::makeCurveConfig(const Date& refDate, const std::string& name) {
		
		/* gets latest curve config */
		pqxx::result configDate = db_.executePrepared("latest_curve_config", name);
		std::string lastestDate = configDate.front()[0].as<std::string>();

		pqxx::result configResults = db_.executePrepared("curves", lastestDate, name);
		
		int curve_id = configResults.front()[0].as<int>();
		std::string dayCounterStr = configResults.front()[1].as<std::string>();
		
		/*parsing*/
		std::string strDate = parser::parseDate(refDate, parser::DateFormat::MIXED);
		DayCounter dayCounter = parser::parseDayCounter(dayCounterStr);
		
		/* config */
		CurveConfig curveConfig{ .curve_id = curve_id, .name = name, .dayCounter = dayCounter };
		BaseConfiguration config{ .refDate = refDate,
			.type = BaseConfiguration::ConfigType::CURVE,
			.subType = BaseConfiguration::ConfigSubType::NONE,
			.subConfig = boost::make_shared<CurveConfig>(curveConfig) };
		return boost::make_shared<BaseConfiguration>(config);
	};
}