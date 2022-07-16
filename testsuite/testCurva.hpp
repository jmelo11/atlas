#pragma once

#include "atlas.hpp"
#include "auxF.hpp"

using namespace atlas;

/*test basico */
void testSimpleCurva() {
	Date today(23, (Month)5, 2022);
	std::vector<Date> dates{
		Date(23, (Month)5, 2022),
		Date(23, (Month)5, 2023),
		Date(23, (Month)5, 2050)
	};
	std::vector<double> rates(3,0.03);
	ZeroRateCurve curve(dates, rates);

	for (const Date& d: dates)
		std::cout << "DF (" << d << "): " << curve.df(d) << "\n";

	for (const Date& d : dates)
		std::cout << "Fr (" << today <<", " << d << "): " << curve.forwardRate(today, d) << "\n";
	std::cout << "\n";
}

/* test curva CF */
void testCurvaCF() {
	std::string date = "07062022";
	std::string curveName = "cf_clp";
	ZeroRateCurve curve = queryCurve<Linear>(curveName, date);
	for (size_t i = 0; i < 20; i++)
	{
		std::cout << "DF(" << (double)i / 2 << "y)" << curve.df((double)i / 2) << "\n";
	}
}

void testCurvas() {
	testSimpleCurva();
	testCurvaCF();
	//testConn();
	//testConn2();
}
