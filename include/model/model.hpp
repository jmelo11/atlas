#pragma once


#include <data/marketData.hpp>
#include <curves/curve.hpp>
#include <queue>

namespace atlas {
	
	using Scenario = std::vector<MarketData>;
	
	class Model {
	public:
		Model() {};
		Model(const MarketRequest& marketRequest) : marketRequest_(marketRequest) {}
		
		virtual bool simulate(const std::vector<Date>& evalDates, Scenario& scenario) const = 0;
		
	protected:
		MarketRequest marketRequest_;
	};

}
