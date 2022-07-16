#include "ratehelperparser.h"

namespace parsers {
	class CurveParser : public RateHelperParser {
		typedef std::vector<ext::shared_ptr<QL::RateHelper>> HelperVector;
		typedef ext::shared_ptr<QL::YieldTermStructure> CurvePtr;

	public:
		
		CurveParser();
		CurveParser(const json& data);
		
		CurvePtr parsePiecewiseCurve(const json& curve);
		CurvePtr parsePureDiscountCurve(const json& curve);

	private:

		void initIntepolationTypeMap();
		void initUnderlyingDataTypeMap();
		
		HelperVector helpers_;
		enum Interpolation { Linear, Cubic, LogLinear };
		std::map<std::string, Interpolation> interpolationMap_;
		enum UnderlyingData { Discounts, ForwardRates};
		std::map<std::string, UnderlyingData> underlyingDataTypeMap_;
	};
}
