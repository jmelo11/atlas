#include "curveparser.h"
#include <typeinfo>
#include <ql/termstructures/yield/piecewiseyieldcurve.hpp>
#include <ql/termstructures/yield/discountcurve.hpp>

namespace parsers {
	typedef std::vector<ext::shared_ptr<QL::RateHelper>> HelperVector;
	typedef ext::shared_ptr<QL::YieldTermStructure> CurvePtr;

	CurveParser::CurveParser() : RateHelperParser() {
		initIntepolationTypeMap();
		initUnderlyingDataTypeMap();
	};
	CurveParser::CurveParser(const json& data) : RateHelperParser(data) {
		initIntepolationTypeMap();
		initUnderlyingDataTypeMap();
	};

	CurvePtr CurveParser::parsePiecewiseCurve(const json& curve) {

		CurvePtr outputCurve;

		QL::Date referenceDate = parseDate(curve["referenceDate"]);
		QL::DayCounter dayCounter = parseDayCounter(curve["parameters"]["dayCounter"]);

		HelperVector rateHelpers;
		for (auto& rh : curve["rateHelpers"])
		{
			rateHelpers.push_back(parseRateHelper(rh["RateHelper"]));
		}
		Interpolation interp_ = interpolationMap_[curve["parameters"]["interpolation"]];
		UnderlyingData dataType_ = underlyingDataTypeMap_[curve["parameters"]["underlyingData"]];

		if (dataType_ == UnderlyingData::Discounts)
		{
			if (interp_ == Interpolation::Linear)
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::Discount, QL::Linear>(referenceDate, rateHelpers, dayCounter)));
			}
			else if (interp_ == Interpolation::Cubic)
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::Discount, QL::Cubic>(referenceDate, rateHelpers, dayCounter)));
			}
			else if (interp_ == Interpolation::LogLinear)
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::Discount, QL::LogLinear>(referenceDate, rateHelpers, dayCounter)));
			}
			else
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::Discount, QL::Linear>(referenceDate, rateHelpers, dayCounter)));
			}
		}
		else {
			if (interp_ == Interpolation::Linear)
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::ForwardRate, QL::Linear>(referenceDate, rateHelpers, dayCounter)));
			}
			else if (interp_ == Interpolation::Cubic)
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::ForwardRate, QL::Cubic>(referenceDate, rateHelpers, dayCounter)));
			}
			else if (interp_ == Interpolation::LogLinear)
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::ForwardRate, QL::LogLinear>(referenceDate, rateHelpers, dayCounter)));
			}
			else
			{
				outputCurve = std::move(CurvePtr(new QL::PiecewiseYieldCurve<QL::ForwardRate, QL::Linear>(referenceDate, rateHelpers, dayCounter)));
			}
		}

		helpers_ = rateHelpers;
		bool extrapolationEnabled = curve["parameters"]["allowExtrapolation"];
		outputCurve->enableExtrapolation(extrapolationEnabled);
		return std::move(outputCurve);
	}
	CurvePtr CurveParser::parsePureDiscountCurve(const json& curve) {
		std::vector<QL::Date> dates;
		std::vector<QL::DiscountFactor> discounts;
		for (auto& pillar: curve["pillars"])
		{
			dates.push_back(parseDate(pillar["date"]));
			discounts.push_back(pillar["discount"]);
		}
		Interpolation interp_ = interpolationMap_[curve["parameters"]["interpolation"]];
		QL::DayCounter dayCounter = parseDayCounter(curve["parameters"]["dayCounter"]);		
		bool extrapolationEnabled = curve["parameters"]["allowExtrapolation"];
		CurvePtr outputCurve;
		if (interp_ == Interpolation::Linear){		
			outputCurve = std::move(CurvePtr(new QL::InterpolatedDiscountCurve<QL::Linear>(dates, discounts, dayCounter)));
		}
		else if (interp_ == Interpolation::Cubic)
		{
			outputCurve = std::move(CurvePtr(new QL::InterpolatedDiscountCurve<QL::Cubic>(dates, discounts, dayCounter)));
		}
		else if (interp_ == Interpolation::LogLinear)
		{
			outputCurve = std::move(CurvePtr(new QL::InterpolatedDiscountCurve<QL::LogLinear>(dates, discounts, dayCounter)));
		}		
		outputCurve->enableExtrapolation(extrapolationEnabled);
		return std::move(outputCurve);
	}

	void CurveParser::initIntepolationTypeMap() {
		interpolationMap_.insert({ "linear", Interpolation::Linear });
		interpolationMap_.insert({ "cubic", Interpolation::Cubic });
		interpolationMap_.insert({ "logLinear", Interpolation::LogLinear });
	}
	void CurveParser::initUnderlyingDataTypeMap() {
		underlyingDataTypeMap_.insert({ "discounts", UnderlyingData::Discounts });
		underlyingDataTypeMap_.insert({ "f1orwardRates", UnderlyingData::ForwardRates });
	}

}
