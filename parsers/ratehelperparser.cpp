#include "ratehelperparser.h"
#include <ql/cashflows/cashflows.hpp>

namespace parsers {
	RateHelperParser::RateHelperParser() : JSONParser() {
		initRateHelperTypeMap();
	}
	RateHelperParser::RateHelperParser(const json& data) : JSONParser(data) {
		initRateHelperTypeMap();
	}

	ext::shared_ptr<QL::RateHelper> RateHelperParser::parseRateHelper(const json& obj) {
		switch (rateHelperTypeMap_[obj["type"]])
		{
		case RateHelperType::FixedRateBond:
			return parseBondRateHelper(obj["parameters"]);;
		case RateHelperType::Deposit:
			return parseDepositRateHelper(obj["parameters"]);
		default:
			break;
		}
	}

	ext::shared_ptr<QL::DepositRateHelper> RateHelperParser::parseDepositRateHelper(const json& params) {
		QL::Natural n = params["timeunit"][0];
		QL::TimeUnit timeUnit = parseTimeUnit(params["timeUnit"][1]);
		QL::Natural settlementDays = params["settlementDays"];
		QL::Calendar calendar = parseCalendar(params["calendar"]);
		QL::BusinessDayConvention convention = parseDBConvention(params["bdConvention"]);

		//IRR
		QL::DayCounter irrDayCounter = parseDayCounter(params["IRR"]["dayCounter"]);
		QL::Compounding irrCompounding = parseCompounding(params["IRR"]["compounding"]);
		QL::Frequency  irrFrequency = parseFrequency(params["IRR"]["frequency"]);
		QL::Rate irr(params["IRR"]["rate"]);
		return std::move(ext::shared_ptr<QL::DepositRateHelper>(new QL::DepositRateHelper(irr, QL::Period(n, timeUnit), settlementDays, calendar, convention, false, irrDayCounter)));
	}

	ext::shared_ptr<QL::BondHelper> RateHelperParser::parseBondRateHelper(const json& params) {
		QL::Date startDate = parseDate(params["startDate"]);
		QL::Date endDate = parseDate(params["endDate"]);

		//coupon rate
		QL::DayCounter couponDayCounter = parseDayCounter(params["coupon"]["dayCounter"]);
		QL::Frequency  couponFrequency = parseFrequency(params["coupon"]["frequency"]);
		QL::Compounding couponCompounding = parseCompounding(params["coupon"]["compounding"]);
		QL::InterestRate coupon(params["coupon"]["rate"], couponDayCounter, couponCompounding, couponFrequency);
		std::vector<QL::InterestRate> couponVector(1, coupon);

		//IRR
		QL::DayCounter irrDayCounter = parseDayCounter(params["IRR"]["dayCounter"]);
		QL::Compounding irrCompounding = parseCompounding(params["IRR"]["compounding"]);
		QL::Frequency  irrFrequency = parseFrequency(params["IRR"]["frequency"]);
		QL::InterestRate irr(params["IRR"]["rate"], irrDayCounter, irrCompounding, irrFrequency);
		
		
		QL::Natural settlementDays = params["settlementDays"];	
		QL::Real faceAmount = params["faceAmount"];
		QL::Calendar calendar = parseCalendar(params["calendar"]);
		QL::BusinessDayConvention convention = parseDBConvention(params["dbConvention"]);
		QL::Frequency frequency = parseFrequency(params["paymentFrequency"]);
		QL::Schedule schedule(startDate, endDate, QL::Period(frequency), calendar, convention, convention, QL::DateGeneration::Backward, false);

		ext::shared_ptr<QL::Bond> bondPtr (new QL::FixedRateBond(settlementDays, faceAmount, schedule, couponVector, convention, 100, startDate));

		QL::Real npv = QL::CashFlows::npv(bondPtr->cashflows(), irr, true);
		QL::Handle<QL::Quote> price(ext::shared_ptr<QL::Quote>(new QL::SimpleQuote(npv)));	
		return std::move(ext::shared_ptr<QL::BondHelper>(new QL::BondHelper(price, bondPtr)));
	}

	/*
	ext::shared_ptr<QL::SwapRateHelper> RateHelperParser::parseSwapRateHelper(const json& params) {
		QL::Date startDate = parseDate(params["StartDate"]);
		QL::Date endDate = parseDate(params["EndDate"]);

		//coupon rate
		QL::DayCounter couponDayCounter = parseDayCounter(params["coupon"]["dayCounter"]);
		QL::Frequency  couponFrequency = parseFrequency(params["coupon"]["Frequency"]);
		QL::Compounding couponCompounding = parseCompounding(params["coupon"]["Compounding"]);
		QL::Rate coupon(params["coupon"]["rate"]);

		QL::Natural settlementDays = params["settlementDays"];
		QL::Real faceAmount = params["FaceAmount"];
		QL::Calendar calendar = parseCalendar(params["Calendar"]);
		QL::BusinessDayConvention convention = parseDBConvention(params["BDConvention"]);
		QL::Frequency frequency = parseFrequency(params["PaymentFrequency"]);
		QL::Schedule schedule(startDate, endDate, QL::Period(frequency), calendar, convention, convention, QL::DateGeneration::Backward, false);
		

		QL::Handle<QL::Quote> price(ext::shared_ptr<QL::Quote>(new QL::SimpleQuote(npv)));
		return std::move(ext::shared_ptr<QL::SwapRateHelper>(new QL::SwapRateHelper(rate, ));
	}
	*/
	void RateHelperParser::initRateHelperTypeMap() {
		rateHelperTypeMap_.insert({ "fixedRateBond", RateHelperType::FixedRateBond });
		rateHelperTypeMap_.insert({ "deposit", RateHelperType::Deposit });
		rateHelperTypeMap_.insert({ "swap", RateHelperType::Swap });
		rateHelperTypeMap_.insert({ "OIS", RateHelperType::OIS });
	}
}