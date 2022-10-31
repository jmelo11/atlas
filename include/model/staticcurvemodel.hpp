#pragma once

#include<model/model.hpp>

namespace atlas {

	class StaticCurveModel : public Model {
	public:
		StaticCurveModel(const MarketRequest& marketRequest,
			const std::unordered_map<std::string, std::unique_ptr<YieldCurve>>& discountCurves,
			const std::unordered_map<std::string, std::unique_ptr<YieldCurve>>& forecastCurves,
			DoubleMap<std::string, Date, double> historicalData = DoubleMap<std::string, Date, double>()) :
			Model(marketRequest), discountCurves_(discountCurves), forecastCurves_(forecastCurves) {
			if (checkRefDates())
			{
				if (!discountCurves_.empty()) {
					refDate_ = discountCurves_.begin()->second->refDate();
				}
				if (!forecastCurves_.empty()) {
					refDate_ = forecastCurves_.begin()->second->refDate();
				}
			}
		};

		void setRequest(const MarketRequest& request) {
			marketRequest_ = request;
		}

		bool simulate(const std::vector<Date>& evalDates, Scenario& scenario) const override {
			for (const auto& evalDate : evalDates) {
				MarketData marketData;
				marketData.allocate(marketRequest_);
				//marketData.initialize();
				simulateDiscounts(evalDate, marketData);
				if (!forecastCurves_.empty()) {
					simulateForwards(marketData);
				}
				marketData.refDate = evalDate;
				scenario.push_back(marketData);
			}			
			return true;
		};
		
		/*
		* falta check si la fecha requerida es anterior a la de la curva
		*/
	private:
		
		void simulateDiscounts(const Date& evalDate, MarketData& md) const {
			double df;			
			for (auto& request : marketRequest_.dfs) {				
				const auto& curve = discountCurves_.at(std::get<0>(request));
				const Date& d = std::get<1>(request);
				if (curve->refDate() <= d)
				{
					df = curve->forwardDf(evalDate, d);
				}
				else
				{
					df = 0;
				}
				md.dfs.push_back(df);
			}
		};

		void simulateForwards(MarketData& md) const {
			for (auto& request : marketRequest_.fwds) {
				const auto& curve = forecastCurves_.at(std::get<0>(request));
				const Date& start = std::get<1>(request);
				const Date& end = std::get<2>(request);
				double fwd;
				if (curve->refDate() <= start)
				{
					fwd = curve->forwardRate(start, end);
				}
				else {					
					fwd = historicalData_.at(std::get<0>(request)).at(start);
				}
				md.fwds.push_back(fwd);
			}
		};
		
		bool checkRefDates() {
			if (!discountCurves_.empty()) {
				auto it = discountCurves_.begin();
				Date a = it->second->refDate();
				for (auto& [k, v] : discountCurves_) {
					Date b = v->refDate();
					if (a != b)
					{
						return false;
					}
				}
			}
			if (!forecastCurves_.empty())
			{
				auto it = forecastCurves_.begin();
				Date a = it->second->refDate();
				for (auto& [k, v] : forecastCurves_) {
					Date b = v->refDate();
					if (a != b)
					{
						return false;
					}
				}
			}
			return true;
		}

		Date refDate_;
		const std::unordered_map<std::string, std::unique_ptr<YieldCurve>>& discountCurves_;
		const std::unordered_map<std::string, std::unique_ptr<YieldCurve>>& forecastCurves_;
		DoubleMap<std::string, Date, double> historicalData_;
	};
}