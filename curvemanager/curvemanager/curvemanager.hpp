#pragma once

#include <any>
#include <memory>
#include <iostream>

#include "..\database\database.hpp"
#include "curvestore.hpp"
#include "configurations.hpp"

namespace CurveManager {

	class CurveBuilder
	{
		
	public:
		
		CurveBuilder(CurveStore& curveStore, DataBase::DataManager& db) : curveStore_(curveStore), db_(db) {}

		/* queries the parameters from db, parse them, build the ratehelpers with the prices and parameters, then build curve */
		boost::shared_ptr<YieldTermStructure> get(const Date& refDate, const std::string& name) {
			if (!curveStore_.hasCurve(refDate, name)) {
				boost::shared_ptr<BaseConfiguration> configurations = makeCurveConfig(refDate, name);
				build(configurations);
			}
			return curveStore_.getCurve(refDate, name);
		};

	private:

		/* builds the curve with the given rate helpers /fixed interpoaltion and data type */
		void build(boost::shared_ptr<BaseConfiguration> config);

		/* queries the parameters and px */
		boost::shared_ptr<RateHelper> buildHelper(boost::shared_ptr<BaseConfiguration> config);

		/* this methods gets the config and prices from the db for each rate helper in the curve*/
		std::vector<boost::shared_ptr<BaseConfiguration>> makeHelperConfigs(const Date& refDate, int curve_id);
		boost::shared_ptr<BaseConfiguration> makeHelperConfigFromDB(int helperId, const std::string& type, const Date& refDate);
		std::any parseStringFields(const std::string& field, const std::string& value, const Date& refDate);
		
		/* this method gets the config for each curve*/
		boost::shared_ptr<BaseConfiguration> makeCurveConfig(const Date& refDate, const std::string& name);

		CurveStore& curveStore_;
		DataBase::DataManager& db_;
	};
	
}


