#pragma once

#include "parser.h"
#include <ql/interestrate.hpp>
#include <ql/instrument.hpp>
#include <ql/cashflows/fixedratecoupon.hpp>

namespace parsers {
	class InstrumentParser : public JSONParser {
	public:
		InstrumentParser();
		InstrumentParser(const json& data);
		ext::shared_ptr<QL::Instrument> parseInstrument(const json& data);
	private:
		ext::shared_ptr<QL::Instrument> parseFixedRateBond(const json& data);
		//ext::shared_ptr<QL::Instrument> parseFixedRateLoan(json& data);
		//ext::shared_ptr<QL::Instrument> parseEqualPaymentLoan(json& data);
		//ext::shared_ptr<QL::Instrument> parseSwap(json& data);
		
		QL::Leg parseFixedRateCouponLeg(const json& data, const QL::InterestRate& r);

		void initInstrumentMap();
		const enum Instrument {FixedRateBond, FloatingRateBond, FixedRateLoan, FloatingRateLoan, EqualPaymentLoan, Swap};
		std::map<std::string, Instrument> instrumentMap_;
		ext::shared_ptr<QL::Instrument> instrument_;
	};
}


