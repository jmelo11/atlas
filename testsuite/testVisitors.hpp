#pragma once
#include "auxF.hpp"
#include "atlas.hpp"

using namespace atlas;

void testDFIndx() {
	Date start = Date(1, Month::Jan, 2021);
	Date end = Date(1, Month::Aug, 2021);
	double rate_A = 0.03;
	double notional = 100;
	DAP dap_A(start, end, notional, rate_A);

	Date end2 = Date(1, Month::Aug, 2022);
	DAP dap_B(start, end2, notional, rate_A);

	DFIndexer df("test");

	dap_A.accept(df);
	dap_B.accept(df);

	MarketRequest request;
	df.setRequest(request);
	
	std::cout << "Curve: " << std::get<0>(request.dfs[0]) << ", Date: " << std::get<1>(request.dfs[0]) << "\n";
	std::cout << dap_A.DFIdx()->back() << "\n";

	std::cout << "Curve: " << std::get<0>(request.dfs[1]) << ", Date: " << std::get<1>(request.dfs[1]) << "\n";
	std::cout << dap_B.DFIdx()->back() << "\n";
}

/*
* portafolio/secuencial
* 1 - construir portfolio
* 2 - buscar factores de descuentos necesarios
* 3 - calcular factores de descuento
* 4 - calcular npv portafolio
*/
void testFullSec() {
	std::vector<DAP> daps;
	{
		Timer t0;
		daps = buildDapPortfolio();
	}
	
	Date today(23, (Month)5, 2022);
	DFIndexer df("test");
	{
		Timer t1;
		for (DAP& dap : daps)
			if (dap.isAlive(today))
				dap.accept(df);
	}


	MarketRequest request;
	df.setRequest(request);
	std::shared_ptr<ZeroRateCurve<Cubic>> curve = std::make_shared<ZeroRateCurve<Cubic>>(buildFlatCurve<Cubic>(today, 0.03));
	std::map<std::string, std::shared_ptr<YieldCurve>> discountCurves;
	std::map<std::string, std::shared_ptr<YieldCurve>> forecastCurves;
	
	discountCurves["test"] = curve;

	MarketData data;
	{
		Timer t2;
		StaticCurveModel model(request, discountCurves, forecastCurves);
		model.simulate(data);
	}

	NPVCalculator calculator(data);
	{
		Timer t3;

		for (DAP& dap : daps) {
			if (dap.isAlive(today)) {
				dap.accept(calculator);
			}				
		}			
	}
	double npv = calculator.result();
	std::cout << "NPV: " << npv << "\n";
}

void testIndexers() {
	RateIndex libor6m("libor6m", Thirty360(Thirty360::BondBasis), Frequency::Semiannual, Compounding::Simple);
	
	Date start = Date(23, (Month)5, 2021);
	Date end = Date(23, (Month)5, 2025);
	
	FloatingBullet floating(start, end, 100, 0, libor6m);

	DFIndexer dfIndexer("discountCurve");
	ForwardRatendexer fwdIndexer;
	floating.accept(dfIndexer);
	floating.accept(fwdIndexer);
	
	MarketRequest request;
	dfIndexer.setRequest(request);
	fwdIndexer.setRequest(request);

	std::shared_ptr<ZeroRateCurve<Cubic>> discountCurve = std::make_shared<ZeroRateCurve<Cubic>>(buildFlatCurve<Cubic>(start, 0.03));
	std::shared_ptr<ZeroRateCurve<Cubic>> forecastCurve = std::make_shared<ZeroRateCurve<Cubic>>(buildFlatCurve<Cubic>(start, 0.03));
 
	std::map<std::string, std::shared_ptr<YieldCurve>> discountCurves;
	std::map<std::string, std::shared_ptr<YieldCurve>> forecastCurves;

	discountCurves["discountCurve"] = discountCurve;
	forecastCurves["libor6m"] = forecastCurve;

	StaticCurveModel model(request, discountCurves, forecastCurves);
	MarketData data;
	model.simulate(data);
	
	InterestCalculator interestCacl(data);
	floating.accept(interestCacl);

	NPVCalculator npvCalc(data);
	floating.accept(npvCalc);

	std::cout << "Floating NPV: " << npvCalc.result() << "\n";
}

void testSolver() {
	Date start = Date(23, (Month)5, 2021);
	Date end = Date(23, (Month)5, 2027);
	double notional = 100;
	InterestRate coupon(0.0278133, Act360(), Compounding::Simple, Frequency::Annual);
	FixedBullet bond(start, end, Frequency::Semiannual, notional, coupon);


	/*setup discount factors*/
	DFIndexer dfIndexer("discountCurve");
	bond.accept(dfIndexer);

	MarketRequest request;
	dfIndexer.setRequest(request);

	/*curvas test*/
	std::shared_ptr<ZeroRateCurve<Cubic>> discountCurve = std::make_shared<ZeroRateCurve<Cubic>>(buildFlatCurve<Cubic>(start, 0.03));

	std::map<std::string, std::shared_ptr<YieldCurve>> discountCurves;
	std::map<std::string, std::shared_ptr<YieldCurve>> forecastCurves;
	discountCurves["discountCurve"] = discountCurve;

	/*generar dfs*/
	StaticCurveModel model(request, discountCurves, forecastCurves);
	MarketData data;
	model.simulate(data);
	
	NPVCalculator npvCalc(data);
	bond.accept(npvCalc);
	std::cout << "NPV: " << npvCalc.result() << "\n";

	ParRateSolver solver(data);
	bond.accept(solver);

	std::cout << "Par Rate: " << solver.result() << "\n";
}

void testSolver2() {
	std::string evalDate = "09062022";
	std::string evalCurve = "cf_clp";

	Date start = fastDateParser(evalDate) + Period(1, TimeUnit::Months);
	Date end = start+Period(5, TimeUnit::Years);
	double notional = 100;
	InterestRate coupon(0, Thirty360(Thirty360::BondBasis), Compounding::Compounded, Frequency::Annual);
	//FixedBullet bond(start, end, Frequency::Semiannual, notional, coupon);
	EqualPayment bond(start, end, Frequency::Monthly, notional, coupon);

	/*setup discount factors*/
	DFIndexer dfIndexer(evalCurve);
	bond.accept(dfIndexer);

	MarketRequest request;
	dfIndexer.setRequest(request);

	/*curvas test*/
	std::shared_ptr<ZeroRateCurve<Linear>> discountCurve = std::make_shared<ZeroRateCurve<Linear>>(queryCurve<Linear>(evalCurve, evalDate));

	std::map<std::string, std::shared_ptr<YieldCurve>> discountCurves;
	std::map<std::string, std::shared_ptr<YieldCurve>> forecastCurves;
	discountCurves[evalCurve] = discountCurve;

	/*generar dfs*/
	StaticCurveModel model(request, discountCurves, forecastCurves);
	MarketData data;
	model.simulate(data);

	NPVCalculator npvCalc(data);
	bond.accept(npvCalc);
	std::cout << "NPV: " << npvCalc.result() << "\n";

	ParRateSolver solver(data);
	bond.accept(solver);

	std::cout << "Par Rate: " << solver.result() << "\n";
}

void testVisitors() {
	testDFIndx();
	//testFullSec();
	testIndexers();
	testSolver2();
}