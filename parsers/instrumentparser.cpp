#include "instrumentparser.h"
#include <ql/instruments/bonds/fixedratebond.hpp>
#include <ql/instruments/bond.hpp>
#include <ql/cashflows/cashflowvectors.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>

namespace parsers {
	
	InstrumentParser::InstrumentParser() :JSONParser() {};
	InstrumentParser::InstrumentParser(const json& data) : JSONParser(data) {
		initInstrumentMap();
	};
	void InstrumentParser::initInstrumentMap() {
		instrumentMap_.insert({ "bond", Instrument::FixedRateBond });
		instrumentMap_.insert({ "loan", Instrument::FixedRateLoan });
		instrumentMap_.insert({ "equalPaymentLoan", Instrument::EqualPaymentLoan });
		instrumentMap_.insert({ "swap", Instrument::Swap });
	}
	ext::shared_ptr<QL::Instrument> InstrumentParser::parseInstrument(const json& data) {
		switch (instrumentMap_[data["type"]])
		{
		case Instrument::FixedRateBond:
			instrument_ = parseFixedRateBond(data);
		default:
			break;
		}
		return instrument_;
	};
	
	ext::shared_ptr<QL::Instrument> InstrumentParser::parseFixedRateBond(const json& data) {
		json params = data["parameters"];
		
		QL::Date startDate = parseDate(params["startDate"]);
		QL::Date endDate = parseDate(params["endDate"]);		
		QL::Natural settlementDays = params["settlementDays"];

		QL::Calendar calendar = parseCalendar(params["calendar"]);
		QL::BusinessDayConvention convention = parseDBConvention(params["dbConvention"]);
		QL::Frequency paymentFrequency = parseFrequency(params["paymentFrequency"]);
		QL::Schedule schedule(startDate, endDate, QL::Period(paymentFrequency), calendar, convention, convention, QL::DateGeneration::Backward, false);

		json couponData = params["coupon"];
		QL::DayCounter couponDayCounter = parseDayCounter(couponData["dayCounter"]);
		QL::Frequency couponFrequency = parseFrequency((std::string)couponData["frequency"]);
		QL::Compounding couponCompounding = parseCompounding((std::string)couponData["compounding"]);
		QL::InterestRate r(couponData["rate"], couponDayCounter, couponCompounding, couponFrequency);
		std::vector<QL::InterestRate> vr(1, r);

		if (!params["buildByCoupon"])
		{						
			QL::Real faceAmount = params["faceAmount"];						
			return ext::shared_ptr<QL::Instrument>(new QL::FixedRateBond(settlementDays, faceAmount, schedule, vr));
		}
		else
		{
			QL::Leg leg = parseFixedRateCouponLeg(data["coupons"], r);
			return ext::shared_ptr<QL::Instrument>(new QL::Bond(settlementDays, calendar, startDate, leg));
		}
	}
	QL::Leg InstrumentParser::parseFixedRateCouponLeg(const json& coupons, const QL::InterestRate& r) {
		QL::Leg leg;
		for (auto coupon: coupons)
		{
			QL::Date startDate = parseDate(coupon["startDate"]);
			QL::Date endDate = parseDate(coupon["endDate"]);
			QL::Real amount = coupon["amount"];
			leg.push_back(ext::shared_ptr<QL::CashFlow> (new QL::FixedRateCoupon(endDate, amount, r, startDate, endDate)));
		}
		return leg;
	}
}