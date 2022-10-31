#include "pch.h"
#include <atlas.hpp>


TEST(TestDFIndexer, TestCalculators) {
	atlas::Date startDate(1, atlas::Month::Jan, 2022);
	atlas::Date endDate(1, atlas::Month::Jan, 2025);

	atlas::DayCounter dayCounter = atlas::Thirty360();

	double amount = 100;

	atlas::InterestRate rate(0.03, dayCounter, atlas::Compounding::Simple, atlas::Frequency::Annual);	
	atlas::Frequency freq = atlas::Frequency::Semiannual;
	atlas::MetaData data;
	atlas::FixedBullet bullet(startDate, endDate, freq, amount, rate, data);
	
	bullet.calculateInterests();
	
	atlas::DFIndexer indexer;
	indexer.visit(bullet);
	const std::vector<size_t>& idx = bullet.DFIdx();
	//empty indexer
	for (size_t i = 0; i < bullet.interest().size(); i++)
	{
		EXPECT_EQ(idx.at(i), i);
	}
}

TEST(TestFwdIndexer, TestCalculators) {
	atlas::Date startDate(1, atlas::Month::Jan, 2022);
	atlas::Date endDate(1, atlas::Month::Jan, 2025);

	atlas::DayCounter dayCounter = atlas::Thirty360();

	double amount = 100;

	atlas::Frequency freq = atlas::Frequency::Semiannual;
	atlas::MetaData data;
	atlas::FloatingBullet bullet(startDate, endDate, amount, freq, 0, data);
	
	atlas::ForwardRatendexer indexer;
	indexer.visit(bullet);
	
	const std::vector<size_t>& idx = bullet.FWDIdx();
	
	//empty indexer
	for (size_t i = 0; i < idx.size(); i++)
	{
		EXPECT_EQ(idx.at(i), i);
	}
}

TEST(TestNPVFixedInstrument, TestCalculators) {
	atlas::Date startDate(1, atlas::Month::Jan, 2022);
	atlas::Date endDate(1, atlas::Month::Jan, 2025);

	atlas::DayCounter dayCounter = atlas::Thirty360();

	double amount = 100;

	atlas::InterestRate rate(0.03, dayCounter, atlas::Compounding::Simple, atlas::Frequency::Annual);
	atlas::Frequency freq = atlas::Frequency::Semiannual;
	atlas::MetaData data;
	atlas::FixedBullet bullet(startDate, endDate, freq, amount, rate, data);

	bullet.calculateInterests();

	atlas::DFIndexer indexer;
	indexer.visit(bullet);


	atlas::MarketRequest request;
	indexer.setRequest(request);

	std::unique_ptr<atlas::ZeroRateCurve<atlas::Cubic>> discountCurve
		= std::make_unique<atlas::ZeroRateCurve<atlas::Cubic>>(atlas::buildFlatCurve<atlas::Cubic>(startDate, 0.03));
	std::unique_ptr<atlas::ZeroRateCurve<atlas::Cubic>> forecastCurve
		= std::make_unique<atlas::ZeroRateCurve<atlas::Cubic>>(atlas::buildFlatCurve<atlas::Cubic>(startDate, 0.03));

	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> discountCurves;
	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> forecastCurves;

	discountCurves["undefined"] = std::move(discountCurve);
	forecastCurves["undefined"] = std::move(forecastCurve);

	atlas::StaticCurveModel model(request, discountCurves, forecastCurves);

	std::vector<atlas::Date>evalDates(1, discountCurves["undefined"]->refDate());
	atlas::Scenario results;
	model.simulate(evalDates, results);
	
	atlas::NPVCalculator npvCalc(results[0]);
	bullet.accept(npvCalc);
	
	double npv = 0;
	for (size_t i = 0; i < bullet.interest().size(); i++)
		npv += (bullet.interest().at(i) + bullet.redemptions().at(i)) * discountCurves["undefined"]->df(bullet.dates().at(i+1));
	
	EXPECT_FLOAT_EQ(npv, npvCalc.result()); 
}

TEST(TestParRateFixedInstrument, TestCalculators) {
	atlas::Date startDate(1, atlas::Month::Jan, 2022);
	atlas::Date endDate(1, atlas::Month::Jan, 2025);

	atlas::DayCounter dayCounter = atlas::Thirty360();

	double amount = 100;

	atlas::InterestRate rate(0.03, dayCounter, atlas::Compounding::Simple, atlas::Frequency::Annual);
	atlas::Frequency freq = atlas::Frequency::Semiannual;
	atlas::MetaData data;
	atlas::FixedBullet bullet(startDate, endDate, freq, amount, rate, data);

	bullet.calculateInterests();

	atlas::DFIndexer indexer;
	indexer.visit(bullet);

	atlas::MarketRequest request;
	indexer.setRequest(request);

	std::unique_ptr<atlas::ZeroRateCurve<atlas::Cubic>> discountCurve
		= std::make_unique<atlas::ZeroRateCurve<atlas::Cubic>>(atlas::buildFlatCurve<atlas::Cubic>(startDate, 0.03));
	std::unique_ptr<atlas::ZeroRateCurve<atlas::Cubic>> forecastCurve
		= std::make_unique<atlas::ZeroRateCurve<atlas::Cubic>>(atlas::buildFlatCurve<atlas::Cubic>(startDate, 0.03));

	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> discountCurves;
	std::unordered_map<std::string, std::unique_ptr<atlas::YieldCurve>> forecastCurves;

	discountCurves["undefined"] = std::move(discountCurve);
	forecastCurves["undefined"] = std::move(forecastCurve);

	atlas::StaticCurveModel model(request, discountCurves, forecastCurves);
	std::vector<atlas::Date>evalDates(1, discountCurves["undefined"]->refDate());
	std::vector<atlas::MarketData> results;
	model.simulate(evalDates, results);
	

	atlas::ParRateSolver rateSolver(results[0]);
	bullet.accept(rateSolver);	

	EXPECT_FLOAT_EQ(0.02936255, rateSolver.result());

}
