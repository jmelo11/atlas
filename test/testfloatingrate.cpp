#include "pch.h"
#include <atlas.hpp>
#include "commonvars.h"

std::vector<double> calculateCoupons(atlas::FloatingRateInstrument& inst, const std::unique_ptr<atlas::YieldCurve>& curve) {
	std::vector<double> result;
	auto dates = inst.dates();
	for (size_t i = 1; i < dates.size(); i++)
	{
		double yf = commonVars.dayCounter.yearFraction(dates[i - 1], dates[i]);
		if (yf > 0.0 + QL_EPSILON)
		{
			double r = curve->forwardRate(dates[i - 1], dates[i])+ inst.spread();
			atlas::InterestRate rate(r, commonVars.dayCounter, atlas::Compounding::Simple, atlas::Frequency::Annual);
			double amount = inst.outstanding()[i - 1];
			result.push_back(amount * (rate.compoundFactor(dates[i - 1], dates[i]) - 1));
		}
	}
	return result;
}


TEST(TestFloatingBulletConsistency, TestFloatingRate) {

	atlas::FloatingBullet bullet(commonVars.startDate, commonVars.endDate, commonVars.amount, commonVars.paymentFreq, commonVars.spread, commonVars.metaData);
	bullet.calculateOutstanding();
	std::unique_ptr<atlas::ZeroRateCurve<atlas::Cubic>> forecastCurve
		= std::make_unique<atlas::ZeroRateCurve<atlas::Cubic>>(atlas::buildFlatCurve<atlas::Cubic>(commonVars.startDate, commonVars.curveRate));

	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> discountCurves;
	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> forecastCurves;

	forecastCurves["undefined"] = std::move(forecastCurve);

	atlas::ForwardRatendexer indexer;
	indexer.visit(bullet);
	atlas::MarketRequest request;
	indexer.setRequest(request);
	
	atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
	std::vector<atlas::Date>evalDates(1, atlas::Date(1, atlas::Month::August, 2022));
	std::vector<atlas::MarketData> results;
	model.simulate(evalDates, results);
	
	atlas::InterestCalculator caculator(results[0]);
	caculator.visit(bullet);
	
	std::vector<double> coupons = calculateCoupons(bullet, forecastCurves["undefined"]);

	EXPECT_EQ(bullet.interest().size(), coupons.size());
	for (size_t i = 0; i < coupons.size(); i++)
	{
		EXPECT_FLOAT_EQ(bullet.interest().at(i), coupons.at(i));
	}

}

TEST(TestFloatingCustomConsistency, TestFloatingRate) {

	//define bond
	double notional = 100;
	std::vector<atlas::Date> dates = commonVars.schedule.dates();
	std::vector<double> redemptions(commonVars.schedule.size() - 1, notional / (commonVars.schedule.size()-1));
	
	atlas::FloatingCustomStructure bond(dates, redemptions, commonVars.rate, commonVars.metaData);
	bond.calculateOutstanding();


	//market curves for pricing
	std::unique_ptr<atlas::ZeroRateCurve<atlas::Cubic>> forecastCurve
		= std::make_unique<atlas::ZeroRateCurve<atlas::Cubic>>(atlas::buildFlatCurve<atlas::Cubic>(commonVars.startDate, commonVars.curveRate));

	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> discountCurves;
	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> forecastCurves;

	forecastCurves["undefined"] = std::move(forecastCurve);

	//index rates
	atlas::ForwardRatendexer indexer;
	indexer.visit(bond);

	//make request
	atlas::MarketRequest request;
	indexer.setRequest(request);

	//simulate
	atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
	std::vector<atlas::Date>evalDates = std::vector<atlas::Date>(1, commonVars.evalDate);
	std::vector<atlas::MarketData> results;
	model.simulate(evalDates, results);

	//set simulated rates
	atlas::InterestCalculator caculator(results[0]);
	caculator.visit(bond);

	std::vector<double> coupons = calculateCoupons(bond, forecastCurves["undefined"]);
	
	//check number of coupons
	EXPECT_EQ(bond.interest().size(), coupons.size());

	//check interest
	for (size_t i = 0; i < coupons.size(); i++)
	{
		EXPECT_FLOAT_EQ(bond.interest().at(i), coupons.at(i));
	}
}