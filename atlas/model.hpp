#pragma once

#include "marketData.hpp"
#include "curve.hpp"

namespace atlas {
	class Model {
	public:
		Model(const MarketRequest& marketRequest) : marketRequest_(marketRequest) {}
		virtual void simulate(MarketData& md) = 0;
	protected:
		const MarketRequest& marketRequest_;
	};

	class StaticCurveModel : public Model {
	public:
		StaticCurveModel(const MarketRequest& marketRequest,
			std::map < std::string, std::shared_ptr<YieldCurve>> discountCurves,
			std::map<std::string, std::shared_ptr<YieldCurve>> forecastCurves,
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
		void simulate(MarketData& md) override {
			simulateDiscounts(md);
			simulateForwards(md);
			md.refDate = refDate_;
		};
		/*
		* falta check si la fecha requerida es anterior a la de la curva
		*/
		void simulateDiscounts(MarketData& md) {	
			double df;
			for (auto& request : marketRequest_.dfs) {
				const std::shared_ptr<YieldCurve> curve = discountCurves_[std::get<0>(request)];
				Date d = std::get<1>(request);
				if (curve->refDate()<=d)
				{
					df = curve->df(d);
				}
				else {
					df = 0;
				}
				md.dfs.push_back(df);
			}			
		};
		void simulateForwards(MarketData& md) {
			double fwd;
			for (auto& request : marketRequest_.fwds) {
				std::shared_ptr<YieldCurve> curve = forecastCurves_[std::get<0>(request)];
				Date start = std::get<1>(request);
				Date end = std::get<2>(request);
				if (curve->refDate()<=start)
				{
					fwd = curve->forwardRate(start,end);
				}
				else {
					//aqui debe ir a buscar fixing historico
					fwd = historicalData_[std::get<0>(request)][start];
				}
				md.fwds.push_back(fwd);
			}				
		};
		
	private:
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
		std::map<std::string, std::shared_ptr<YieldCurve>> discountCurves_;
		std::map<std::string, std::shared_ptr<YieldCurve>> forecastCurves_;
		DoubleMap<std::string, Date, double> historicalData_;
	};
}
