#pragma once
#include <map>
#include <unordered_map>
#include <string>

#include "curvemanager/qlconfig.hpp"

namespace CurveManager {
	class CurveStore {

	public:
		CurveStore() {};
		boost::shared_ptr<YieldTermStructure> getCurve(const Date& refDate, const std::string& name) {
			return curveMap_[refDate][name];
		};

		bool hasCurve(const Date& refDate, const std::string& name) {
			if (curveMap_.find(refDate) != curveMap_.end())
				return curveMap_[refDate].find(name) != curveMap_[refDate].end();
			else
				return false;
			return false;
		}
		bool hasIndex(Date refDate, const std::string& name) {
			if (indexMap_.find(refDate) != indexMap_.end())
				return indexMap_[refDate].find(name) != indexMap_[refDate].end();
			else
				return false;
			return false;
		}
		void addCurve(Date date, const std::string& name, boost::shared_ptr<YieldTermStructure>& curve) {
			curveMap_[date][name] = std::move(curve);
		}

	private:
		std::map<Date, std::unordered_map<std::string, boost::shared_ptr<YieldTermStructure>>> curveMap_;
		std::map<Date, std::unordered_map<std::string, boost::shared_ptr<YieldTermStructure>>> indexMap_;
	};
	
}