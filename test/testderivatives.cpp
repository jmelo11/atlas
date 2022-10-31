#include "pch.h"
#include <atlas.hpp>
#include "commonvars.h"


TEST(TestVanillaSwapConsistency, TestDerivatives) {

	atlas::VanillaSwap swap(atlas::VanillaSwap::Side::Payer, commonVars.startDate, commonVars.endDate, 
		commonVars.rate, commonVars.paymentFreq, 
		commonVars.paymentFreq, commonVars.amount, 
		commonVars.spread, commonVars.metaData);

	atlas::MarketData marketData;
		
	atlas::InterestCalculator calc(marketData);
	calc.visit(swap);
	
	atlas::Date lastDate = commonVars.startDate;
	std::vector<double> fixCoupons;
	for (const auto& d : commonVars.schedule.dates()) {
		double yf = commonVars.dayCounter.yearFraction(lastDate, d);
		if (yf > 0.0 + QL_EPSILON)
		{
			fixCoupons.push_back(commonVars.amount * (commonVars.rate.compoundFactor(yf) - 1));
		}
		lastDate = d;
	}
	//fix leg
	const auto& fixInterest = swap.legs()[0].interest();
	EXPECT_EQ(fixInterest.size(), fixCoupons.size());
	for (size_t i = 0; i < fixCoupons.size(); i++)
	{
		EXPECT_FLOAT_EQ(fixInterest.at(i), fixCoupons.at(i));
	}

	std::unique_ptr<atlas::ZeroRateCurve<atlas::Cubic>> forecastCurve
		= std::make_unique<atlas::ZeroRateCurve<atlas::Cubic>>(atlas::buildFlatCurve<atlas::Cubic>(commonVars.startDate, commonVars.curveRate));

	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> discountCurves;
	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> forecastCurves;

	forecastCurves["undefined"] = std::move(forecastCurve);

	atlas::ForwardRatendexer indexer;
	indexer.visit(swap);
	atlas::MarketRequest request;
	indexer.setRequest(request);

	atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
	std::vector<atlas::Date>evalDates(1, atlas::Date(1, atlas::Month::August, 2022));
	std::vector<atlas::MarketData> results;
	model.simulate(evalDates, results);

	atlas::InterestCalculator caculator(results[0]);
	caculator.visit(swap);

	std::vector<double> floatCoupons;
	for (const auto& d : commonVars.schedule.dates()) {
		double yf = commonVars.dayCounter.yearFraction(lastDate, d);
		if (yf > 0.0 + QL_EPSILON)
		{
			double r = forecastCurves["undefined"]->forwardRate(lastDate, d);
			atlas::InterestRate rate(r, commonVars.dayCounter, atlas::Compounding::Simple, atlas::Frequency::Annual);
			floatCoupons.push_back(commonVars.amount * (rate.compoundFactor(yf) - 1));
		}
		lastDate = d;
	}
	const auto& floatInterest = swap.legs()[1].interest();
	EXPECT_EQ(floatInterest.size(), floatCoupons.size());
	for (size_t i = 0; i < floatCoupons.size(); i++)
	{
		EXPECT_FLOAT_EQ(floatInterest.at(i), floatCoupons.at(i));
	}
}