
#include <iostream>
#include "curvemanager/curvemanager.hpp"


using namespace CurveManager;

int main()
{
	CurveStore store;

	DataBase::DataManager db = DataBase::setupDB();
	CurveBuilder builder(store, db);
	
	Date refDate(25, Month::Jul, 2022);
	boost::shared_ptr<YieldTermStructure> curve = builder.get(refDate,"CF_CLP");
	std::cout << curve->discount(1) << "\n";
}

