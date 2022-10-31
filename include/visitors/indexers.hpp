#pragma once

#include <visitors/visitor.hpp>
#include <data/marketData.hpp>
#include <algorithm>

namespace atlas {

	/*
	* 1-el check de si la fwd esta agrega un overhead que quizas no compensa simplemente calcular una fwd extra.
	* 2-como asignar la curva que le correponde descontar? Deberia ser un atributo del objecto a valorizar o un parametro del indexador?
	*/
	class DFIndexer : public Visitor {
	public:
		DFIndexer() {};
		void visit(DAP& inst) override {
			addDFs(inst.leg());
		};
		void visit(FixedBullet& inst) override {
			addDFs(inst.leg());
		}
		void visit(EqualPayment& inst) override {
			addDFs(inst.leg());
		}
		void visit(FloatingBullet& inst) override {
			addDFs(inst.leg());
		}
		
		void visit(FloatingCustomStructure& inst) override {
			addDFs(inst.leg());
		}
		
		void visit(FixedCustomStructure& inst) override {
			addDFs(inst.leg());
		}

		void visit(VanillaSwap& inst) override {
			addDFs(inst.legs()[0]);
			addDFs(inst.legs()[1]);
		}
		
		void setRequest(MarketRequest& request) {
			request.dfs.insert(request.dfs.end(), dfs_.begin(), dfs_.end());
		}

		void clear() {
			dfs_.clear();
		}
	private:
		/*
		* agrega, además del df de cada cupon,  el primer factor de descuento
		*/
		void addDFs(Leg& leg) {
			const auto& dates = leg.dates();			
			const std::string& discountCurve = leg.metaData().discountCurve_;						
			for (const auto& d : dates)
			{
				std::tuple<std::string, Date> t(discountCurve, d);
				dfs_.push_back(t);
				leg.DFIdx().push_back(dfs_.size() - 1);
			}
		}
			
		std::vector<std::tuple<std::string, Date>> dfs_;
	};

	/*
	* en vez de iterar schedule (todas las fechas), se podria hacer un metodo que genere las fechas de inicio y termino del cupon
	* - que pasa si tiene un periodo de gracia?
	*/
	class ForwardRatendexer : public Visitor {
	public:

		void visit(FloatingBullet& inst) override {
			addFwds(inst.leg());
		}

		void visit(DAP& inst) override {};

		void visit(EqualPayment& inst) override {};

		void visit(FixedBullet& inst) override {};

		void visit(FixedCustomStructure& inst) override {};
		
		void visit(FloatingCustomStructure& inst) override {
			addFwds(inst.leg());
		};
		
		void visit(VanillaSwap& inst) override {
			addFwds(inst.legs()[1]);
		}
		
		void setRequest(MarketRequest& request) {
			request.fwds.insert(request.fwds.end(), fwds_.begin(), fwds_.end());
		};

		void clear() {
			fwds_.clear();
		}
	private:
		/*
		* asume que todos los flotantes tienen indice, casos especiales se pueden manejar con un indice sin frecuencia, o con un visit especial
		* considera que las forwards parten en la fecha de originacion hasta la penultima fecha.
		*/
		void addFwds(Leg& leg) {
			const std::string& idxName = leg.metaData().index_.name_;
			Period p(leg.metaData().index_.fixingFreq_);
			
			const auto& dates = leg.dates();
			size_t n = dates.size();			
			for (size_t i = 0; i< n-1; i++)
			{		

				Date tmp = dates.at(i);
				std::tuple<std::string, Date, Date> t(idxName, tmp, tmp+p);
				fwds_.push_back(t);
				leg.FWDIdx().push_back(fwds_.size() - 1);
			}		
		};
		std::vector<std::tuple<std::string, Date, Date>> fwds_;
	};

	class ForwardAndDiscountIndexer: public Visitor{
	public:
		void visit(DAP& inst) override {
			addFwdsAndDfs(inst.leg(), false);
		};

		void visit(EqualPayment& inst) override {
			addFwdsAndDfs(inst.leg(), false);
		};

		void visit(FixedBullet& inst) override {
			addFwdsAndDfs(inst.leg(), false);
		};

		void visit(FixedCustomStructure& inst) override {
			addFwdsAndDfs(inst.leg(), false);
		};
		void visit(FloatingBullet& inst) override {
			addFwdsAndDfs(inst.leg(), true);
		}
		void visit(FloatingCustomStructure& inst) override {
			addFwdsAndDfs(inst.leg(), true);
		};

		void visit(VanillaSwap& inst) override {
			addFwdsAndDfs(inst.legs()[0], false);
			addFwdsAndDfs(inst.legs()[1], true);
		}

		void setRequest(MarketRequest& request) {
			request.dfs.insert(request.dfs.end(), dfs_.begin(), dfs_.end());
			request.fwds.insert(request.fwds.end(), fwds_.begin(), fwds_.end());
		};

	private:

		void addFwdsAndDfs(Leg& leg, bool addFwd) {
			
			const std::string& idxName = leg.metaData().index_.name_;
			const std::string& discountCurve = leg.metaData().discountCurve_;
						
			const auto& dates = leg.dates();			
			Period p(leg.metaData().index_.fixingFreq_);

			size_t n = dates.size();
			for (size_t i = 0; i < n; i++)
			{
				Date evalDate = dates.at(i);
				std::tuple<std::string, Date> t0(discountCurve, evalDate);
				dfs_.push_back(t0);
				leg.DFIdx().push_back(dfs_.size() - 1);
				if (addFwd && i < n-1){
					std::tuple<std::string, Date, Date> t1(idxName, evalDate, evalDate + p);
					fwds_.push_back(t1);
					leg.FWDIdx().push_back(fwds_.size() - 1);
				}				
			}
		};

		std::vector<std::tuple<std::string, Date, Date>> fwds_;
		std::vector<std::tuple<std::string, Date>> dfs_;
	};
}