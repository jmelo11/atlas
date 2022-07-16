#include "parser.h"
#include <ql/termstructures/yield/ratehelpers.hpp>
#include <ql/termstructures/yield/bondhelpers.hpp>

namespace parsers {
	class RateHelperParser : public JSONParser {
	public:
		RateHelperParser();
		RateHelperParser(const json& data);
		/*deberian ser shared_ptr*/

		ext::shared_ptr<QL::RateHelper> parseRateHelper(const json& obj);
		ext::shared_ptr<QL::DepositRateHelper> parseDepositRateHelper(const json& params);		
		ext::shared_ptr<QL::BondHelper> parseBondRateHelper(const json& params);

		//ext::shared_ptr<QL::SwapRateHelper> parseSwapRateHelper(const json& params);


	private:
		void initRateHelperTypeMap();
		enum RateHelperType { FixedRateBond, Deposit, Swap, OIS };
		std::map<std::string, RateHelperType> rateHelperTypeMap_;
	};
}