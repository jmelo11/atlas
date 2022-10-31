#pragma once
#include <model/model.hpp>

namespace atlas {

	/* mvp version */
	class ExternalForwardHW : public Model {
		ExternalForwardHW(const MarketRequest& marketRequest,
			const Date& refDate,			
			const std::vector<std::vector<double>>& shortRates,
			const std::unique_ptr<YieldCurve>& curve, double dt = 1 / 360, double alpha = 0.01, double sigma = 0.01) :
			Model(marketRequest), refDate_(refDate), alpha_(alpha), sigma_(sigma), shortRates_(shortRates), curve_(curve), dt_(dt), nSim_(shortRates[0].size()) {};


		bool simulate(const std::vector<Date>& evalDates, Scenario& scenario) const {
			for (const auto& evalDate : evalDates) {
				MarketData marketData;
				marketData.allocate(marketRequest_);				
				simulateDiscounts(marketData);				
				simulateForwards(marketData);
				
				marketData.refDate = evalDate;
				scenario.push_back(marketData);
				
			}
		}
		
		void resetCounter() {
			counter_ = 0;
		}


	private:

		void simulateForwards(MarketData& md) const {
			Date evalDate = md.refDate;
			const auto& shortRates = shortRates_[counter_];
			for (auto& req : marketRequest_.dfs) {
				Date fwdStartDate = std::get<1>(req);
				Date fwdEndDate = std::get<1>(req);
				if (fwdEndDate < evalDate) {
					md.fwds.push_back(0.0);
				}
				else if (fwdStartDate < evalDate) {
					md.fwds.push_back(0.03); //fix
				}
				else{															
					int today = evalDate - refDate_;
					
					double t0 = today * dt_;
					double t = (fwdStartDate - refDate_)*dt_, T = (fwdEndDate - refDate_) * dt_;					
					double r = shortRates_[counter_][today];
					double df0 = modelZC(t0, t, r), df1 = modelZC(t0, T, r);
					double fwd = (df0 / df1 - 1.0) / (T - t);
					md.fwds.push_back(fwd);
				}				
			}			
		}
		void simulateDiscounts(MarketData& md) const {
			Date evalDate = md.refDate;
			const auto& shortRates = shortRates_[counter_];
			for (auto& req : marketRequest_.dfs) {
				Date dfDate = std::get<1>(req);
				if (dfDate > evalDate) {
					int days = evalDate - refDate_;
					double t = (days) * dt_;
					double T = (dfDate - refDate_) * dt_;
					double r = shortRates_[counter_][days];
					double df = modelZC(t, T, r);
				}
				else if (dfDate == evalDate) {
					md.dfs.push_back(1.0);
				}
				else {
					md.dfs.push_back(0.0);
				}				
			}
		}

		/*pending: simulate spots */
		

		double modelZC(double t, double T, double r) const {
			return A(t, T) * std::exp(-B(t, T) * r);
		}
		
		double A(double t, double T) const {
			double time0 = std::max(t - dt_ / 2.0, 0.0);
			double time1 = t + dt_;
			double implDF = curve_->df(time0) / curve_->df(time1);
			double forward = InterestRate::impliedRate(implDF, Act360(), Compounding::Continuous, Frequency::NoFrequency, dt_);
			double tmp = sigma_ * B(t, T);
			double value = B(t, T) * 
				- 0.25 * tmp * tmp * B(0.0, 2.0 * t);
			return curve_->df(T) / curve_->df(t) * std::exp(value);
		}
		
		double B(double t, double T) const {
			return (1 - std::exp(-alpha_ * (T - t))) / alpha_;
		}

		double alpha_, sigma_, dt_;
		int nSim_, xPos_, counter_ = 0;
		const Date refDate_;
		const std::unique_ptr<YieldCurve>& curve_;
		const std::vector<std::vector<double>>& shortRates_;

	};
}

