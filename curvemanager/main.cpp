
#include <iostream>

#include "curvemanager.hpp"

using namespace CurveManager;
int main()
{
	CurveStore store;
	CurveBuilder builder(store);

	
	Date refDate(21, Month::July, 2022);
	Settings::instance().evaluationDate() = refDate;

	std::shared_ptr<YieldTermStructure> curve = builder.get(refDate, "EUR");
	
	std::cout << curve->discount(1) << "\n";
}

