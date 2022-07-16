#pragma once
#include "visitor.hpp"
#include "marketData.hpp"

namespace atlas {
	class InterestCalculator : public Visitor {
	public:
		InterestCalculator(const MarketData& marketData): marketData_(marketData){}
		
		void visit(FloatingBullet& inst) override {
			forecast(inst);
		}
		void visit(DAP& inst) override {};

		void visit(EqualPayment& inst) override {};

		void visit(FixedBullet& inst)override {};
	
		void forecast(FloatingRateInstrument& instrument) {
			if (instrument.outstanding()->empty())
				instrument.calcOutstanding();
			auto notionals = instrument.outstanding();
			auto dates = instrument.dates();
			auto interest = instrument.interest();
			auto fwdIdxs = instrument.fwdRateIdx();
			size_t n = fwdIdxs->size();
			RateIndex index = instrument.index();

			interest->clear();
			double spread = instrument.spread();

			for (size_t i = 0; i < n; i++)
			{
				double fwd = marketData_.fwds[fwdIdxs->at(i)];
				InterestRate r(fwd + spread, index.dayCounter_, index.comp_, index.freq_);
				double x = notionals->at(i) * (r.compoundFactor(dates->at(i), dates->at(i+1)) - 1);
				interest->push_back(x);
			}
		}

	private:
		const MarketData& marketData_;
	};

	class NPVCalculator : public Visitor {
	public:
		NPVCalculator(const MarketData& marketData): marketData_(marketData) {}
		
		void visit(DAP& inst) override{
			npv(inst);
		}		

		void visit(EqualPayment& inst) override {
			npv(inst);
		}

		void visit(FixedBullet& inst) override {
			npv(inst);
		}

		void visit(FloatingBullet& inst) override {
			npv(inst);
		}
		
		void reset() {
			npv_ = 0;
		}
		double result() const {
			return npv_;
		}
	private:

		/*descuenta los flujos, partiendo del primer cupon y no desde el desembolso*/
		void npv(CashFlowInstrument& inst) {
			auto v = inst.DFIdx();
			auto interest = inst.interest();
			auto redemptions = inst.redemptions();
			for (size_t i = 0; i < interest->size(); i++)
			{
				size_t pos = v->at(i+1);
				double df = marketData_.dfs[pos];
				npv_ += (interest->at(i) + redemptions->at(i)) * df;
			}
		}

		const MarketData& marketData_;
		double npv_ = 0;		
	};

	class ParRateSolver : public Visitor {
	public:
		ParRateSolver(const MarketData& marketData, double accuracy = 0.0001, double guess = 0.01, double step = 0.0001)
			: marketData_(marketData), accuracy_(accuracy), guess_(guess), step_(step), npvCalc_(NPVCalculator(marketData)) {}
		
		void visit(DAP& inst) override {
			double startDF = marketData_.dfs[inst.DFIdx()->front()];

			auto f = [&](double rate) {
				inst.setRate(rate);
				inst.fillInterest();

				inst.accept(npvCalc_);
				double npv = npvCalc_.result();
				npvCalc_.reset();
				return npv - inst.faceAmount()*startDF;
			};		
			rate_ = solver_.solve(f, accuracy_, guess_, step_);
		}

		void visit(FixedBullet& inst) override {
			double startDF = marketData_.dfs[inst.DFIdx()->front()];

			auto f = [&](double rate) {
				inst.setRate(rate);
				inst.fillInterest();

				inst.accept(npvCalc_);
				double npv = npvCalc_.result();
				npvCalc_.reset();
				return npv - inst.faceAmount() * startDF;
			};
			rate_ = solver_.solve(f, accuracy_, guess_, step_);
		}
	
		void visit(EqualPayment& inst) override {
			double startDF = marketData_.dfs[inst.DFIdx()->front()];
			auto f = [&](double rate) {
				inst.setRate(rate);
				inst.calculateNotionals();

				inst.accept(npvCalc_);
				double npv = npvCalc_.result();
				npvCalc_.reset();
				return npv - inst.faceAmount() * startDF;
			};
			rate_ = solver_.solve(f, accuracy_, guess_, step_);
		}

		void visit(FloatingBullet& inst) override {}

		double result() const {
			return rate_;
		}

	private:
		BrentSolver solver_;
		double accuracy_ = 0;
		double guess_ = 0;
		double step_ = 0;

		NPVCalculator npvCalc_;
		const MarketData& marketData_;
		double rate_ = 0;
	};
}