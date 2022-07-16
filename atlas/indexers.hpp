#pragma once
#include "visitor.hpp"
#include "marketData.hpp"

namespace atlas {

	/*
	* 1-el check de si la fwd esta agrega un overhead que quizas no compensa simplemente calcular una fwd extra.
	* 2-como asignar la curva que le correponde descontar? Deberia ser un atributo del objecto a valorizar o un parametro del indexador?
	*/
	class DFIndexer : public Visitor {
	public:
		DFIndexer(const std::string& curveName) : curveName_(curveName) {};
		void visit(DAP& inst) override {
			addDFs(inst);
		};
		void visit(FixedBullet& inst) override {
			addDFs(inst);
		}
		void visit(EqualPayment& inst) override {
			addDFs(inst);
		}
		void visit(FloatingBullet& inst) override {
			addDFs(inst);
		}
		void setRequest(MarketRequest& request) {
			request.dfs.insert(request.dfs.end(), dfs_.begin(), dfs_.end());
		}

		//setter, getter
		void curveName(const std::string& curveName) {
			curveName_ = curveName;
		}
		std::string curveName() const {
			return curveName_;
		}
		void clear() {
			dfs_.clear();
		}
	private:
		/*
		* agrega, además del df de cada cupon, --ya no: saltandose el primer factor de descuento--
		*/
		void addDFs(CashFlowInstrument& instrument) {
			auto dates = instrument.dates();
			for (auto d = dates->begin(); d != dates->end(); d++)
			{
				std::tuple<std::string, Date> t(curveName_, *d);
				dfs_.push_back(t);
				instrument.DFIdx()->push_back(dfs_.size() - 1);
			}
		}

		std::vector<std::tuple<std::string, Date>> dfs_;
		std::string curveName_;
	};

	/*
	* en vez de iterar schedule (todas las fechas), se podria hacer un metodo que genere las fechas de inicio y termino del cupon
	* - que pasa si tiene un periodo de gracia?
	*/
	class ForwardRatendexer : public Visitor {
	public:

		void visit(FloatingBullet& inst) override {
			addFwds(inst);
		}

		/* NA */
		void visit(DAP& inst) override {};

		void visit(EqualPayment& inst) override {};

		void visit(FixedBullet& inst) override {};


		void setRequest(MarketRequest& request) {
			request.fwds.insert(request.fwds.end(), fwds_.begin(), fwds_.end());
		}

		void clear() {
			fwds_.clear();
		}
	private:
		/*
		* asume que todos los flotantes tienen indice, casos especiales se pueden manejar con un indice sin frecuencia, o con un visit especial
		* considera que las forwards parten en la fecha de originacion hasta la penultima fecha.
		*/
		void addFwds(FloatingRateInstrument& inst) {
			const char* idxName = inst.index().name_;
			Period p(inst.index().freq_);
			auto dates = inst.dates();
			for (auto d = dates->begin(); d != dates->end() - 1; d++)
			{
				std::tuple<std::string, Date, Date> t(idxName, *d, *d + p);
				fwds_.push_back(t);
				inst.fwdRateIdx()->push_back(fwds_.size() - 1);
			}
		};

		std::vector<std::tuple<std::string, Date, Date>> fwds_;
	};
}